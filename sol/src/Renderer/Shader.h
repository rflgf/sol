#pragma once

#include <glm/glm.hpp>
#include <string>

namespace sol
{

class Shader
{
public:
	virtual ~Shader() = default;

	static Shader *create(const std::string &vertex_source,
	                      const std::string &fragment_source);

	virtual void bind() const   = 0;
	virtual void unbind() const = 0;

	virtual void upload_uniform_mat4(std::string name,
	                                 const glm::mat4 &matrix) const = 0;
};
} // namespace sol