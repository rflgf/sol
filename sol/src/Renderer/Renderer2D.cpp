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

	data->vbo =
	    VertexBuffer::create(Data::MAX_VERTICES_PER_BATCH * sizeof(QuadVertex));

	using ElementType = BufferElement::Type;
	BufferLayout layout({{ElementType::FLOAT_3, "position"},
	                     {ElementType::FLOAT_2, "texture_coordinates"},
	                     {ElementType::FLOAT_4, "color"},
	                     {ElementType::FLOAT, "tiling_factor"},
	                     {ElementType::FLOAT, "texture_slot"}});
	data->vbo->set_layout(layout);
	data->vao->add_vertex_buffer(std::shared_ptr<VertexBuffer>(data->vbo));

	uint32_t *indices = new uint32_t[Data::MAX_INDICES_PER_BATCH];
	size_t offset     = 0;
	for (size_t i = 0; i < Data::MAX_INDICES_PER_BATCH;
	     i += Data::INDICES_PER_QUAD)
	{
		// upper triangle.
		indices[i + 0] = offset + 0;
		indices[i + 1] = offset + 1;
		indices[i + 2] = offset + 2;

		// bottom triangle.
		indices[i + 3] = offset + 2;
		indices[i + 4] = offset + 3;
		indices[i + 5] = offset + 0;

		offset += Data::VERTICES_PER_QUAD;
	}
	data->ibo = IndexBuffer::create(indices, Data::MAX_INDICES_PER_BATCH);
	// NOTE(rafael): if this is ever changed to use a command-queue approach,
	// change indices to an std::shared_ptr instead.
	data->vao->set_index_buffer(std::shared_ptr<IndexBuffer>(data->ibo));
	delete[] indices;

	data->base             = new QuadVertex[Data::MAX_VERTICES_PER_BATCH];
	data->quad_index_count = 0;

	data->tinted_texture_shader = std::shared_ptr<const Shader>(
	    Shader::create("assets/shaders/tinted_texture.shader"));
	Shader::Library::add(data->tinted_texture_shader);
	data->tinted_texture_shader->bind();

	data->white_texture = Texture2D::create(1, 1);

	uint32_t white_texture_data = 0xff'ff'ff'ff;
	data->white_texture->set_data(&white_texture_data,
	                              sizeof(white_texture_data));
	data->textures[0] = data->white_texture;

	std::array<int32_t, Data::MAX_TEXTURE_SLOTS> samplers;
	for (size_t i = 0; i < Data::MAX_TEXTURE_SLOTS; ++i)
		samplers[i] = i;
	data->tinted_texture_shader->set_int_array("textures", samplers.data(),
	                                           Data::MAX_TEXTURE_SLOTS);
}

void Renderer2D::deinit() { delete data; }

void Renderer2D::begin_scene(const OrthographicCamera &camera)
{
	data->tinted_texture_shader->bind();
	data->tinted_texture_shader->set_matrix_4("view_projection",
	                                          camera.projection_view_matrix);

	start_batch();
}

void Renderer2D::end_scene() { flush_batch(); }

void Renderer2D::start_batch()
{
	data->quad_index_count   = 0;
	data->offset_from_base   = data->base;
	data->texture_slot_index = 1;
}

void Renderer2D::flush_batch()
{
	size_t data_size = reinterpret_cast<uint8_t *>(data->offset_from_base) -
	                   reinterpret_cast<uint8_t *>(data->base);
	data->vbo->set_data(data->base, data_size);

	data->vao->bind();
	for (size_t i = 0; i < data->texture_slot_index; ++i)
		data->textures[i].get()->bind(i);

	RenderCommand::draw_indexed(*data->vao, data->quad_index_count);

	++data->statistics.draw_calls;
}

void Renderer2D::draw_quad(const glm::mat4 transform, const glm::vec4 color,
                           const float tiling_factor)
{
	draw_quad(transform, data->white_texture, color, tiling_factor);
}

void Renderer2D::draw_quad(const glm::mat4 transform,
                           std::shared_ptr<const Texture> texture,
                           const float tiling_factor)
{
	draw_quad(transform, texture, {1, 1, 1, 1}, tiling_factor);
}

void Renderer2D::draw_quad(const glm::mat4 transform,
                           std::shared_ptr<const Texture> texture,
                           const glm::vec3 tint, const float tiling_factor)
{
	draw_quad(transform, texture, {tint.r, tint.g, tint.b, 1}, tiling_factor);
}

