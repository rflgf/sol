#pragma once

#include "Core.h"

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

	void bind();
	void unbind();
};
} // namespace sol