#include "Renderer.h"

#include "RendererAPI.h"

namespace sol
{

Renderer::SceneData *Renderer::data = new Renderer::SceneData;

void Renderer::init() { RenderCommand::init(); }

void Renderer::deinit() {}

void Renderer::begin_scene(OrthographicCamera &camera)
{
	data->view_projection_matrix = camera.projection_view_matrix;
}

void Renderer::end_scene() {}

void Renderer::submit(const Shader &shader, VertexArray &vao, const glm::mat4 &transform)
{
	shader.bind();
	
	shader.upload_uniform_mat4("view_projection",
	                           Renderer::data->view_projection_matrix);
	
	shader.upload_uniform_mat4("transform", transform);

	vao.bind();
	RenderCommand::draw_indexed(vao);
}

} // namespace sol