void Renderer2D::draw_quad(const glm::mat4 transform,
                           std::shared_ptr<const Texture> texture,
                           const glm::vec4 tint, const float tiling_factor)
{
	if (data->texture_slot_index >= Data::MAX_TEXTURE_SLOTS ||
	    data->quad_index_count >= Data::MAX_INDICES_PER_BATCH)
	{
		flush_batch();
		start_batch();
	}

	size_t index = data->texture_slot_index;
	for (size_t i = 0; i < data->texture_slot_index; ++i)
		if (*data->textures[i].get() == *texture.get())
		{
			index = i;
			break;
		}

	if (index == data->texture_slot_index)
		++data->texture_slot_index;

	if (index != 0)
		data->textures[index] = texture;

	static const std::array<const glm::vec4, Data::VERTICES_PER_QUAD> vertices {
	    glm::vec4 {-0.5f, 0.5f, 0.0f, 1.0f},  // bottom left
	    glm::vec4 {0.5f, 0.5f, 0.0f, 1.0f},   // bottom right
	    glm::vec4 {0.5f, -0.5f, 0.0f, 1.0f},  // top right
	    glm::vec4 {-0.5f, -0.5f, 0.0f, 1.0f}, // top left
	};

	// bottom left vertex.
	data->offset_from_base->position            = transform * vertices[0];
	data->offset_from_base->texture_coordinates = {0.0f, 0.0f};
	data->offset_from_base->color               = tint;
	data->offset_from_base->tiling_factor       = tiling_factor;
	data->offset_from_base->texture_slot        = index;
	++data->offset_from_base;

	// bottom right vertex.
	data->offset_from_base->position            = transform * vertices[1];
	data->offset_from_base->texture_coordinates = {1.0f, 0.0f};
	data->offset_from_base->color               = tint;
	data->offset_from_base->tiling_factor       = tiling_factor;
	data->offset_from_base->texture_slot        = index;
	++data->offset_from_base;

	// top right vertex.
	data->offset_from_base->position            = transform * vertices[2];
	data->offset_from_base->texture_coordinates = {1.0f, 1.0f};
	data->offset_from_base->color               = tint;
	data->offset_from_base->tiling_factor       = tiling_factor;
	data->offset_from_base->texture_slot        = index;
	++data->offset_from_base;

	// top left vertex.
	data->offset_from_base->position            = transform * vertices[3];
	data->offset_from_base->texture_coordinates = {0.0f, 1.0f};
	data->offset_from_base->color               = tint;
	data->offset_from_base->tiling_factor       = tiling_factor;
	data->offset_from_base->texture_slot        = index;
	++data->offset_from_base;

	data->quad_index_count += Data::INDICES_PER_QUAD;

	++data->statistics.quad_count;

	// glm::mat4 transform =
	//     glm::translate(glm::mat4(1.0f), position) *
	//     glm::rotate(glm::mat4(1.0f), rotation, glm::vec3(0.0f, 0.0f, 1.0f)) *
	//     glm::scale(glm::mat4(1.0f), {size.x, size.y, 1.0f});

	// data->tinted_texture_shader->bind();

	// data->tinted_texture_shader->set_matrix_4("transform", transform);
	// data->tinted_texture_shader->set_float_4("tint", tint);
	// texture.bind();

	// data->vao->bind();
	// RenderCommand::draw_indexed(*data->vao);
}

void Renderer2D::draw_quad(const glm::vec3 position, const glm::vec2 size,
                           std::shared_ptr<const Texture> texture,
                           const glm::vec4 tint, const float rotation,
                           const float tiling_factor)
{

	glm::mat4 transform =
	    glm::translate(glm::mat4(1.0f), position) *
	    glm::rotate(glm::mat4(1.0f), rotation, glm::vec3(0.0f, 0.0f, 1.0f)) *
	    glm::scale(glm::mat4(1.0f), {size.x, size.y, 1.0f});
	draw_quad(transform, texture, tint, tiling_factor);
}

void Renderer2D::draw_quad(const glm::vec2 position, const glm::vec2 size,
                           const glm::vec4 color, const float rotation,
                           const float tiling_factor)
{
	draw_quad({position.x, position.y, 0.0f}, size, color, rotation,
	          tiling_factor);
}

