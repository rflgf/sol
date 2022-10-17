#pragma once

#include "KeyCodes.h"

#include <utility>

namespace sol
{

class Input
{
protected:
	static Input *instance;

	virtual bool is_key_pressed_impl(KeyCode key_code)                     = 0;
	virtual bool is_key_released_impl(KeyCode key_code)                    = 0;
	virtual bool is_mouse_button_pressed_impl(MouseButtonCode button_code) = 0;
	virtual std::pair<float, float> get_mouse_position_impl() const        = 0;
	virtual float get_mouse_x_impl() const                                 = 0;
	virtual float get_mouse_y_impl() const                                 = 0;

public:
	static bool is_key_pressed(KeyCode key_code)
	{
		return instance->is_key_pressed_impl(key_code);
	}
	static bool is_key_released(KeyCode key_code)
	{
		return instance->is_key_released_impl(key_code);
	}
	static bool is_mouse_button_pressed(MouseButtonCode button_code)
	{
		return instance->is_mouse_button_pressed_impl(button_code);
	}
	static std::pair<float, float> get_mouse_position()
	{
		return instance->get_mouse_position_impl();
	}
	static float get_mouse_x() { return instance->get_mouse_x_impl(); }
	static float get_mouse_y() { return instance->get_mouse_y_impl(); }

	friend class Window;
};

} // namespace sol