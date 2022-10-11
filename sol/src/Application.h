#pragma once

#include "Layer.h"
#include "LayerStack.h"
#include "Window.h"

#include <memory>

namespace sol
{

class Application
{
protected:
	bool running = true;
	std::unique_ptr<Window> window;
	LayerStack layer_stack;

public:
	Application();
	virtual ~Application();

	void on_event(Event &event);
	void run();

	void push_layer(Layer *layer);
	void push_overlay_layer(Layer *overlay_layer);
};

Application *GetApplication();

} // namespace sol