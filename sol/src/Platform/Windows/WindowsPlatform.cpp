#include "WindowsPlatform.h

#ifdef SOL_PLATFORM_WINDOWS

#include <SDL.h>
#undef main

namespace sol
{
namespace platform
{

Timestep Clock::tick() const { return SDL_GetTicks64(); }

} // namespace platform
} // namespace sol

#endif