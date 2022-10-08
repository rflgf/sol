#pragma once

#include "Application.h"
#include "Log.h"

namespace sol
{
extern Application *GetApplication();
}

int main(int argc, char **argv)
{
	sol::Log::init();
	SOL_CORE_ERROR("why hello world!");

	sol::Application *app = sol::GetApplication();
	app->run();
	delete app;
}