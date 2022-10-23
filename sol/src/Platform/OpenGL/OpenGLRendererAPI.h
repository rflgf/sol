#pragma once

#include "Renderer/RendererAPI.h"

namespace sol
{

class OpenGLRendererAPI : public RendererAPI
{
	void init() override;
	void set_viewport(uint32_t x, uint32_t y, uint32_t width,
	                  uint32_t height) override;
	void draw_indexed(VertexArray &vao) override;
	void set_clear_color(const glm::vec4 &color) override;
	void clear() const override;
};

} // namespace sol