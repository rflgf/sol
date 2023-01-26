#pragma once

#include "Panels.h"

#include <imgui.h>

// ImGui needs to be included before ImGuizmo. this is here to prevent
// clang-format from doing its reordering thing.
#define SOL_DUMMY
#include <ImGuizmo.h>
#include <sol.h>

namespace sol::ecl
{

class DockspaceLayer : public Layer
{
	CameraController camera_controller;
	std::shared_ptr<Framebuffer> framebuffer;
	std::shared_ptr<Scene> active_scene;
	glm::vec2 scene_view_size;
	bool viewport_focused = false;
	bool viewport_hovered = false;
	pnl::SceneHierarchy scene_hierarchy;
	EditorCamera camera;
	ImGuizmo::OPERATION gizmo_operation = ImGuizmo::OPERATION::TRANSLATE;

public:
	DockspaceLayer();

	void on_update(Timestep dt) override;
	void on_imgui_update() override;
	void on_event(Event &event) override;
};

} // namespace sol::ecl