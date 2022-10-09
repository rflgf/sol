#pragma once

#include "Event/Event.h"

#include <sstream>

namespace sol
{

struct KeyboardEvent : public Event
{
protected:
	uint8_t key_code;

	KeyboardEvent(uint8_t key_code)
	    : key_code(key_code)
	{
	}

public:
	uint8_t get_key_code() const { return key_code; }

	EVENT_CLASS_CATEGORY(INPUT | KEYBOARD)
};

struct KeyPressedEvent : public KeyboardEvent
{
private:
	uint8_t repeat_count = 0;

public:
	KeyPressedEvent(uint8_t key_code, uint8_t repeat_count = 0)
	    : KeyboardEvent(key_code)
	    , repeat_count(repeat_count)
	{
	}

	uint8_t get_repeat_count() const { return repeat_count; }

	virtual std::string to_string() const override
	{
		std::stringstream ss;
		ss << "KeyPressedEvent: " << static_cast<unsigned>(key_code) << " ("
		   << static_cast<unsigned>(repeat_count) << " times)";
		return ss.str();
	}

	EVENT_CLASS_TYPE(KEY_PRESSED)
};

struct KeyReleasedEvent : public KeyboardEvent
{
public:
	KeyReleasedEvent(uint8_t key_code)
	    : KeyboardEvent(key_code)
	{
	}

	virtual std::string to_string() const override
	{
		std::stringstream ss;
		ss << "KeyReleasedEvent: " << static_cast<unsigned>(key_code);
		return ss.str();
	}

	EVENT_CLASS_TYPE(KEY_RELEASED)
};

} // namespace sol