#pragma once

#include "Layer.h"

#include <vector>

namespace sol
{

class LayerStack
{
private:
	std::vector<Layer *> layers;
	unsigned int insertion_index = 0;

public:
	LayerStack();
	~LayerStack();

	void push(Layer *layer);
	void push_overlay(Layer *layer);
	void pop(Layer *layer);
	void pop_overlay(Layer *layer);

	std::vector<Layer *>::iterator begin() { return layers.begin(); }
	std::vector<Layer *>::iterator end() { return layers.end(); }
};
} // namespace sol