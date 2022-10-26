#pragma once

#include "Timestep.h"

namespace sol
{
namespace platform
{

struct Clock
{
	virtual Timestep tick() const;
	static Clock clock;
};

} // namespace platform
} // namespace sol