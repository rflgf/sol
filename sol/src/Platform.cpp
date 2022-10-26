#include "Platform.h"

#include "Core.h"

#ifdef SOL_PLATFORM_WINDOWS
#include "Platform/Windows/WindowsPlatform.h"
#else
#error "only Windows is supported at the moment. define -DSOL_PLATFORM_WINDOWS"
#endif

namespace sol
{
namespace platform
{

Clock Clock::clock = Clock{};

} // namespace platform
} // namespace sol