#pragma once

#include "Panels.h"

#include <imgui.h>
#include <sol.h>

namespace sol::ecl
{

class DockspaceLayer : public Layer
{
	CameraController camera_controller;
	std::shared_ptr<Subtexture2D> subtexture;
	std::shared_ptr<Framebuffer> framebuffer;
	Scene active_scene;
	Entity camera_A;
	Entity camera_B;
	glm::vec2 scene_view_size;
	bool viewport_focused = false;
	bool viewport_hovered = false;
	pnl::SceneHierarchy scene_hierarchy;

public:
	DockspaceLayer();

	void on_update(Timestep dt) override;
	void on_imgui_update() override;
	void on_event(Event &event) override;
};

} // namespace sol::ecl