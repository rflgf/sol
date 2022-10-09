#include "Application.h"

namespace sol
{

Application::Application()
{
	window = std::unique_ptr<Window>(Window::create());
}

Application::~Application() {}

void Application::run()
{
	while (running)
	{
		window->on_update();
	}
}

} // namespace sol