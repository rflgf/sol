#include "Platform/WindowsWindow.h"

#include "Event/ApplicationEvent.h"
#include "Event/KeyboardEvent.h"
#include "Event/MouseEvent.h"
#include "Log.h"

namespace sol
{

uint16_t WindowsWindow::sol_key_from(SDL_Keysym &keysym) { return 0; }

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
	poll_events();
	SDL_GL_SwapWindow(window);
}

void WindowsWindow::poll_events()
{

	SDL_Event e;
	while (SDL_PollEvent(&e))
		switch (e.type)
		{

		// application events:
		case SDL_QUIT:
		{
			WindowCloseEvent event;
			data.callback(event);
			break;
		}
		case SDL_WINDOWEVENT:
		{
			if (e.window.event == SDL_WINDOWEVENT_RESIZED)
			{
				WindowResizeEvent event {static_cast<uint16_t>(e.window.data1),
				                         static_cast<uint16_t>(e.window.data2)};
				data.callback(event);
			}
			break;
		}

		// keyboard events:
		case SDL_KEYDOWN:
		{
			KeyPressedEvent event {sol_key_from(e.key.keysym), e.key.repeat};
			data.callback(event);
			break;
		}
		case SDL_KEYUP:
		{
			KeyReleasedEvent event {sol_key_from(e.key.keysym)};
			data.callback(event);
			break;
		}

		// mouse events:
		case SDL_MOUSEMOTION:
		{
			MouseMovedEvent event {static_cast<float>(e.motion.x),
			                       static_cast<float>(e.motion.y)};
			data.callback(event);
			break;
		}
		case SDL_MOUSEBUTTONDOWN:
		{
			MouseButtonPressedEvent event {e.button.button};
			data.callback(event);
			break;
		}
		case SDL_MOUSEBUTTONUP:
		{
			MouseButtonReleasedEvent event {e.button.button};
			data.callback(event);
			break;
		}
		case SDL_MOUSEWHEEL:
		{
			MouseScrolledEvent event {e.wheel.preciseX, e.wheel.preciseY};
			data.callback(event);
			break;
		}

		default:
			break;
		};
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