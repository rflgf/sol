#include "Renderer.h"

#include "Renderer2D.h"
#include "RendererAPI.h"
#include "Shader.h"

namespace sol
{

Renderer::SceneData *Renderer::data = new Renderer::SceneData;

void Renderer::init()
{
	Shader::Library::init();
	RenderCommand::init();
	Renderer2D::init();
}

void Renderer::deinit() { Renderer2D::deinit(); }

void Renderer::begin_scene(const OrthographicCamera &camera)
{
	data->view_projection_matrix = camera.get_projection_view_matrix();
}

void Renderer::end_scene() {}

void Renderer::submit(const Shader &shader, VertexArray &vao,
                      const glm::mat4 &transform)
{
	shader.bind();

	shader.set_matrix_4("view_projection",
	                    Renderer::data->view_projection_matrix);

	shader.set_matrix_4("transform", transform);

	vao.bind();
	RenderCommand::draw_indexed(vao);
}

} // namespace sol