#pragma once

#include "Event/Event.h"

#include <sstream>

namespace sol
{

struct AppRenderEvent : public Event
{
public:
	SOL_EVENT_CLASS_TYPE(SOL_APP_RENDER)
	SOL_EVENT_CLASS_CATEGORY(SOL_APPLICATION)
};

struct AppTickEvent : public Event
{
public:
	SOL_EVENT_CLASS_TYPE(SOL_APP_TICK)
	SOL_EVENT_CLASS_CATEGORY(SOL_APPLICATION)
};

struct AppUpdateEvent : public Event
{
public:
	SOL_EVENT_CLASS_TYPE(SOL_APP_UPDATE)
	SOL_EVENT_CLASS_CATEGORY(SOL_APPLICATION)
};

struct WindowCloseEvent : public Event
{
public:
	SOL_EVENT_CLASS_TYPE(SOL_WINDOW_CLOSE)
	SOL_EVENT_CLASS_CATEGORY(SOL_APPLICATION)
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

	SOL_EVENT_CLASS_TYPE(SOL_WINDOW_RESIZE)
	SOL_EVENT_CLASS_CATEGORY(SOL_APPLICATION)
};

} // namespace sol