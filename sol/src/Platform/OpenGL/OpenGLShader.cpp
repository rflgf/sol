#include "OpenGLShader.h"

#include "Core.h"
#include "Platform/OpenGL/OpenGLContext.h"
#include "Utils.h"

#include <glm/gtc/type_ptr.hpp>
#include <regex>

namespace sol
{

OpenGLShader::OpenGLShader(const std::string &filepath)
{
	size_t last_slash = filepath.find_last_of("/\\");
	last_slash        = last_slash == std::string::npos ? 0 : last_slash + 1;
	size_t last_dot   = filepath.rfind('.');
	size_t count = last_dot == std::string::npos ? filepath.size() - last_slash
	                                             : last_dot - last_slash;
	name         = filepath.substr(last_slash, count);

	std::string source = utils::read_from_file(filepath);
	std::unordered_map<Shader::Step, std::string> steps = parse(source);
	SOL_CORE_ASSERT(steps.contains(Step::VERTEX) &&
	                    steps.contains(Step::FRAGMENT),
	                "missing required shader step in {}", filepath);
	compile(steps);
}

OpenGLShader::OpenGLShader(const std::string &name,
                           const std::string &vertex_source,
                           const std::string &fragment_source)
    : name(name)
{
	compile({{Step::FRAGMENT, fragment_source}, {Step::VERTEX, vertex_source}});
}

std::unordered_map<Shader::Step, std::string>
OpenGLShader::parse(const std::string &source)
{
	std::unordered_map<Shader::Step, std::string> steps;
	const char step_token[] = "#step";
	size_t cursor           = 0;
	while ((cursor = source.find(step_token, cursor)) != std::string::npos)
	{
		cursor += strlen(step_token);
		while (source[cursor] == ' ' || source[cursor] == '\t' ||
		       source[cursor] == '\n' || source[cursor] == '\r')
			++cursor;
		std::regex new_line_chars("\\s");
		std::cmatch match;
		size_t end_of_step_value = 0;
		if (std::regex_search(&source[cursor], match, new_line_chars))
			end_of_step_value = match.prefix().length();
		std::string step_value_string =
		    source.substr(cursor, end_of_step_value);
		Step step = step_from_string(step_value_string);
		// next "#step", i.e., next shader source.
		cursor += end_of_step_value;
		size_t end_of_current_shader = source.find("#step", cursor);
		// case last shader source.
		if (end_of_current_shader == std::string::npos)
			end_of_current_shader = source.size();
		std::string current_shader_source = source.substr(
		    cursor, end_of_current_shader - cursor);
		steps[step] = current_shader_source;
		cursor      = end_of_current_shader;
	}
	return steps;
}

void OpenGLShader::compile(
    const std::unordered_map<Shader::Step, std::string> &sources)
{
	std::vector<GLuint> ids;

	for (auto [step, source] : sources)
	{
		GLuint shader = glCreateShader(gl_step_from_sol_internal_step(step));

		const GLchar *gl_compliant_source =
		    static_cast<const GLchar *>(source.c_str());
		glShaderSource(shader, 1, &gl_compliant_source, 0);

		glCompileShader(shader);

		GLint compiled = 0;
		glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);
		if (compiled == GL_FALSE)
		{
			GLint max_length = 0;
			glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &max_length);

			std::vector<GLchar> error(max_length);
			glGetShaderInfoLog(shader, max_length, &max_length, &error[0]);

			glDeleteShader(shader);

			SOL_CORE_ERROR("{}", error);
			SOL_CORE_ASSERT(false,
			                "shader compilation error on shader of step {}",
			                Shader::step_to_string(step));
		}

