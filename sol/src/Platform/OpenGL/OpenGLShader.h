#pragma once

#include "Renderer/Shader.h"

#include <glm/glm.hpp>

namespace sol
{

class OpenGLShader : public Shader
{
private:
	uint32_t id;

public:
	OpenGLShader(const std::string &vertex_source,
	             const std::string &fragment_source);
	~OpenGLShader();

	void bind() const override;
	void unbind() const override;

	void upload_uniform_mat4(std::string name,
	                         const glm::mat4 &matrix) const override;
};

} // namespace sol