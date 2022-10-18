#pragma once

#include "Event/Event.h"

#include <string>

namespace sol
{

class Layer
{
protected:
	std::string debug_name;
	bool enabled;

public:
	Layer(const std::string name = "Layer", bool enabled = true);
	virtual ~Layer();

	virtual void on_attach()            = 0;
	virtual void on_detatch()           = 0;
	virtual void on_update()            = 0;
	virtual void on_imgui_update()      = 0;
	virtual void on_event(Event &event) = 0;
	virtual void enable() { enabled = true; }
	virtual void disable() { enabled = false; }
	bool is_enabled() const { return enabled; }

	const std::string &get_name() { return debug_name; }
};

} // namespace sol