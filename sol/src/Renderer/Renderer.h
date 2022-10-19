#pragma once

namespace sol
{

class Renderer
{
public:
	enum class API
	{
		// clang-format off
		NONE    = 0,
		OPEN_GL = 1,
		// clang-format on
	};

private:
	static API api;

public:
	static API get_api() { return api; }
};

} // namespace sol