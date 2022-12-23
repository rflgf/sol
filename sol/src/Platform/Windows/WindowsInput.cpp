#include "Application.h"
#include "Input.h"

#ifdef SOL_PLATFORM_WINDOWS

#include <SDL.h>
#undef main

namespace sol
{
bool Input::is_key_pressed(KeyCode key_code)
{
	static const uint8_t *kb_state = SDL_GetKeyboardState(nullptr);
	return kb_state[SDL_GetScancodeFromKey(key_code)] == 1;
}

bool Input::is_key_released(KeyCode key_code)
{
	return !is_key_pressed(key_code);
}

bool Input::is_mouse_button_pressed(MouseButtonCode button_code)
{
	uint32_t mouse_state = SDL_GetMouseState(nullptr, nullptr);
	return (button_code & SDL_BUTTON(mouse_state)) != 0;
}

std::pair<float, float> Input::get_mouse_position()
{
	static int mouse_x, mouse_y;
	SDL_GetMouseState(&mouse_x, &mouse_y);

	return {static_cast<uint16_t>(mouse_x), static_cast<uint16_t>(mouse_y)};
}

float Input::get_mouse_x() { return get_mouse_position().first; }

float Input::get_mouse_y() { return get_mouse_position().second; }

} // namespace sol

#else
#error "only define SOL_PLATFORM_WINDOWS is supported at the moment."
#endif