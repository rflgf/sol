#include "OpenGLRendererAPI.h"

#include <glad/glad.h>

namespace sol
{

void OpenGLRendererAPI::init()
{
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glEnable(GL_DEPTH_TEST);
}

void OpenGLRendererAPI::set_viewport(uint32_t x, uint32_t y, uint32_t width,
                                     uint32_t height)
{
	glViewport(x, y, width, height);
}

void OpenGLRendererAPI::draw_indexed(VertexArray &vao, size_t count)
{
	// NOTE(rafael)> disabled the following line because it was getting in the
	// way of clearing the screen:
	// count = count ? count : vao.get_index_buffer()->get_count();
	glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_INT, nullptr);
}
void OpenGLRendererAPI::set_clear_color(const glm::vec4 &color)
{
	glClearColor(color.r, color.g, color.b, color.a);
}

void OpenGLRendererAPI::clear() const
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}
} // namespace sol