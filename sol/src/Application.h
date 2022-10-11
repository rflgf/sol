#pragma once

#include "Window.h"

#include <memory>

namespace sol
{

class Application
{
private:
	bool running = true;
	std::unique_ptr<Window> window;

public:
	Application();
	virtual ~Application();

	void on_event(Event &event);
	void run();
};

Application *GetApplication();

} // namespace sol