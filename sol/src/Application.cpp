#include "Application.h"

#include "Event/ApplicationEvent.h"
#include "Event/Event.h"
#include "Event/KeyboardEvent.h"
#include "Event/MouseEvent.h"
#include "ImGui/ImGuiLayer.h"
#include "Platform.h"

namespace sol
{
Application::Application()

{
	SOL_CORE_ASSERT(!application_instance,
	                "attempt to instantiate multiple Application");
	application_instance = this;

	window = std::unique_ptr<Window>(Window::create());
	window->set_event_callback(
	    [this](Event &e) -> bool
	    {
		    on_event(e);
		    return true;
	    });

	Renderer::init();

	imgui_layer.begin();
}

Application::~Application() { imgui_layer.end(); }

void Application::run()
{
	Timestep now = 0, previous_dt = 0;
	while (running)
	{
		previous_dt = now;
		now         = platform::Clock::get_clock().tick();
		Timestep dt = now - previous_dt;

		RenderCommand::set_clear_color({0.3f, 0.6f, 0.9f, 1.0f});
		RenderCommand::clear();

		for (Layer *layer : layer_stack)
			if (layer->is_enabled())
				layer->on_update(dt);

		imgui_layer.on_update_begin();
		for (Layer *layer : layer_stack)
			if (layer->is_enabled())
				layer->on_imgui_update();
		imgui_layer.on_update_end();

		window->on_update();
	}

	layer_stack = LayerStack();
}

void Application::close() { running = false; }

void Application::on_event(Event &event)
{
	imgui_layer.on_event(event);
	if (event.handled)
		return;

	Event::Dispatcher dispatcher(event);

	dispatcher.dispatch<WindowCloseEvent>(
	    [this](Event &e) -> bool
	    {
		    running = false;
		    return true;
	    });

	for (auto it = layer_stack.end(); it != layer_stack.begin();)
	{
		--it;
		Layer &layer = *(*it);
		if (layer.is_enabled())
		{
			layer.on_event(event);
			if (event.handled)
				break;
		}
	}

	// SOL_CORE_TRACE("{}", event);
}

void Application::push_layer(Layer *layer) { layer_stack.push(layer); }

void Application::push_overlay(Layer *overlay)
{
	layer_stack.push_overlay(overlay);
}

} // namespace sol