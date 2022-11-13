#include "Platform.h"

#include "Core.h"

#ifdef SOL_PLATFORM_WINDOWS
#include "Platform/Windows/WindowsPlatform.h"
#else
#error                                                                         \
    "only Windows is supported by sol at the moment. define -DSOL_PLATFORM_WINDOWS"
#endif

namespace sol::platform
{

Clock Clock::clock = Clock {};

} // namespace sol::platform