#pragma once

#include "Event/ApplicationEvent.h"
#include "Event/KeyboardEvent.h"
#include "Event/MouseEvent.h"
#include "Layer.h"

namespace sol
{

class ImGuiLayer
{
private:
	bool block_events = true;

public:
	void begin();
	void end();

	void on_update_begin();
	void on_update_end();
	void on_event(Event &event);

	void load_style_settings();

	void set_block_events(bool block) { block_events = block; }
	bool get_block_events() const { return block_events; }
};

} // namespace sol