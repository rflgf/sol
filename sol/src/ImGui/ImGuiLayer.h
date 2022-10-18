#pragma once

#include "Event/ApplicationEvent.h"
#include "Event/KeyboardEvent.h"
#include "Event/MouseEvent.h"
#include "Layer.h"

#include <backends/imgui_impl_opengl3.h>
#include <backends/imgui_impl_sdl.h>
#include <glad/glad.h>
#include <imgui.h>

namespace sol
{

class ImGuiLayer
{
public:
	void begin();
	void end();

	void on_update_begin();
	void on_update_end();
	void on_event(Event &event);
};

} // namespace sol