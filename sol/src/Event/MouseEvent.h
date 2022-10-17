#pragma once

#include "Event/Event.h"

#include <sstream>

namespace sol
{

struct MouseButtonEvent : public Event
{
protected:
	MouseButtonCode button_code;

	MouseButtonEvent(MouseButtonCode button_code)
	    : button_code(button_code)
	{
	}

public:
	MouseButtonCode get_button_code() const { return button_code; }

	EVENT_CLASS_CATEGORY(INPUT | MOUSE | MOUSE_BUTTON)
};

struct MouseButtonPressedEvent : public MouseButtonEvent
{
public:
	MouseButtonPressedEvent(MouseButtonCode button_code)
	    : MouseButtonEvent(button_code)
	{
	}

	virtual std::string to_string() const override
	{
		std::stringstream ss;
		ss << "MouseButtonPressedEvent: " << button_code;
		return ss.str();
	}

	EVENT_CLASS_TYPE(MOUSE_BUTTON_PRESSED)
};

struct MouseButtonReleasedEvent : public MouseButtonEvent
{
public:
	MouseButtonReleasedEvent(MouseButtonCode button_code)
	    : MouseButtonEvent(button_code)
	{
	}

	virtual std::string to_string() const override
	{
		std::stringstream ss;
		ss << "MouseButtonReleasedEvent: " << button_code;
		return ss.str();
	}

	EVENT_CLASS_TYPE(MOUSE_BUTTON_RELEASED)
};

struct MouseMovedEvent : public Event
{
private:
	float x, y;

public:
	MouseMovedEvent(float x, float y)
	    : x(x)
	    , y(y)
	{
	}

	float get_x() const { return x; }
	float get_y() const { return y; }

	virtual std::string to_string() const override
	{
		std::stringstream ss;
		ss << "MouseMovedEvent: (" << x << ", " << y << ')';
		return ss.str();
	}

	EVENT_CLASS_CATEGORY(INPUT | MOUSE)
	EVENT_CLASS_TYPE(MOUSE_MOVED)
};

struct MouseScrolledEvent : public Event
{
private:
	// some mice seem to have 2D "wheel" input, so we also keep track of
	// x_offset, even though the xs axis probably won't ever be used.
	float x_offset, y_offset;

public:
	MouseScrolledEvent(float x_offset, float y_offset)
	    : x_offset(x_offset)
	    , y_offset(y_offset)
	{
	}

	float get_x_offset() const { return x_offset; }
	float get_y_offset() const { return y_offset; }

	virtual std::string to_string() const override
	{
		std::stringstream ss;
		ss << "MouseScrolledEvent: (" << x_offset << ", " << y_offset << ')';
		return ss.str();
	}

	EVENT_CLASS_CATEGORY(INPUT | MOUSE)
	EVENT_CLASS_TYPE(MOUSE_SCROLLED)
};

} // namespace sol