		ids.push_back(shader);
	}

	id = glCreateProgram();

	for (GLuint shader_id : ids)
		glAttachShader(id, shader_id);

	glLinkProgram(id);

	GLint linked = 0;
	glGetProgramiv(id, GL_LINK_STATUS, static_cast<int *>(&linked));
	if (linked == GL_FALSE)
	{
		GLint max_length = 0;
		glGetProgramiv(id, GL_INFO_LOG_LENGTH, &max_length);

		std::vector<GLchar> error(max_length);
		glGetProgramInfoLog(id, max_length, &max_length, &error[0]);

		glDeleteProgram(id);
		for (GLuint shader_id : ids)
			glDeleteShader(shader_id);

		SOL_CORE_ERROR("{}", error);
		SOL_CORE_ASSERT(false, "shader linking error");
	}

	for (GLuint shader_id : ids)
		glDetachShader(id, shader_id);
}

GLenum OpenGLShader::gl_step_from_sol_internal_step(Shader::Step step)
{
	switch (step)
	{
	case Step::FRAGMENT:
		return GL_FRAGMENT_SHADER;
	case Step::VERTEX:
		return GL_VERTEX_SHADER;

	case Step::NONE:
	default:
		return 0;
	}
}

OpenGLShader::~OpenGLShader() { glDeleteProgram(id); }

void OpenGLShader::bind() const { glUseProgram(id); }

void OpenGLShader::unbind() const { glUseProgram(0); }

void OpenGLShader::upload_uniform_int(std::string name,
                                      const int32_t value) const
{
	GLint location = get_uniform_location(name);
	glUniform1i(location, value);
}

void OpenGLShader::upload_uniform_int_array(const std::string &name,
                                            int32_t *value, size_t count) const
{
	GLint location = get_uniform_location(name);
	glUniform1iv(location, count, value);
}

void OpenGLShader::upload_uniform_unsigned_int(std::string name,
                                               const uint32_t value) const
{
	GLint location = get_uniform_location(name);
	glUniform1ui(location, value);
}

void OpenGLShader::upload_uniform_float(std::string name,
                                        const float_t value) const
{
	GLint location = get_uniform_location(name);
	glUniform1f(location, value);
}

void OpenGLShader::upload_uniform_float_2(std::string name,
                                          const glm::vec2 value) const
{
	GLint location = get_uniform_location(name);
	glUniform2f(location, value.x, value.y);
}

void OpenGLShader::upload_uniform_float_3(std::string name,
                                          const glm::vec3 &value) const
{
	GLint location = get_uniform_location(name);
	glUniform3f(location, value.x, value.y, value.z);
}

void OpenGLShader::upload_uniform_float_4(std::string name,
                                          const glm::vec4 &value) const
{
	GLint location = get_uniform_location(name);
	glUniform4f(location, value.x, value.y, value.z, value.w);
}

void OpenGLShader::upload_uniform_matrix_4(std::string name,
                                           const glm::mat4 &value) const
{
	GLint location = get_uniform_location(name);
	glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(value));
}

void OpenGLShader::set_int(std::string name, const int32_t value) const
{
	upload_uniform_int(name, value);
}

void OpenGLShader::set_int_array(const std::string &name, int32_t *value,
                                 size_t count) const
{
	upload_uniform_int_array(name, value, count);
}

void OpenGLShader::set_unsigned_int(std::string name,
                                    const uint32_t value) const
{
	upload_uniform_unsigned_int(name, value);
}

void OpenGLShader::set_float(std::string name, const float_t value) const
{
	upload_uniform_float(name, value);
}

void OpenGLShader::set_float_2(std::string name, const glm::vec2 value) const
{
	upload_uniform_float_2(name, value);
}

void OpenGLShader::set_float_3(std::string name, const glm::vec3 &value) const
{
	upload_uniform_float_3(name, value);
}

void OpenGLShader::set_float_4(std::string name, const glm::vec4 &value) const
{
	upload_uniform_float_4(name, value);
}

void OpenGLShader::set_matrix_4(std::string name, const glm::mat4 &value) const
{
	upload_uniform_matrix_4(name, value);
}

int OpenGLShader::get_uniform_location(const std::string &name) const
{
	if (locations.find(name) != locations.end())
		return locations[name];

	GLint location = glGetUniformLocation(id, name.c_str());
	SOL_CORE_ASSERT(location != -1, "could not find uniform location: {}",
	                name);
	locations[name] = location;
	return location;
}

} // namespace sol