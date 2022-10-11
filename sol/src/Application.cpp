#include "Application.h"

#include "Event/ApplicationEvent.h"
#include "Event/Event.h"
#include "Event/KeyboardEvent.h"
#include "Event/MouseEvent.h"

namespace sol
{

Application::Application()
{
	window = std::unique_ptr<Window>(Window::create());
	window->set_event_callback(
	    [this](Event &e) -> bool
	    {
		    on_event(e);
		    return true;
	    });
}

Application::~Application() {}

void Application::run()
{
	while (running)
	{
		window->on_update();
	}
}

void Application::on_event(Event &event)
{
	Event::Dispatcher dispatcher(event);

	dispatcher.dispatch<WindowCloseEvent>(
	    [this](Event &e) -> bool
	    {
		    running = false;
		    return true;
	    });

	SOL_CORE_TRACE("{}", event);
}

} // namespace sol