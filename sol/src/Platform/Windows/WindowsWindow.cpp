#pragma once

#include "WindowsWindow.h"

#include "Event/ApplicationEvent.h"
#include "Event/KeyboardEvent.h"
#include "Event/MouseEvent.h"
#include "Input.h"
#include "Log.h"
#include "Platform/OpenGL/OpenGLContext.h"
#include "WindowsInput.h"

#include <backends/imgui_impl_sdl.h>
#include <imgui.h>

namespace sol
{

Window *Window::create(const WindowProps &props)
{
	Input::instance = new WindowsInput;
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

	rendering_context = std::make_unique<OpenGLContext>(window);
	rendering_context->init();
}

void WindowsWindow::shutdown()
{
	rendering_context->deinit();
	SDL_DestroyWindow(window);

	SDL_Quit();
}

void WindowsWindow::on_update()
{
	poll_events();
	rendering_context->swap_buffers();
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
			event.underlying_event = static_cast<void *>(&e);
			data.callback(event);
			break;
		}
		case SDL_WINDOWEVENT:
		{
			if (e.window.event == SDL_WINDOWEVENT_RESIZED)
			{
				WindowResizeEvent event {static_cast<uint16_t>(e.window.data1),
				                         static_cast<uint16_t>(e.window.data2)};
				event.underlying_event = static_cast<void *>(&e);
				data.callback(event);
			}
			break;
		}

		// keyboard events:
		case SDL_TEXTINPUT:
		{
			KeyTypedEvent event {static_cast<KeyCode>(e.key.keysym.sym)};
			event.underlying_event = static_cast<void *>(&e);
			data.callback(event);
			break;
		}
		case SDL_KEYDOWN:
		{
			KeyPressedEvent event {static_cast<KeyCode>(e.key.keysym.sym),
			                       e.key.repeat};
			event.underlying_event = static_cast<void *>(&e);
			data.callback(event);
			break;
		}
		case SDL_KEYUP:
		{
			KeyReleasedEvent event {static_cast<KeyCode>(e.key.keysym.sym)};
			event.underlying_event = static_cast<void *>(&e);
			data.callback(event);
			break;
		}

		// mouse events:
		case SDL_MOUSEMOTION:
		{
			MouseMovedEvent event {static_cast<float>(e.motion.x),
			                       static_cast<float>(e.motion.y)};
			event.underlying_event = static_cast<void *>(&e);
			SOL_TRACE(event);
			data.callback(event);
			break;
		}
		case SDL_MOUSEBUTTONDOWN:
		{
			MouseButtonPressedEvent event {
			    static_cast<MouseButtonCode>(e.button.button)};
			event.underlying_event = static_cast<void *>(&e);
			data.callback(event);
			break;
		}
		case SDL_MOUSEBUTTONUP:
		{
			MouseButtonReleasedEvent event {
			    static_cast<MouseButtonCode>(e.button.button)};
			event.underlying_event = static_cast<void *>(&e);
			data.callback(event);
			break;
		}
		case SDL_MOUSEWHEEL:
		{
			MouseScrolledEvent event {e.wheel.preciseX, e.wheel.preciseY};
			event.underlying_event = static_cast<void *>(&e);
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

void *WindowsWindow::get_native_window() const
{
	return static_cast<void *>(window);
}

} // namespace sol