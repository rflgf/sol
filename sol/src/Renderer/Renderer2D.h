#pragma once

#include "Buffers.h"
#include "OrthographicCamera.h"
#include "Shader.h"
#include "Texture.h"

#include <glm/glm.hpp>
#include <memory>

namespace sol
{

class Renderer2D
{
public:
	struct QuadVertex
	{
		glm::vec3 position;
		glm::vec2 texture_coordinates;
		glm::vec4 color;
		float tiling_factor;
		float texture_slot;
	};

	struct Data
	{
		static const size_t MAX_QUADS_PER_BATCH    = 10000;
		static const size_t MAX_VERTICES_PER_BATCH = MAX_QUADS_PER_BATCH * 4;
		static const size_t MAX_INDICES_PER_BATCH  = MAX_QUADS_PER_BATCH * 6;
		static const size_t MAX_TEXTURE_SLOTS      = 16;

		size_t quad_index_count      = 0;
		QuadVertex *base             = nullptr;
		QuadVertex *offset_from_base = nullptr;

		size_t texture_slot_index = 1; // index 0 is for white texture.
		std::array<std::shared_ptr<const Texture>, MAX_TEXTURE_SLOTS> textures;

		std::shared_ptr<VertexArray> vao;
		std::shared_ptr<VertexBuffer> vbo;
		std::shared_ptr<IndexBuffer> ibo;
		std::shared_ptr<const Shader> tinted_texture_shader;
		std::shared_ptr<const Texture> white_texture;
	};

	static Data *data;

	static void init();
	static void deinit();

	static void begin_scene(const OrthographicCamera &camera);
	static void end_scene();
	static void flush();

	static void draw_quad(const glm::vec2 position, const glm::vec2 size,
	                      const glm::vec4 color, const float rotation = 0.0f,
	                      const float tiling_factor = 1.0f);
	static void draw_quad(const glm::vec3 position, const glm::vec2 size,
	                      const glm::vec4 color, const float rotation = 0.0f,
	                      const float tiling_factor = 1.0f);
	static void draw_quad(const glm::vec2 position, const glm::vec2 size,
	                      std::shared_ptr<const Texture> texture,
	                      const float rotation      = 0.0f,
	                      const float tiling_factor = 1.0f);
	static void draw_quad(const glm::vec3 position, const glm::vec2 size,
	                      std::shared_ptr<const Texture> texture,
	                      const float rotation      = 0.0f,
	                      const float tiling_factor = 1.0f);
	static void draw_quad(const glm::vec2 position, const glm::vec2 size,
	                      std::shared_ptr<const Texture> texture,
	                      const glm::vec3 tint, const float rotation = 0.0f,
	                      const float tiling_factor = 1.0f);
	static void draw_quad(const glm::vec3 position, const glm::vec2 size,
	                      std::shared_ptr<const Texture> texture,
	                      const glm::vec3 tint, const float rotation = 0.0f,
	                      const float tiling_factor = 1.0f);
	static void draw_quad(const glm::vec2 position, const glm::vec2 size,
	                      std::shared_ptr<const Texture> texture,
	                      const glm::vec4 tint, const float rotation = 0.0f,
	                      const float tiling_factor = 1.0f);
	static void draw_quad(const glm::vec3 position, const glm::vec2 size,
	                      std::shared_ptr<const Texture> texture,
	                      const glm::vec4 tint, const float rotation = 0.0f,
	                      const float tiling_factor = 1.0f);
};

} // namespace sol