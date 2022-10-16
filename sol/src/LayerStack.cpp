#include "LayerStack.h"

namespace sol
{
LayerStack::LayerStack() {}

LayerStack::~LayerStack()
{
	for (Layer *layer : layers)
	{
		if (layer->is_enabled())
			layer->on_detatch();
		delete layer;
	}
}

void LayerStack::push(Layer *layer)
{
	layers.emplace(layers.begin() + insertion_index, layer);
	++insertion_index;
	if (layer->is_enabled())
		layer->on_attach();
}

void LayerStack::push_overlay(Layer *overlay)
{
	layers.emplace_back(overlay);
	if (overlay->is_enabled())
		overlay->on_attach();
}

void LayerStack::pop(Layer *layer)
{
	std::vector<Layer *>::iterator it =
	    std::find(layers.begin(), layers.end(), layer);
	if (it != layers.end())
	{
		layers.erase(it);
		--insertion_index;
		if (layer->is_enabled())
			layer->on_detatch();
	}

#ifdef SOL_DEBUG
	else
		SOL_CORE_WARN("attempt to pop layer not present on the stack: {}",
		              layer->get_name());
#endif
}

void LayerStack::pop_overlay(Layer *overlay)
{

	std::vector<Layer *>::iterator it =
	    std::find(layers.begin(), layers.end(), overlay);
	if (it != layers.end())
	{
		layers.erase(it);
		if (overlay->is_enabled())
			overlay->on_detatch();
	}

#ifdef SOL_DEBUG
	else
		SOL_CORE_WARN(
		    "attempt to pop_overlay layer not present on the stack: {}",
		    overlay->get_name());
#endif
}

} // namespace sol