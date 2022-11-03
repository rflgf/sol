#include "OpenGLShader.h"

#include "Core.h"
#include "Platform/OpenGL/OpenGLContext.h"

#include <glm/gtc/type_ptr.hpp>

namespace sol
{

OpenGLShader *OpenGLShader::compile(
    const std::unordered_map<Shader::Type, std::string> &sources)
{
	OpenGLShader *sol_shader = new OpenGLShader;
	std::vector<GLuint> ids;

	for (auto [type, source] : sources)
	{
		GLuint shader = glCreateShader(gl_type_from_sol_internal_type(type));

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

			delete sol_shader;

			SOL_CORE_ERROR("{}", error);
			SOL_CORE_ASSERT(false,
			                "shader compilation error on shader of type {}",
			                Shader::type_to_string(type));
		}

		ids.push_back(shader);
	}

	sol_shader->id = glCreateProgram();

	for (GLuint shader_id : ids)
		glAttachShader(sol_shader->id, shader_id);

	glLinkProgram(sol_shader->id);

	GLint linked = 0;
	glGetProgramiv(sol_shader->id, GL_LINK_STATUS, static_cast<int *>(&linked));
	if (linked == GL_FALSE)
	{
		GLint max_length = 0;
		glGetProgramiv(sol_shader->id, GL_INFO_LOG_LENGTH, &max_length);

		std::vector<GLchar> error(max_length);
		glGetProgramInfoLog(sol_shader->id, max_length, &max_length, &error[0]);

		glDeleteProgram(sol_shader->id);
		for (GLuint shader_id : ids)
			glDeleteShader(shader_id);

		delete sol_shader;

		SOL_CORE_ERROR("{}", error);
		SOL_CORE_ASSERT(false, "shader linking error");
	}

	for (GLuint shader_id : ids)
		glDetachShader(sol_shader->id, shader_id);

	return sol_shader;
}

GLenum OpenGLShader::gl_type_from_sol_internal_type(Shader::Type type)
{
	switch (type)
	{
		// clang-format off
		case Type::FRAGMENT:      return GL_FRAGMENT_SHADER;
		case Type::VERTEX:        return GL_VERTEX_SHADER;
		
		case Type::NONE: default: return 0;
		// clang-format on
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