void Renderer2D::draw_quad(const glm::vec3 position, const glm::vec2 size,
                           const glm::vec4 color, const float rotation,
                           const float tiling_factor)
{
	draw_quad(position, size, data->white_texture, color, rotation,
	          tiling_factor);
}

void Renderer2D::draw_quad(const glm::vec2 position, const glm::vec2 size,
                           std::shared_ptr<const Texture> texture,
                           const float rotation, const float tiling_factor)
{
	draw_quad({position.x, position.y, 0.0f}, size, texture, rotation,
	          tiling_factor);
}

void Renderer2D::draw_quad(const glm::vec3 position, const glm::vec2 size,
                           std::shared_ptr<const Texture> texture,
                           const float rotation, const float tiling_factor)
{
	draw_quad(position, size, texture, {1.0f, 1.0f, 1.0f, 1.0f}, rotation,
	          tiling_factor);
}

void Renderer2D::draw_quad(const glm::vec2 position, const glm::vec2 size,
                           std::shared_ptr<const Texture> texture,
                           const glm::vec3 tint, const float rotation,
                           const float tiling_factor)
{
	draw_quad({position.x, position.y, 0.0f}, size, texture, tint, rotation,
	          tiling_factor);
}

void Renderer2D::draw_quad(const glm::vec3 position, const glm::vec2 size,
                           std::shared_ptr<const Texture> texture,
                           const glm::vec3 tint, const float rotation,
                           const float tiling_factor)
{
	draw_quad(position, size, texture, {tint.r, tint.g, tint.b, 1.0f}, rotation,
	          tiling_factor);
}

void Renderer2D::draw_quad(const glm::vec2 position, const glm::vec2 size,
                           std::shared_ptr<const Texture> texture,
                           const glm::vec4 tint, const float rotation,
                           const float tiling_factor)
{
	draw_quad({position.x, position.y, 0.0f}, size, texture, tint, rotation,
	          tiling_factor);
}

void Renderer2D::draw_quad(const glm::vec2 position, const glm::vec2 size,
                           std::shared_ptr<const Subtexture2D> subtexture,
                           const float rotation, const float tiling_factor)
{
	draw_quad({position.x, position.y, 0.0f}, size, subtexture, rotation,
	          tiling_factor);
}

void Renderer2D::draw_quad(const glm::vec3 position, const glm::vec2 size,
                           std::shared_ptr<const Subtexture2D> subtexture,
                           const float rotation, const float tiling_factor)
{
	draw_quad(position, size, subtexture, {1.0f, 1.0f, 1.0f, 1.0f}, rotation,
	          tiling_factor);
}

void Renderer2D::draw_quad(const glm::vec2 position, const glm::vec2 size,
                           std::shared_ptr<const Subtexture2D> subtexture,
                           const glm::vec3 tint, const float rotation,
                           const float tiling_factor)
{
	draw_quad({position.x, position.y, 0.0f}, size, subtexture, tint, rotation,
	          tiling_factor);
}

void Renderer2D::draw_quad(const glm::vec3 position, const glm::vec2 size,
                           std::shared_ptr<const Subtexture2D> subtexture,
                           const glm::vec3 tint, const float rotation,
                           const float tiling_factor)
{
	draw_quad(position, size, subtexture, {tint.r, tint.g, tint.b, 1.0f},
	          rotation, tiling_factor);
}

void Renderer2D::draw_quad(const glm::vec2 position, const glm::vec2 size,
                           std::shared_ptr<const Subtexture2D> subtexture,
                           const glm::vec4 tint, const float rotation,
                           const float tiling_factor)
{
	draw_quad({position.x, position.y, 0.0f}, size, subtexture, tint, rotation,
	          tiling_factor);
}

void Renderer2D::draw_quad(const glm::mat4 transform,
                           std::shared_ptr<const Subtexture2D> subtexture,
                           const float tiling_factor)
{
	draw_quad(transform, subtexture, {1, 1, 1, 1}, tiling_factor);
}

void Renderer2D::draw_quad(const glm::vec3 position, const glm::vec2 size,
                           std::shared_ptr<const Subtexture2D> subtexture,
                           const glm::vec4 tint, const float rotation,
                           const float tiling_factor)
{

	glm::mat4 transform =
	    glm::translate(glm::mat4(1.0f), position) *
	    glm::rotate(glm::mat4(1.0f), rotation, glm::vec3(0.0f, 0.0f, 1.0f)) *
	    glm::scale(glm::mat4(1.0f), {size.x, size.y, 1.0f});
	draw_quad(transform, subtexture, tint, tiling_factor);
}

