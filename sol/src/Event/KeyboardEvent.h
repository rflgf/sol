#pragma once

#include "Event/Event.h"

#include <sstream>

namespace sol
{

struct KeyboardEvent : public Event
{
protected:
	KeyCode key_code;

	KeyboardEvent(KeyCode key_code)
	    : key_code(key_code)
	{
	}

public:
	KeyCode get_key_code() const { return key_code; }

	EVENT_CLASS_CATEGORY(INPUT | KEYBOARD)
};

struct KeyPressedEvent : public KeyboardEvent
{
private:
	uint8_t repeat_count = 0;

public:
	KeyPressedEvent(KeyCode key_code, uint8_t repeat_count = 0)
	    : KeyboardEvent(key_code)
	    , repeat_count(repeat_count)
	{
	}

	uint8_t get_repeat_count() const { return repeat_count; }

	virtual std::string to_string() const override
	{
		std::stringstream ss;
		ss << "KeyPressedEvent: " << key_code << " ("
		   << static_cast<char>(key_code) << " as char), repeated "
		   << static_cast<unsigned>(repeat_count) << " times)";
		return ss.str();
	}

	EVENT_CLASS_TYPE(KEY_PRESSED)
};

struct KeyReleasedEvent : public KeyboardEvent
{
public:
	KeyReleasedEvent(KeyCode key_code)
	    : KeyboardEvent(key_code)
	{
	}

	virtual std::string to_string() const override
	{
		std::stringstream ss;
		ss << "KeyReleasedEvent: " << key_code << " ("
		   << static_cast<char>(key_code) << " as char)";
		return ss.str();
	}

	EVENT_CLASS_TYPE(KEY_RELEASED)
};

struct KeyTypedEvent : public KeyboardEvent
{
public:
	KeyTypedEvent(KeyCode key_code)
	    : KeyboardEvent(key_code)
	{
	}

	virtual std::string to_string() const override
	{
		std::stringstream ss;
		ss << "KeyTypedEvent: " << static_cast<char>(key_code);
		return ss.str();
	}

	EVENT_CLASS_TYPE(KEY_TYPED)
};

} // namespace sol