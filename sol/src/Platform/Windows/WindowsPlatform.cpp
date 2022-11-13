#include "WindowsPlatform.h"

#ifdef SOL_PLATFORM_WINDOWS

#include <SDL.h>
#undef main

namespace sol::platform
{

Timestep Clock::tick() const { return SDL_GetTicks64(); }

} // namespace sol::platform

#endif