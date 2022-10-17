#include "Platform/WindowsInput.h"

#include "Application.h"

#include <SDL.h>
#undef main

namespace sol
{
bool WindowsInput::is_key_pressed_impl(KeyCode key_code)
{
	static const uint8_t *kb_state = SDL_GetKeyboardState(nullptr);
	return kb_state[SDL_GetScancodeFromKey(key_code)] == 1;
}

bool WindowsInput::is_key_released_impl(KeyCode key_code)
{
	return !is_key_pressed_impl(key_code);
}

bool WindowsInput::is_mouse_button_pressed_impl(MouseButtonCode button_code)
{
	uint32_t mouse_state = SDL_GetMouseState(nullptr, nullptr);
	return (button_code & SDL_BUTTON(mouse_state)) != 0;
}

std::pair<float, float> WindowsInput::get_mouse_position_impl() const
{
	static int mouse_x, mouse_y;
	SDL_GetMouseState(&mouse_x, &mouse_y);

	return {static_cast<uint16_t>(mouse_x), static_cast<uint16_t>(mouse_y)};
}

float WindowsInput::get_mouse_x_impl() const
{
	return get_mouse_position_impl().first;
}

float WindowsInput::get_mouse_y_impl() const
{
	return get_mouse_position_impl().second;
}

} // namespace sol