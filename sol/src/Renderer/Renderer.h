#pragma once

#include "Buffers.h"
#include "RenderCommand.h"
#include "Shader.h"

#include <glm/glm.hpp>
#include <memory>

namespace sol
{

class Renderer
{
public:
	static void init();
	static void deinit();

	static void begin_scene();
	static void end_scene();

	static RendererAPI::API get_api_type() { return RendererAPI::get_type(); }

	static void submit(const Shader &shader, VertexArray &vao);
};
} // namespace sol