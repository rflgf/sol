#pragma once

#include "Renderer/Shader.h"

#include <glm/glm.hpp>
#include <unordered_map>

namespace sol
{

class OpenGLShader : public Shader
{
private:
	uint32_t id;

	// FIXME(rafael): this, alongside get_uniform_location(string), is a dirty
	// little trick to get uniform locations faster. optimally, all uniform
	// locations would be calculated at shader compilation time.
	mutable std::unordered_map<std::string, int> locations;

public:
	OpenGLShader(const std::string &vertex_source,
	             const std::string &fragment_source);
	~OpenGLShader();

	void bind() const override;
	void unbind() const override;

	void upload_uniform_mat4(std::string name,
	                         const glm::mat4 &matrix) const override;

	// see comment in Shader::locations.
	int get_uniform_location(const std::string &name) const;
};

} // namespace sol