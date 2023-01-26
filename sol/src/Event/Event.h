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
		SOL_NONE = 0,

		SOL_APP_RENDER, SOL_APP_TICK, SOL_APP_UPDATE,

		SOL_KEY_PRESSED, SOL_KEY_RELEASED, SOL_KEY_TYPED,

		SOL_MOUSE_BUTTON_PRESSED, SOL_MOUSE_BUTTON_RELEASED,
		SOL_MOUSE_MOVED, SOL_MOUSE_SCROLLED,

		SOL_WINDOW_CLOSE, SOL_WINDOW_FOCUS, SOL_WINDOW_LOST_FOCUS,
		SOL_WINDOW_MOVED, SOL_WINDOW_RESIZE
		// clang-format on
	};

	enum Category : uint16_t
	{
		SOL_NONE         = 0,
		SOL_APPLICATION  = SOL_BIT(0),
		SOL_INPUT        = SOL_BIT(1),
		SOL_KEYBOARD     = SOL_BIT(2),
		SOL_MOUSE        = SOL_BIT(3),
		SOL_MOUSE_BUTTON = SOL_BIT(4)
	};

	class Dispatcher
	{
		template <typename T>
		using EventFn = std::function<bool(T &)>;

	private:
		Event &event;

	public:
		Dispatcher(Event &event)
		    : event(event)
		{
		}

		template <typename T>
		    requires std::is_base_of_v<Event, T> bool
		dispatch(EventFn<T> func)
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

// FIXME(rafael): this nukes EVERYTHING:
// inline std::ostream &operator<<(std::ostream &os, const Event &e)
// {
// 	return os << e.to_string();
// }

#define SOL_EVENT_CLASS_TYPE(type)                                             \
	static Type get_static_type()                                              \
	{                                                                          \
		return Type::type;                                                     \
	}                                                                          \
	virtual Type get_event_type() const override                               \
	{                                                                          \
		return get_static_type();                                              \
	}                                                                          \
	virtual const char *get_name() const override                              \
	{                                                                          \
		return #type;                                                          \
	}

#define SOL_EVENT_CLASS_CATEGORY(category)                                     \
	virtual uint16_t get_category_flags() const override                       \
	{                                                                          \
		return category;                                                       \
	}

} // namespace sol