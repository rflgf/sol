#pragma once

#include "Timestep.h"

namespace sol::platform
{

struct Clock
{
	virtual Timestep tick() const;
	static Clock clock;
};

} // namespace sol::platform