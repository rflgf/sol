#pragma once

#include "Buffers.h"
#include "OrthographicCamera.h"
#include "RenderCommand.h"
#include "Shader.h"

#include <glm/glm.hpp>
#include <memory>

namespace sol
{

class Renderer
{
public:
	struct SceneData
	{
		glm::mat4 view_projection_matrix;
	};

	static SceneData *data;

	static void init();
	static void deinit();

	static void begin_scene(OrthographicCamera &camera);
	static void end_scene();

	static RendererAPI::API get_api_type() { return RendererAPI::get_type(); }

	static void submit(const Shader &shader, VertexArray &vao, const glm::mat4 &transform = glm::mat4(1.0f));
};
} // namespace sol