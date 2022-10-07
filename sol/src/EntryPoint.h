#pragma once

#include "Application.h"

namespace sol
{
extern Application *GetApplication();
}

int main(int argc, char **argv)
{
	sol::Application *app = sol::GetApplication();
	app->run();
	delete app;
}