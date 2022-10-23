#pragma once

#include "RendererAPI.h"

namespace sol
{

class RenderCommand
{
public:
	static void init();
	static void set_viewport(uint32_t x, uint32_t y, uint32_t width,
	                         uint32_t height);
	static void draw_indexed(VertexArray &vao);
	static void set_clear_color(const glm::vec4 &color);
	static void clear();

private:
	static RendererAPI *api;
};

} // namespace sol