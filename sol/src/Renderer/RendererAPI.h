#pragma once

#include "Buffers.h"

#include <glm/glm.hpp>
#include <memory>

namespace sol
{

class RendererAPI
{
public:
	enum class API
	{
		// clang-format off
		NONE    = 0,
		OPEN_GL = 1,
		// clang-format on
	};

	static API get_type() { return api_type; }

	virtual void init() = 0;

	virtual void set_viewport(uint32_t x, uint32_t y, uint32_t width,
	                          uint32_t height)           = 0;
	virtual void draw_indexed(VertexArray &vao)          = 0;
	virtual void set_clear_color(const glm::vec4 &color) = 0;
	virtual void clear() const                           = 0;

	static RendererAPI *create();

private:
	static API api_type;
};

} // namespace sol