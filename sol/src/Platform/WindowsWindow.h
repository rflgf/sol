#pragma once

#include "Window.h"

#include <SDL.h>
#undef main
#include <string>

namespace sol
{

struct WindowsWindow : public Window
{
private:
	struct Data
	{
		std::string title;
		uint8_t width, height;
		bool vsync;

		EventCallbackFn callback;
	};
	Data data;
	SDL_Window *window;
	SDL_GLContext gl_context;

public:
	WindowsWindow(const WindowProps &props);
	virtual ~WindowsWindow();

	virtual void on_update() override;
	virtual uint8_t get_width() const { return data.width; };
	virtual uint8_t get_height() const { return data.height; };

	virtual void set_event_callback(const EventCallbackFn &callback) override
	{
		data.callback = callback;
	}
	virtual void set_vsync(bool enabled) override;
	virtual bool is_vsync() const override;

	virtual void init(const WindowProps &props);
	virtual void shutdown();
};
} // namespace sol