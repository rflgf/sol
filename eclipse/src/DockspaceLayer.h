#pragma once

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
	glm::vec2 scene_view_size;
	bool viewport_focused = false;
	bool viewport_hovered = false;

public:
	DockspaceLayer();

	void on_update(Timestep dt) override;
	void on_imgui_update() override;
	void on_event(Event &event) override;
};

} // namespace sol::ecl