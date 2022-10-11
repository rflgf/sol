#pragma once

#include "Event/Event.h"

#include <functional>

namespace sol
{

struct Window
{
	using EventCallbackFn = std::function<bool(Event &)>;

public:
	struct WindowProps
	{
	public:
		std::string title;
		uint16_t width, height;

		WindowProps(const std::string &title = "Sol Window",
		            uint16_t width = 1280, uint16_t height = 720)
		    : title(title)
		    , width(width)
		    , height(height)
		{
		}
	};

	virtual ~Window() {}

	virtual void on_update() = 0;

	virtual uint16_t get_width() const  = 0;
	virtual uint16_t get_height() const = 0;

	virtual void set_event_callback(const EventCallbackFn &callback) = 0;
	virtual void set_vsync(bool enabled)                             = 0;
	virtual bool is_vsync() const                                    = 0;

	static Window *create(const WindowProps &props = WindowProps {});
};

} // namespace sol