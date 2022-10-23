#include "Renderer.h"

#include "RendererAPI.h"

namespace sol
{

void Renderer::init() { RenderCommand::init(); }

void Renderer::deinit() {}

void Renderer::begin_scene() {}

void Renderer::end_scene() {}

void Renderer::submit(const Shader &shader, VertexArray &vao)
{
	shader.bind();
	vao.bind();
	RenderCommand::draw_indexed(vao);
}

} // namespace sol