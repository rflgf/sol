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

class ImGuiLayer : public Layer
{
public:
	ImGuiLayer(const std::string name = "ImGui Layer", bool enabled = true);
	virtual ~ImGuiLayer();

	virtual void on_attach() override;
	virtual void on_detatch() override;
	virtual void on_update() override;
	virtual void on_event(Event &event) override;
};

} // namespace sol