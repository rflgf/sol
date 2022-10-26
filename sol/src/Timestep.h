#pragma once

#include <iostream>

namespace sol
{

struct Timestep
{
private:
	float time;

public:
	Timestep(float miliseconds)
	    : time(miliseconds)
	{
	}

	float in_seconds() { return time / 1000.0f; }
	float in_miliseconds() { return time; }

	operator float() const { return time; }

	Timestep &operator-=(Timestep rhs)
	{
		this->time -= rhs.time;
		return *this;
	}

	Timestep &operator+=(Timestep rhs)
	{
		this->time += rhs.time;
		return *this;
	}

	Timestep &operator/=(Timestep rhs)
	{
		this->time *= rhs.time;
		return *this;
	}

	Timestep &operator*=(Timestep rhs)
	{
		this->time *= rhs.time;
		return *this;
	}

	friend class Application;
	friend std::ostream &operator<<(std::ostream &os, const Timestep &dt);
};

using DeltaTime = Timestep;

} // namespace sol