void Renderer2D::draw_quad(const glm::mat4 transform,
                           std::shared_ptr<const Subtexture2D> texture,
                           const glm::vec3 tint, const float tiling_factor)
{
	draw_quad(transform, texture, {tint.r, tint.g, tint.b, 1}, tiling_factor);
}

void Renderer2D::draw_quad(const glm::mat4 transform,
                           std::shared_ptr<const Subtexture2D> subtexture,
                           const glm::vec4 tint, const float tiling_factor)
{
	if (data->texture_slot_index >= Data::MAX_TEXTURE_SLOTS ||
	    data->quad_index_count >= Data::MAX_INDICES_PER_BATCH)
	{
		flush_batch();
		start_batch();
	}

	size_t index = data->texture_slot_index;
	for (size_t i = 0; i < data->texture_slot_index; ++i)
		if (*data->textures[i].get() == *subtexture.get()->get_atlas().get())
		{
			index = i;
			break;
		}

	if (index == data->texture_slot_index)
		++data->texture_slot_index;

	if (index != 0)
		data->textures[index] = subtexture.get()->get_atlas();

	static const std::array<const glm::vec4, Data::VERTICES_PER_QUAD> vertices {
	    glm::vec4 {-0.5f, 0.5f, 0.0f, 1.0f},  // bottom left
	    glm::vec4 {0.5f, 0.5f, 0.0f, 1.0f},   // bottom right
	    glm::vec4 {0.5f, -0.5f, 0.0f, 1.0f},  // top right
	    glm::vec4 {-0.5f, -0.5f, 0.0f, 1.0f}, // top left
	};

	// bottom left vertex.
	data->offset_from_base->position = transform * vertices[0];
	data->offset_from_base->texture_coordinates =
	    subtexture.get()->get_texture_coordinates()[0];
	data->offset_from_base->color         = tint;
	data->offset_from_base->tiling_factor = tiling_factor;
	data->offset_from_base->texture_slot  = index;
	++data->offset_from_base;

	// bottom right vertex.
	data->offset_from_base->position = transform * vertices[1];
	data->offset_from_base->texture_coordinates =
	    subtexture.get()->get_texture_coordinates()[1];
	data->offset_from_base->color         = tint;
	data->offset_from_base->tiling_factor = tiling_factor;
	data->offset_from_base->texture_slot  = index;
	++data->offset_from_base;

	// top right vertex.
	data->offset_from_base->position = transform * vertices[2];
	data->offset_from_base->texture_coordinates =
	    subtexture.get()->get_texture_coordinates()[2];
	data->offset_from_base->color         = tint;
	data->offset_from_base->tiling_factor = tiling_factor;
	data->offset_from_base->texture_slot  = index;
	++data->offset_from_base;

	// top left vertex.
	data->offset_from_base->position = transform * vertices[3];
	data->offset_from_base->texture_coordinates =
	    subtexture.get()->get_texture_coordinates()[3];
	data->offset_from_base->color         = tint;
	data->offset_from_base->tiling_factor = tiling_factor;
	data->offset_from_base->texture_slot  = index;
	++data->offset_from_base;

	data->quad_index_count += Data::INDICES_PER_QUAD;

	++data->statistics.quad_count;

	// glm::mat4 transform =
	//     glm::translate(glm::mat4(1.0f), position) *
	//     glm::rotate(glm::mat4(1.0f), rotation, glm::vec3(0.0f, 0.0f, 1.0f)) *
	//     glm::scale(glm::mat4(1.0f), {size.x, size.y, 1.0f});

	// data->tinted_texture_shader->bind();

	// data->tinted_texture_shader->set_matrix_4("transform", transform);
	// data->tinted_texture_shader->set_float_4("tint", tint);
	// texture.bind();

	// data->vao->bind();
	// RenderCommand::draw_indexed(*data->vao);
}

uint32_t Renderer2D::Statistics::total_vertex_count() const
{
	return quad_count * Renderer2D::Data::VERTICES_PER_QUAD;
}

uint32_t Renderer2D::Statistics::total_index_count() const
{
	return quad_count * Renderer2D::Data::INDICES_PER_QUAD;
}

void Renderer2D::Statistics::reset()
{
	draw_calls = 0;
	quad_count = 0;
}

} // namespace sol
