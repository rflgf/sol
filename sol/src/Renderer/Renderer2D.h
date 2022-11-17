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
	struct Data
	{
		std::shared_ptr<VertexArray> vao;
		std::shared_ptr<const Shader> tinted_texture_shader;
		std::shared_ptr<const Texture> white_texture;
	};

	static Data *data;

	static void init();
	static void deinit();

	static void begin_scene(const OrthographicCamera &camera);
	static void end_scene();

	static void draw_quad(const glm::vec2 position, const glm::vec2 size,
	                      const glm::vec4 color, const float rotation = 0.0f);
	static void draw_quad(const glm::vec3 position, const glm::vec2 size,
	                      const glm::vec4 color, const float rotation = 0.0f);
	static void draw_quad(const glm::vec2 position, const glm::vec2 size,
	                      const Texture &texture, const float rotation = 0.0f);
	static void draw_quad(const glm::vec3 position, const glm::vec2 size,
	                      const Texture &texture, const float rotation = 0.0f);
	static void draw_quad(const glm::vec2 position, const glm::vec2 size,
	                      const Texture &texture, const glm::vec3 tint,
	                      const float rotation = 0.0f);
	static void draw_quad(const glm::vec3 position, const glm::vec2 size,
	                      const Texture &texture, const glm::vec3 tint,
	                      const float rotation = 0.0f);
	static void draw_quad(const glm::vec2 position, const glm::vec2 size,
	                      const Texture &texture, const glm::vec4 tint,
	                      const float rotation = 0.0f);
	static void draw_quad(const glm::vec3 position, const glm::vec2 size,
	                      const Texture &texture, const glm::vec4 tint,
	                      const float rotation = 0.0f);
};

} // namespace sol