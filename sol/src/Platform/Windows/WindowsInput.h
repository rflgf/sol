#pragma once

#include "Core.h"
#include "Input.h"

namespace sol
{
class WindowsInput : public Input
{
public:
	static void init()
	{
		SOL_CORE_ASSERT(!Input::instance,
		                "attempt to instantiate multiple Input");
		instance = new WindowsInput();
	}
	static void deinit()
	{
		SOL_CORE_ASSERT(instance, "attempt to delete unitialized Input");
		delete instance;
	}

protected:
	bool is_key_pressed_impl(KeyCode key_code) override;
	bool is_key_released_impl(KeyCode key_code) override;
	bool is_mouse_button_pressed_impl(MouseButtonCode button_code) override;
	std::pair<float, float> get_mouse_position_impl() const override;
	float get_mouse_x_impl() const override;
	float get_mouse_y_impl() const override;
};
} // namespace sol