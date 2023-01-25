#include "Renderer2D.h"

#include "Renderer.h"
#include "Scene/Components.h"

#include <glm/gtc/type_ptr.hpp>
#include <imgui.h>

namespace sol
{

Renderer2D::Data Renderer2D::data;

void Renderer2D::init()
{
	data.vao = std::unique_ptr<VertexArray>(VertexArray::create());

	data.vbo =
	    VertexBuffer::create(Data::MAX_VERTICES_PER_BATCH * sizeof(QuadVertex));

	using ElementType = BufferElement::Type;
	BufferLayout layout({{ElementType::FLOAT_3, "position"},
	                     {ElementType::FLOAT_2, "texture_coordinates"},
	                     {ElementType::FLOAT_4, "color"},
	                     {ElementType::FLOAT, "tiling_factor"},
	                     {ElementType::FLOAT, "texture_slot"}});
	data.vbo->set_layout(layout);
	data.vao->add_vertex_buffer(std::shared_ptr<VertexBuffer>(data.vbo));

	uint32_t *indices = new uint32_t[Data::MAX_INDICES_PER_BATCH];
	size_t offset     = 0;
	for (size_t i = 0; i < Data::MAX_INDICES_PER_BATCH;
	     i += Data::INDICES_PER_QUAD)
	{
		enum VERTEX_INDICES
		{
			BOTTOM_LEFT  = 0, // 1___2
			TOP_LEFT     = 1, // |  /|
			TOP_RIGHT    = 2, // | / |
			BOTTOM_RIGHT = 3  // 0/__3
		};

		// upper triangle.
		indices[i + 0] = offset + BOTTOM_LEFT;
		indices[i + 1] = offset + TOP_LEFT;
		indices[i + 2] = offset + TOP_RIGHT;

		// bottom triangle.
		indices[i + 3] = offset + TOP_RIGHT;
		indices[i + 4] = offset + BOTTOM_RIGHT;
		indices[i + 5] = offset + BOTTOM_LEFT;

		offset += Data::VERTICES_PER_QUAD;
	}
	data.ibo = IndexBuffer::create(indices, Data::MAX_INDICES_PER_BATCH);
	// NOTE(rafael): if this is ever changed to use a command-queue approach,
	// change indices to an std::shared_ptr instead.
	data.vao->set_index_buffer(std::shared_ptr<IndexBuffer>(data.ibo));
	delete[] indices;

	data.base             = new QuadVertex[Data::MAX_VERTICES_PER_BATCH];
	data.quad_index_count = 0;

	data.tinted_texture_shader = std::shared_ptr<const Shader>(
	    Shader::create("assets/shaders/tinted_texture.shader"));
	Shader::Library::add(data.tinted_texture_shader);
	data.tinted_texture_shader->bind();

	data.white_texture = Texture2D::create(1, 1);

	uint32_t white_texture_data = 0xff'ff'ff'ff;
	data.white_texture->set_data(&white_texture_data,
	                             sizeof(white_texture_data));

	std::array<int32_t, Data::MAX_TEXTURE_SLOTS> samplers;
	for (size_t i = 0; i < Data::MAX_TEXTURE_SLOTS; ++i)
		samplers[i] = i;
	data.tinted_texture_shader->set_int_array("textures", samplers.data(),
	                                          Data::MAX_TEXTURE_SLOTS);
}

void Renderer2D::deinit() {}

void Renderer2D::begin_scene(const OrthographicCamera &camera)
{
	data.tinted_texture_shader->bind();
	data.tinted_texture_shader->set_matrix_4("view_projection",
	                                         camera.projection_view_matrix);

	start_batch();
}

void Renderer2D::begin_scene(const Camera &camera, glm::mat4 &transform)
{
	data.tinted_texture_shader->bind();
	glm::mat4 projection_view_matrix =
	    camera.projection * glm::inverse(transform);
	data.tinted_texture_shader->set_matrix_4("view_projection",
	                                         projection_view_matrix);

	start_batch();
}

void Renderer2D::end_scene() { flush_batch(); }

void Renderer2D::start_batch()
{
	data.quad_index_count   = 0;
	data.offset_from_base   = data.base;
	data.texture_slot_index = 0;
}

void Renderer2D::flush_batch()
{
	size_t data_size = reinterpret_cast<uint8_t *>(data.offset_from_base) -
	                   reinterpret_cast<uint8_t *>(data.base);
	data.vbo->set_data(data.base, data_size);

	for (size_t i = 0; i < data.texture_slot_index; ++i)
		data.textures[i]->bind(i);

	RenderCommand::draw_indexed(*data.vao, data.quad_index_count);
	++data.statistics.draw_calls;
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

void Renderer2D::draw_quad(const glm::mat4 transform,
                           std::shared_ptr<Texture> texture,
                           const std::array<glm::vec2, 4> texture_coordinates,
                           const glm::vec4 tint, const float tiling_factor)
{
	if (data.quad_index_count >= Data::MAX_INDICES_PER_BATCH)
	{
		flush_batch();
		start_batch();
	}

	// set to an invalid value (not 0 since it is valid) so we can catch later
	// if this has not changed.
	size_t index = data.texture_slot_index;

	for (size_t i = 0; i < data.texture_slot_index; ++i)
		if (*data.textures[i].get() == *texture.get())
		{
			index = i;
			break;
		}

	// if the index is invalid, it's not in a texture slot so we need to
	// upload it to the GPU.
	if (index == data.texture_slot_index)
	{
		// if all of the slots are busy, we flush so we can push more
		// textures.
		if (data.texture_slot_index == Data::MAX_TEXTURE_SLOTS)
		{
			flush_batch();
			start_batch();
		}

		index                = data.texture_slot_index;
		data.textures[index] = texture;
		++data.texture_slot_index;
	}

	static const std::array<const glm::vec4, Data::VERTICES_PER_QUAD> vertices {
	    glm::vec4 {-0.5f, -0.5f, 0.0f, 1.0f}, // bottom left
	    glm::vec4 {-0.5f, 0.5f, 0.0f, 1.0f},  // top left
	    glm::vec4 {0.5f, 0.5f, 0.0f, 1.0f},   // top right
	    glm::vec4 {0.5f, -0.5f, 0.0f, 1.0f},  // bottom right
	};

	for (size_t vertex = 0; vertex < Data::VERTICES_PER_QUAD; ++vertex)
	{
		data.offset_from_base->position = transform * vertices[vertex];
		data.offset_from_base->texture_coordinates =
		    texture_coordinates[vertex];
		data.offset_from_base->color         = tint;
		data.offset_from_base->tiling_factor = tiling_factor;
		data.offset_from_base->texture_slot  = static_cast<float>(index);
		++data.offset_from_base;
	}

	data.quad_index_count += Data::INDICES_PER_QUAD;

	++data.statistics.quad_count;
}

void Renderer2D::draw_quad(const glm::mat4 transform, const glm::vec4 color,
                           const float tiling_factor)
{
	draw_quad(transform, data.white_texture, color, tiling_factor);
}

void Renderer2D::draw_quad(const glm::mat4 transform,
                           std::shared_ptr<Texture> texture,
                           const float tiling_factor)
{
	draw_quad(transform, texture, {1, 1, 1, 1}, tiling_factor);
}

void Renderer2D::draw_quad(const glm::mat4 transform,
                           std::shared_ptr<Texture> texture,
                           const glm::vec3 tint, const float tiling_factor)
{
	draw_quad(transform, texture, {tint.r, tint.g, tint.b, 1}, tiling_factor);
}

void Renderer2D::draw_quad(const glm::vec3 position, const glm::vec2 size,
                           std::shared_ptr<Texture> texture,
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
	draw_quad(position, size, data.white_texture, color, rotation,
	          tiling_factor);
}

void Renderer2D::draw_quad(const glm::vec2 position, const glm::vec2 size,
                           std::shared_ptr<Texture> texture,
                           const float rotation, const float tiling_factor)
{
	draw_quad({position.x, position.y, 0.0f}, size, texture, rotation,
	          tiling_factor);
}

void Renderer2D::draw_quad(const glm::vec3 position, const glm::vec2 size,
                           std::shared_ptr<Texture> texture,
                           const float rotation, const float tiling_factor)
{
	draw_quad(position, size, texture, {1.0f, 1.0f, 1.0f, 1.0f}, rotation,
	          tiling_factor);
}

void Renderer2D::draw_quad(const glm::vec2 position, const glm::vec2 size,
                           std::shared_ptr<Texture> texture,
                           const glm::vec3 tint, const float rotation,
                           const float tiling_factor)
{
	draw_quad({position.x, position.y, 0.0f}, size, texture, tint, rotation,
	          tiling_factor);
}

void Renderer2D::draw_quad(const glm::vec3 position, const glm::vec2 size,
                           std::shared_ptr<Texture> texture,
                           const glm::vec3 tint, const float rotation,
                           const float tiling_factor)
{
	draw_quad(position, size, texture, {tint.r, tint.g, tint.b, 1.0f}, rotation,
	          tiling_factor);
}

void Renderer2D::draw_quad(const glm::vec2 position, const glm::vec2 size,
                           std::shared_ptr<Texture> texture,
                           const glm::vec4 tint, const float rotation,
                           const float tiling_factor)
{
	draw_quad({position.x, position.y, 0.0f}, size, texture, tint, rotation,
	          tiling_factor);
}

void Renderer2D::draw_quad(const glm::vec2 position, const glm::vec2 size,
                           Subtexture2D subtexture, const float rotation,
                           const float tiling_factor)
{
	draw_quad({position.x, position.y, 0.0f}, size, subtexture, rotation,
	          tiling_factor);
}

void Renderer2D::draw_quad(const glm::vec3 position, const glm::vec2 size,
                           Subtexture2D subtexture, const float rotation,
                           const float tiling_factor)
{
	draw_quad(position, size, subtexture, {1.0f, 1.0f, 1.0f, 1.0f}, rotation,
	          tiling_factor);
}

void Renderer2D::draw_quad(const glm::vec2 position, const glm::vec2 size,
                           Subtexture2D subtexture, const glm::vec3 tint,
                           const float rotation, const float tiling_factor)
{
	draw_quad({position.x, position.y, 0.0f}, size, subtexture, tint, rotation,
	          tiling_factor);
}

void Renderer2D::draw_quad(const glm::vec3 position, const glm::vec2 size,
                           Subtexture2D subtexture, const glm::vec3 tint,
                           const float rotation, const float tiling_factor)
{
	draw_quad(position, size, subtexture, {tint.r, tint.g, tint.b, 1.0f},
	          rotation, tiling_factor);
}

void Renderer2D::draw_quad(const glm::vec2 position, const glm::vec2 size,
                           Subtexture2D subtexture, const glm::vec4 tint,
                           const float rotation, const float tiling_factor)
{
	draw_quad({position.x, position.y, 0.0f}, size, subtexture, tint, rotation,
	          tiling_factor);
}

void Renderer2D::draw_quad(const glm::mat4 transform, Subtexture2D subtexture,
                           const float tiling_factor)
{
	draw_quad(transform, subtexture, {1, 1, 1, 1}, tiling_factor);
}

void Renderer2D::draw_quad(const glm::vec3 position, const glm::vec2 size,
                           Subtexture2D subtexture, const glm::vec4 tint,
                           const float rotation, const float tiling_factor)
{

	glm::mat4 transform =
	    glm::translate(glm::mat4(1.0f), position) *
	    glm::rotate(glm::mat4(1.0f), rotation, glm::vec3(0.0f, 0.0f, 1.0f)) *
	    glm::scale(glm::mat4(1.0f), {size.x, size.y, 1.0f});
	draw_quad(transform, subtexture, tint, tiling_factor);
}

void Renderer2D::draw_quad(const glm::mat4 transform, Subtexture2D texture,
                           const glm::vec3 tint, const float tiling_factor)
{
	draw_quad(transform, texture, {tint.r, tint.g, tint.b, 1}, tiling_factor);
}

void Renderer2D::draw_quad(const glm::mat4 transform,
                           std::shared_ptr<Texture> texture,
                           const glm::vec4 tint, const float tiling_factor)
{
	const std::array<glm::vec2, 4> texture_coordinates {
	    glm::vec2 {0.0f, 0.0f},
	    glm::vec2 {0.0f, 1.0f},
	    glm::vec2 {1.0f, 1.0f},
	    glm::vec2 {1.0f, 0.0f},
	};

	draw_quad(transform, texture, texture_coordinates, tint, tiling_factor);
}

void Renderer2D::draw_quad(const glm::mat4 transform, Subtexture2D subtexture,
                           const glm::vec4 tint, const float tiling_factor)
{
	draw_quad(transform, subtexture.get_atlas(),
	          subtexture.get_texture_coordinates(), tint, tiling_factor);
}

} // namespace sol
