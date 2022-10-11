#pragma once

#include "Window.h"

#include <SDL.h>
#undef main
#include <string>

namespace sol
{

struct WindowsWindow : public Window
{
public:
	struct Data
	{
		std::string title;
		uint16_t width, height;
		bool vsync;

		EventCallbackFn callback;
	};

private:
	Data data;
	SDL_Window *window;
	SDL_GLContext gl_context;

public:
	WindowsWindow(const WindowProps &props);
	virtual ~WindowsWindow();

	virtual void on_update() override;
	virtual uint16_t get_width() const { return data.width; };
	virtual uint16_t get_height() const { return data.height; };

	virtual void set_event_callback(const EventCallbackFn &callback) override
	{
		data.callback = callback;
	}
	virtual void set_vsync(bool enabled) override;
	virtual bool is_vsync() const override;

	virtual void init(const WindowProps &props);
	virtual void shutdown();

	void poll_events();
	static uint16_t sol_key_from(SDL_Keysym &keysym);
};
} // namespace sol