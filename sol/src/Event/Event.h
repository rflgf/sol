#pragma once

#include "Core.h"
#include "KeyCodes.h"

#include <concepts>
#include <functional>
#include <string>
#include <type_traits>

namespace sol
{

struct Event
{
public:
	enum class Type
	{
		// clang-format off
		NONE = 0,

		APP_RENDER, APP_TICK, APP_UPDATE,

		KEY_PRESSED, KEY_RELEASED, KEY_TYPED,

		MOUSE_BUTTON_PRESSED, MOUSE_BUTTON_RELEASED, MOUSE_MOVED, MOUSE_SCROLLED,

		WINDOW_CLOSE, WINDOW_FOCUS, WINDOW_LOST_FOCUS, WINDOW_MOVED, WINDOW_RESIZE
		// clang-format on
	};

	enum Category : uint16_t
	{
		NONE         = 0,
		APPLICATION  = BIT(0),
		INPUT        = BIT(1),
		KEYBOARD     = BIT(2),
		MOUSE        = BIT(3),
		MOUSE_BUTTON = BIT(4)
	};

	class Dispatcher
	{
		template <typename T> using EventFn = std::function<bool(T &)>;

	private:
		Event &event;

	public:
		Dispatcher(Event &event)
		    : event(event)
		{
		}

		template <typename T>
		requires std::is_base_of_v<Event, T>
		bool dispatch(EventFn<T> func)
		{
			if (event.get_event_type() == T::get_static_type())
			{
				event.handled = func(*(T *)&event);
				return true;
			}
			return false;
		}
	};

	void *underlying_event = nullptr;
	bool handled           = false;

	virtual Event::Type get_event_type() const  = 0;
	virtual const char *get_name() const        = 0;
	virtual uint16_t get_category_flags() const = 0;
	virtual std::string to_string() const { return get_name(); }

	bool is_in_category(Event::Category c) { return get_category_flags() & c; }
};

inline std::ostream &operator<<(std::ostream &os, const Event &e)
{
	return os << e.to_string();
}

#define EVENT_CLASS_TYPE(type)                                                 \
	static Type get_static_type() { return Type::type; }                       \
	virtual Type get_event_type() const override { return get_static_type(); } \
	virtual const char *get_name() const override { return #type; }

#define EVENT_CLASS_CATEGORY(category)                                         \
	virtual uint16_t get_category_flags() const override { return category; }

} // namespace sol