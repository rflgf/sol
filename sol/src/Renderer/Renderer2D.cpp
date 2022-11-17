#include "Renderer2D.h"

#include "Renderer.h"

#include <glm/gtc/type_ptr.hpp>
#include <imgui.h>

namespace sol
{

Renderer2D::Data *Renderer2D::data = nullptr;

void Renderer2D::init()
{
	data = new Renderer2D::Data();

	data->vao = std::unique_ptr<VertexArray>(VertexArray::create());

	constexpr float vertices[4 * (3 + 2)] = {
	    -0.5f, -0.5f, 0.0f, 0.0f, 1.0f, // top-left
	    0.5f,  -0.5f, 0.0f, 1.0f, 1.0f, // top-right
	    -0.5f, 0.5f,  0.0f, 0.0f, 0.0f, // bottom-left
	    0.5f,  0.5f,  0.0f, 1.0f, 0.0f, // bottom-right
	};

	VertexBuffer *vbo = VertexBuffer::create(vertices, sizeof(vertices));

	using ElementType = BufferElement::Type;
	BufferLayout layout({{ElementType::FLOAT_3, "position"},
	                     {ElementType::FLOAT_2, "texture_coordinates"}});
	vbo->set_layout(layout);

	uint32_t indices[6] = {0, 1, 2, 2, 1, 3};
	IndexBuffer *ibo =
	    IndexBuffer::create(indices, sizeof(indices) / sizeof(uint32_t));

	data->vao->add_vertex_buffer(std::shared_ptr<VertexBuffer>(vbo));
	data->vao->set_index_buffer(std::shared_ptr<IndexBuffer>(ibo));

	data->tinted_texture_shader = std::shared_ptr<const Shader>(
	    Shader::create("assets/shaders/tinted_texture.shader"));
	Shader::Library::add(data->tinted_texture_shader);
	data->tinted_texture_shader->bind();

	data->white_texture = Texture2D::create(1, 1);

	uint32_t white_texture_data = 0xff'ff'ff'ff;
	data->white_texture->set_data(&white_texture_data,
	                              sizeof(white_texture_data));
}

void Renderer2D::deinit() { delete data; }

void Renderer2D::begin_scene(const OrthographicCamera &camera)
{
	data->tinted_texture_shader->bind();
	data->tinted_texture_shader->set_matrix_4("view_projection",
	                                          camera.projection_view_matrix);
}

void Renderer2D::end_scene() {}

void Renderer2D::draw_quad(const glm::vec2 position, const glm::vec2 size,
                           const glm::vec4 color, const float rotation)
{
	draw_quad({position.x, position.y, 0.0f}, size, color, rotation);
}

void Renderer2D::draw_quad(const glm::vec3 position, const glm::vec2 size,
                           const glm::vec4 color, const float rotation)
{
	draw_quad(position, size, *data->white_texture, color, rotation);
}

void Renderer2D::draw_quad(const glm::vec2 position, const glm::vec2 size,
                           const Texture &texture, const float rotation)
{
	draw_quad({position.x, position.y, 0.0f}, size, texture, rotation);
}

void Renderer2D::draw_quad(const glm::vec3 position, const glm::vec2 size,
                           const Texture &texture, const float rotation)
{
	draw_quad(position, size, texture, {1.0f, 1.0f, 1.0f, 1.0f}, rotation);
}

void Renderer2D::draw_quad(const glm::vec2 position, const glm::vec2 size,
                           const Texture &texture, const glm::vec3 tint,
                           const float rotation)
{
	draw_quad({position.x, position.y, 0.0f}, size, texture, tint, rotation);
}

void Renderer2D::draw_quad(const glm::vec3 position, const glm::vec2 size,
                           const Texture &texture, const glm::vec3 tint,
                           const float rotation)
{
	draw_quad(position, size, texture, {tint.r, tint.g, tint.b, 1.0f},
	          rotation);
}

void Renderer2D::draw_quad(const glm::vec2 position, const glm::vec2 size,
                           const Texture &texture, const glm::vec4 tint,
                           const float rotation)
{
	draw_quad({position.x, position.y, 0.0f}, size, texture, tint, rotation);
}

void Renderer2D::draw_quad(const glm::vec3 position, const glm::vec2 size,
                           const Texture &texture, const glm::vec4 tint,
                           const float rotation)
{
	glm::mat4 transform =
	    glm::translate(glm::mat4(1.0f), position) *
	    glm::rotate(glm::mat4(1.0f), rotation, glm::vec3(0.0f, 0.0f, 1.0f)) *
	    glm::scale(glm::mat4(1.0f), {size.x, size.y, 1.0f});

	data->tinted_texture_shader->bind();

	data->tinted_texture_shader->set_matrix_4("transform", transform);
	data->tinted_texture_shader->set_float_4("tint", tint);
	texture.bind();

	data->vao->bind();
	RenderCommand::draw_indexed(*data->vao);
}

} // namespace sol