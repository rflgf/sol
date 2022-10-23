#include "RenderCommand.h"

namespace sol
{

RendererAPI *RenderCommand::api = RendererAPI::create();

void RenderCommand::init() { api->init(); }

void RenderCommand::set_viewport(uint32_t x, uint32_t y, uint32_t width,
                                 uint32_t height)
{
	api->set_viewport(x, y, width, height);
}

void RenderCommand::draw_indexed(VertexArray &vao) { api->draw_indexed(vao); }

void RenderCommand::set_clear_color(const glm::vec4 &color)
{
	api->set_clear_color(color);
}

void RenderCommand::clear() { api->clear(); }

} // namespace sol