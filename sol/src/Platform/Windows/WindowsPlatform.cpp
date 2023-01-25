#include "WindowsPlatform.h"

#ifdef SOL_PLATFORM_WINDOWS

#include <SDL.h>
#undef main

namespace sol::platform
{

// Timestep --------------------------------------------------------------------
Timestep Clock::tick() const { return SDL_GetTicks64(); }
// end of Timestep -------------------------------------------------------------

} // namespace sol::platform

#else
#error "only define SOL_PLATFORM_WINDOWS is supported at the moment."
#endif