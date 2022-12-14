#pragma once

#include "RenderingContext.h"

#include <SDL.h>
#undef main
#include <backends/imgui_impl_opengl3.h>
#include <glad/glad.h>

namespace sol
{
class OpenGLContext : public RenderingContext
{
private:
	SDL_Window *window;
	SDL_GLContext gl_context;

public:
	OpenGLContext(SDL_Window *window)
	    : window(window) {};
	void init() override;
	void deinit() override;
	void swap_buffers() override;
	void *get_native_rendering_context() const override;
};
} // namespace sol