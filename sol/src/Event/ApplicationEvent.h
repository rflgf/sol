#pragma once

#include "Event/Event.h"

#include <sstream>

namespace sol
{

struct AppRenderEvent : public Event
{
public:
	EVENT_CLASS_TYPE(APP_RENDER)
	EVENT_CLASS_CATEGORY(APPLICATION)
};

struct AppTickEvent : public Event
{
public:
	EVENT_CLASS_TYPE(APP_TICK)
	EVENT_CLASS_CATEGORY(APPLICATION)
};

struct AppUpdateEvent : public Event
{
public:
	EVENT_CLASS_TYPE(APP_UPDATE)
	EVENT_CLASS_CATEGORY(APPLICATION)
};

struct WindowCloseEvent : public Event
{
public:
	EVENT_CLASS_TYPE(WINDOW_CLOSE)
	EVENT_CLASS_CATEGORY(APPLICATION)
};

struct WindowResizeEvent : public Event
{
private:
	uint16_t width, height;

public:
	WindowResizeEvent(uint16_t width, uint16_t height)
	    : width(width)
	    , height(height)
	{
	}

	uint16_t get_width() const { return width; }
	uint16_t get_height() const { return height; }

	virtual std::string to_string() const override
	{
		std::stringstream ss;
		ss << "WindowResizeEvent: " << static_cast<unsigned>(width) << ", "
		   << static_cast<unsigned>(height);
		return ss.str();
	}

	EVENT_CLASS_TYPE(WINDOW_RESIZE)
	EVENT_CLASS_CATEGORY(APPLICATION)
};

} // namespace sol