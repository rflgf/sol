#include "DockspaceLayer.h"

#include <glm/gtc/matrix_transform.hpp>

namespace sol::ecl
{

DockspaceLayer::DockspaceLayer()
    : camera_controller(1280.0f / 720.0f)
    , subtexture(Subtexture2D::from_coordinates(
          Texture2D::create("assets/textures/kenney_tinydungeon/"
                            "Tilemap/tilemap_packed.png"),
          glm::vec2 {0, 3}, glm::vec2 {16, 16}))
    , scene_view_size(0, 0)
{
	Framebuffer::Specification specification;
	specification.width  = 1280;
	specification.height = 720;
	framebuffer          = Framebuffer::create(specification);

	Entity mage = Application::get().active_scene.create("mage");
	mage.add<cmp::SpriteRenderer>(Subtexture2D::from_coordinates(
	    Texture2D::create("assets/textures/kenney_tinydungeon/"
	                      "Tilemap/tilemap_packed.png"),
	    glm::vec2 {0, 3}, glm::vec2 {16, 16}));
	mage.replace<cmp::Transform>(
	    glm::scale(glm::mat4(1.0f), {0.2f, 0.20f, 1.0f}));
}

void DockspaceLayer::on_update(Timestep dt)
{
	if (viewport_focused)
		camera_controller.on_update(dt);

	framebuffer.get()->bind();

	RenderCommand::set_clear_color({0.1f, 0.1f, 0.1f, 1});
	RenderCommand::clear();
	Renderer2D::begin_scene(camera_controller.get_camera());

	Application::get().active_scene.on_update(dt);

	auto group = Application::get().active_scene.registry.group<cmp::Transform>(
	    entt::get<cmp::SpriteRenderer>);

	for (auto entity : group)
	{
		auto [transform, sprite] =
		    group.get<cmp::Transform, cmp::SpriteRenderer>(entity);
		Renderer2D::draw_quad(transform, sprite);
	}

	// Renderer2D::draw_quad({0.6f, 0.3f, 0.2f}, {0.2f, 0.20f}, subtexture,
	//                       glm::vec4 {1.0f, 1.0f, 1.0f, 1.0f}, 0.0f, 1.0f);

	Renderer2D::end_scene();
	framebuffer.get()->unbind();
}

void DockspaceLayer::on_imgui_update()
{
	static bool p_open                        = true;
	static bool opt_fullscreen                = true;
	static bool opt_padding                   = false;
	static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

	// We are using the ImGuiWindowFlags_NoDocking flag to make the parent
	// window not dockable into, because it would be confusing to have two
	// docking targets within each others.
	ImGuiWindowFlags window_flags =
	    ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
	if (opt_fullscreen)
	{
		const ImGuiViewport *viewport = ImGui::GetMainViewport();
		ImGui::SetNextWindowPos(viewport->WorkPos);
		ImGui::SetNextWindowSize(viewport->WorkSize);
		ImGui::SetNextWindowViewport(viewport->ID);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
		window_flags |= ImGuiWindowFlags_NoTitleBar |
		                ImGuiWindowFlags_NoCollapse |
		                ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
		window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus |
		                ImGuiWindowFlags_NoNavFocus;
	}
	else
	{
		dockspace_flags &= ~ImGuiDockNodeFlags_PassthruCentralNode;
	}

	// When using ImGuiDockNodeFlags_PassthruCentralNode, DockSpace() will
	// render our background and handle the pass-thru hole, so we ask
	// Begin() to not render a background.
	if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
		window_flags |= ImGuiWindowFlags_NoBackground;

	// Important: note that we proceed even if Begin() returns false (aka
	// window is collapsed). This is because we want to keep our DockSpace()
	// active. If a DockSpace() is inactive, all active windows docked into
	// it will lose their parent and become undocked. We cannot preserve the
	// docking relationship between an active window and an inactive
	// docking, otherwise any change of dockspace/settings would lead to
	// windows being stuck in limbo and never being visible.
	if (!opt_padding)
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
	ImGui::Begin("Eclipse Editor", &p_open, window_flags);
	if (!opt_padding)
		ImGui::PopStyleVar();

	if (opt_fullscreen)
		ImGui::PopStyleVar(2);

	// Submit the DockSpace
	ImGuiIO &io = ImGui::GetIO();
	if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
	{
		ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
		ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
	}

	if (ImGui::BeginMenuBar())
	{
		if (ImGui::BeginMenu("Options"))
		{
			// Disabling fullscreen would allow the window to be moved to
			// the front of other windows, which we can't undo at the moment
			// without finer window depth/z control.
			ImGui::MenuItem("Fullscreen", NULL, &opt_fullscreen);
			ImGui::MenuItem("Padding", NULL, &opt_padding);
			ImGui::Separator();

			if (ImGui::MenuItem(
			        "Flag: NoSplit", "",
			        (dockspace_flags & ImGuiDockNodeFlags_NoSplit) != 0))
			{
				dockspace_flags ^= ImGuiDockNodeFlags_NoSplit;
			}
			if (ImGui::MenuItem(
			        "Flag: NoResize", "",
			        (dockspace_flags & ImGuiDockNodeFlags_NoResize) != 0))
			{
				dockspace_flags ^= ImGuiDockNodeFlags_NoResize;
			}
			if (ImGui::MenuItem("Flag: NoDockingInCentralNode", "",
			                    (dockspace_flags &
			                     ImGuiDockNodeFlags_NoDockingInCentralNode) !=
			                        0))
			{
				dockspace_flags ^= ImGuiDockNodeFlags_NoDockingInCentralNode;
			}
			if (ImGui::MenuItem(
			        "Flag: AutoHideTabBar", "",
			        (dockspace_flags & ImGuiDockNodeFlags_AutoHideTabBar) != 0))
			{
				dockspace_flags ^= ImGuiDockNodeFlags_AutoHideTabBar;
			}
			if (ImGui::MenuItem("Flag: PassthruCentralNode", "",
			                    (dockspace_flags &
			                     ImGuiDockNodeFlags_PassthruCentralNode) != 0,
			                    opt_fullscreen))
			{
				dockspace_flags ^= ImGuiDockNodeFlags_PassthruCentralNode;
			}
			ImGui::Separator();

			if (ImGui::MenuItem("Close", NULL, false, p_open != NULL))
			{
				p_open = false;
				Application::get().close();
			}

			ImGui::EndMenu();
		}

		ImGui::EndMenuBar();
	}

	{
		ImGui::Begin("statistics");

		ImGui::Text("draw calls: %d", Renderer2D::data->statistics.draw_calls);
		ImGui::Text("quad count: %d", Renderer2D::data->statistics.quad_count);
		ImGui::Text("index count: %d",
		            Renderer2D::data->statistics.total_index_count());
		ImGui::Text("vertex count: %d",
		            Renderer2D::data->statistics.total_vertex_count());

		ImGui::End();
		Renderer2D::data->statistics.reset();
	}

	{
		ImGui::Begin("scene viewport");

		viewport_focused = ImGui::IsWindowFocused();
		viewport_hovered = ImGui::IsWindowHovered();

		Application::get().imgui_layer.block_events =
		    !viewport_focused || !viewport_hovered;

		ImVec2 available_size = ImGui::GetContentRegionAvail();
		glm::vec2 as {available_size.x, available_size.y};
		if (scene_view_size != as && as.x > 0 && as.y > 0)
		{
			scene_view_size = as;
			framebuffer.get()->resize(scene_view_size);
			camera_controller.on_resize(scene_view_size.x, scene_view_size.y);
		}

		uint32_t texture_id =
		    framebuffer.get()->get_color_attachment_renderer_id();
		ImGui::Image(reinterpret_cast<void *>(texture_id), available_size,
		             {0, 1}, {1, 0});

		ImGui::End();
	}

	ImGui::End();
}

void DockspaceLayer::on_event(Event &event)
{
	if (!event.handled)
		camera_controller.on_event(event);
}

} // namespace sol::ecl