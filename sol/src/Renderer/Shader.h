#pragma once

#include "Core.h"

#include <glm/glm.hpp>

namespace sol
{

class Shader
{
private:
	uint32_t id;

public:
	Shader(const std::string &vertex_source,
	       const std::string &fragment_source);
	~Shader();

	void bind() const;
	void unbind() const;

	void upload_uniform_mat4(std::string name, const glm::mat4 &matrix) const;
};
} // namespace sol