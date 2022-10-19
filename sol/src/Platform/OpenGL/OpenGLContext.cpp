#include "OpenGLContext.h"

#include "Core.h"

#include <backends/imgui_impl_opengl3.h>
#include <glad/glad.h>

namespace sol
{

void OpenGLContext::init()
{
	gl_context = SDL_GL_CreateContext(window);
	SOL_CORE_ASSERT(gl_context, "failed to create GL context");

	int status = gladLoadGLLoader(SDL_GL_GetProcAddress);
	SOL_CORE_ASSERT(status, "failed to initialize OpenGL context with glad");

	SDL_GL_MakeCurrent(window, gl_context);
	SDL_GL_SetSwapInterval(1);
}

void OpenGLContext::deinit() { SDL_GL_DeleteContext(gl_context); }

void OpenGLContext::swap_buffers() { SDL_GL_SwapWindow(window); }

void *OpenGLContext::get_native_rendering_context() const
{
	return static_cast<void *>(gl_context);
}

} // namespace sol