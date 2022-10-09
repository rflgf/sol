#include "Platform/WindowsWindow.h"

#include "Log.h"

namespace sol
{

Window *Window::create(const WindowProps &props)
{
	return new WindowsWindow(props);
}

WindowsWindow::WindowsWindow(const WindowProps &props) { init(props); }

WindowsWindow::~WindowsWindow() { shutdown(); }

void WindowsWindow::init(const WindowProps &props)
{
	data.title  = props.title;
	data.width  = props.width;
	data.height = props.height;
	data.vsync  = true;

	int success = SDL_Init(SDL_INIT_VIDEO);
	SOL_CORE_ASSERT(!success, "unable to initialize SDL: {}", SDL_GetError());

	window = SDL_CreateWindow(
	    data.title.c_str(), SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
	    static_cast<int>(data.width), static_cast<int>(data.height),
	    SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);

	SOL_CORE_ASSERT(window, "could not create window: {}", SDL_GetError());

	SOL_CORE_INFO("created window {} with dimensions ({}, {})", data.title,
	              data.width, data.height);

	gl_context = SDL_GL_CreateContext(window);
}

void WindowsWindow::shutdown()
{
	SDL_GL_DeleteContext(gl_context);
	SDL_DestroyWindow(window);
	SDL_Quit();
}

void WindowsWindow::on_update()
{
	SDL_Event e;
	while (SDL_PollEvent(&e) != 0)
	{
	}
	SDL_GL_SwapWindow(window);
}

void WindowsWindow::set_vsync(bool enabled = true)
{
	if (enabled)
		SDL_GL_SetSwapInterval(1);
	else
		SDL_GL_SetSwapInterval(0);
}

bool WindowsWindow::is_vsync() const { return data.vsync; }

} // namespace sol