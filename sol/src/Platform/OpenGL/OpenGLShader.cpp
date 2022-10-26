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

void OpenGLShader::upload_uniform_mat4(std::string name,
                                       const glm::mat4 &matrix) const
{
	GLint location = glGetUniformLocation(id, name.c_str());
	glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(matrix));
}

} // namespace sol