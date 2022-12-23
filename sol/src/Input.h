#pragma once

#include "KeyCodes.h"

#include <utility>

namespace sol
{

class Input
{
public:
	static bool is_key_pressed(KeyCode key_code);
	static bool is_key_released(KeyCode key_code);
	static bool is_mouse_button_pressed(MouseButtonCode button_code);
	static std::pair<float, float> get_mouse_position();
	static float get_mouse_x();
	static float get_mouse_y();

	friend class Window;
};

} // namespace sol