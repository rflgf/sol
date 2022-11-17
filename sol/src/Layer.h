#pragma once

#include "Event/Event.h"
#include "Timestep.h"

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

	virtual void on_attach() {}
	virtual void on_detatch() {}
	virtual void on_update(Timestep dt) {}
	virtual void on_imgui_update() {}
	virtual void on_event(Event &event) {}
	virtual void enable() { enabled = true; }
	virtual void disable() { enabled = false; }
	bool is_enabled() const { return enabled; }

	const std::string &get_name() { return debug_name; }
};

} // namespace sol