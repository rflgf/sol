#pragma once

#include "Buffers.h"
#include "Camera.h"
#include "OrthographicCamera.h"
#include "Shader.h"
#include "Subtexture2D.h"
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

	struct Statistics
	{
		uint32_t draw_calls = 0;
		uint32_t quad_count = 0;

		uint32_t total_vertex_count() const;
		uint32_t total_index_count() const;
		void reset();
	};

	struct Data
	{
		static const size_t MAX_QUADS_PER_BATCH = 10000;
		static const size_t INDICES_PER_QUAD    = 6;
		static const size_t VERTICES_PER_QUAD   = 4;
		static const size_t MAX_VERTICES_PER_BATCH =
		    MAX_QUADS_PER_BATCH * VERTICES_PER_QUAD;
		static const size_t MAX_INDICES_PER_BATCH =
		    MAX_QUADS_PER_BATCH * INDICES_PER_QUAD;
		static const size_t MAX_TEXTURE_SLOTS = 16;

		size_t quad_index_count      = 0;
		QuadVertex *base             = nullptr;
		QuadVertex *offset_from_base = nullptr;

		size_t texture_slot_index = 0;
		std::array<std::shared_ptr<Texture>, MAX_TEXTURE_SLOTS> textures;

		std::shared_ptr<VertexArray> vao;
		std::shared_ptr<VertexBuffer> vbo;
		std::shared_ptr<IndexBuffer> ibo;
		std::shared_ptr<const Shader> tinted_texture_shader;
		std::shared_ptr<Texture> white_texture;

		Statistics statistics;
	};

	static Data data;

	static void init();
	static void deinit();

	static void begin_scene(const OrthographicCamera &camera);
	static void begin_scene(const Camera &camera, glm::mat4 &transform);
	static void end_scene();
	static void start_batch();
	static void flush_batch();

	static void draw_quad(const glm::mat4 transform, const glm::vec3 color,
	                      const float tiling_factor = 1.0f);
	static void draw_quad(const glm::mat4 transform, const glm::vec4 color,
	                      const float tiling_factor = 1.0f);
	static void draw_quad(const glm::mat4 transform,
	                      std::shared_ptr<Texture> texture,
	                      const float tiling_factor = 1.0f);
	static void draw_quad(const glm::mat4 transform,
	                      std::shared_ptr<Texture> texture,
	                      const glm::vec3 tint,
	                      const float tiling_factor = 1.0f);
	static void draw_quad(const glm::mat4 transform,
	                      std::shared_ptr<Texture> texture,
	                      const glm::vec4 tint,
	                      const float tiling_factor = 1.0f);

	static void draw_quad(const glm::vec2 position, const glm::vec2 size,
	                      const glm::vec4 color, const float rotation = 0.0f,
	                      const float tiling_factor = 1.0f);
	static void draw_quad(const glm::vec3 position, const glm::vec2 size,
	                      const glm::vec4 color, const float rotation = 0.0f,
	                      const float tiling_factor = 1.0f);
	static void draw_quad(const glm::vec2 position, const glm::vec2 size,
	                      std::shared_ptr<Texture> texture,
	                      const float rotation      = 0.0f,
	                      const float tiling_factor = 1.0f);
	static void draw_quad(const glm::vec3 position, const glm::vec2 size,
	                      std::shared_ptr<Texture> texture,
	                      const float rotation      = 0.0f,
	                      const float tiling_factor = 1.0f);
	static void draw_quad(const glm::vec2 position, const glm::vec2 size,
	                      std::shared_ptr<Texture> texture,
	                      const glm::vec3 tint, const float rotation = 0.0f,
	                      const float tiling_factor = 1.0f);
	static void draw_quad(const glm::vec3 position, const glm::vec2 size,
	                      std::shared_ptr<Texture> texture,
	                      const glm::vec3 tint, const float rotation = 0.0f,
	                      const float tiling_factor = 1.0f);
	static void draw_quad(const glm::vec2 position, const glm::vec2 size,
	                      std::shared_ptr<Texture> texture,
	                      const glm::vec4 tint, const float rotation = 0.0f,
	                      const float tiling_factor = 1.0f);
	static void draw_quad(const glm::vec3 position, const glm::vec2 size,
	                      std::shared_ptr<Texture> texture,
	                      const glm::vec4 tint, const float rotation = 0.0f,
	                      const float tiling_factor = 1.0f);

	static void draw_quad(const glm::mat4 transform,
	                      std::shared_ptr<Subtexture2D> texture,
	                      const float tiling_factor = 1.0f);
	static void draw_quad(const glm::mat4 transform,
	                      std::shared_ptr<Subtexture2D> texture,
	                      const glm::vec3 tint,
	                      const float tiling_factor = 1.0f);
	static void draw_quad(const glm::mat4 transform,
	                      std::shared_ptr<Subtexture2D> texture,
	                      const glm::vec4 tint,
	                      const float tiling_factor = 1.0f);

	static void draw_quad(const glm::vec2 position, const glm::vec2 size,
	                      std::shared_ptr<Subtexture2D> subtexture,
	                      const float rotation      = 0.0f,
	                      const float tiling_factor = 1.0f);
	static void draw_quad(const glm::vec3 position, const glm::vec2 size,
	                      std::shared_ptr<Subtexture2D> subtexture,
	                      const float rotation      = 0.0f,
	                      const float tiling_factor = 1.0f);
	static void draw_quad(const glm::vec2 position, const glm::vec2 size,
	                      std::shared_ptr<Subtexture2D> subtexture,
	                      const glm::vec3 tint, const float rotation = 0.0f,
	                      const float tiling_factor = 1.0f);
	static void draw_quad(const glm::vec3 position, const glm::vec2 size,
	                      std::shared_ptr<Subtexture2D> subtexture,
	                      const glm::vec3 tint, const float rotation = 0.0f,
	                      const float tiling_factor = 1.0f);
	static void draw_quad(const glm::vec2 position, const glm::vec2 size,
	                      std::shared_ptr<Subtexture2D> subtexture,
	                      const glm::vec4 tint, const float rotation = 0.0f,
	                      const float tiling_factor = 1.0f);
	static void draw_quad(const glm::vec3 position, const glm::vec2 size,
	                      std::shared_ptr<Subtexture2D> subtexture,
	                      const glm::vec4 tint, const float rotation = 0.0f,
	                      const float tiling_factor = 1.0f);

	static void
	draw_quad(const glm::mat4 transform, std::shared_ptr<Texture> texture,
	          const std::array<const glm::vec2, 4> texture_coordinates,
	          const glm::vec4 tint, const float tiling_factor = 1.0f);
};

} // namespace sol