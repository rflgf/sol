#pragma once

#include "Timestep.h"

namespace sol::platform
{

struct Clock
{
private:
	static Clock clock;

public:
	virtual Timestep tick() const;

	static Clock &get_clock() { return clock; }
};

} // namespace sol::platform