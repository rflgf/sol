#include "OpenGLShader.h"

#include "Core.h"
#include "Platform/OpenGL/OpenGLContext.h"

#include <glm/gtc/type_ptr.hpp>

namespace sol
{

OpenGLShader::OpenGLShader(const std::string &vertex_source,
                           const std::string &fragment_source)
{
	GLuint vertex_shader = glCreateShader(GL_VERTEX_SHADER);

	const GLchar *source = static_cast<const GLchar *>(vertex_source.c_str());
	glShaderSource(vertex_shader, 1, &source, 0);

	glCompileShader(vertex_shader);

	GLint compiled = 0;
	glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &compiled);
	if (compiled == GL_FALSE)
	{
		GLint max_length = 0;
		glGetShaderiv(vertex_shader, GL_INFO_LOG_LENGTH, &max_length);

		std::vector<GLchar> error(max_length);
		glGetShaderInfoLog(vertex_shader, max_length, &max_length, &error[0]);

		glDeleteShader(vertex_shader);

		SOL_CORE_ERROR("{}", error);
		SOL_CORE_ASSERT(false, "vertex shader compilation error");
	}

	GLuint fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);

	source = static_cast<const GLchar *>(fragment_source.c_str());
	glShaderSource(fragment_shader, 1, &source, 0);

	glCompileShader(fragment_shader);

	glGetShaderiv(fragment_shader, GL_COMPILE_STATUS, &compiled);
	if (compiled == GL_FALSE)
	{
		GLint max_length = 0;
		glGetShaderiv(fragment_shader, GL_INFO_LOG_LENGTH, &max_length);

		std::vector<GLchar> error(max_length);
		glGetShaderInfoLog(fragment_shader, max_length, &max_length, &error[0]);

		glDeleteShader(fragment_shader);
		glDeleteShader(vertex_shader);

		SOL_CORE_ERROR("{}", error);
		SOL_CORE_ASSERT(false, "fragment shader compilation error");
	}

	id = glCreateProgram();

	glAttachShader(id, vertex_shader);
	glAttachShader(id, fragment_shader);

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
		glDeleteShader(vertex_shader);
		glDeleteShader(fragment_shader);

		SOL_CORE_ERROR("{}", error);
		SOL_CORE_ASSERT(false, "shader linking error");
	}

	glDetachShader(id, vertex_shader);
	glDetachShader(id, fragment_shader);
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