#include "DockspaceLayer.h"

#include "KeyCodes.h"
#include "Scene/Components.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace sol::ecl
{

DockspaceLayer::DockspaceLayer()
    : camera_controller(1280.0f / 720.0f)
    , subtexture(Subtexture2D::from_coordinates(
          Texture2D::create("assets/textures/kenney_tinydungeon/"
                            "Tilemap/tilemap_packed.png"),
          glm::vec2 {0, 3}, glm::vec2 {16, 16}))
    , scene_view_size(1, 1)
{
	Framebuffer::Specification specification;
	specification.width  = 1280;
	specification.height = 720;
	framebuffer          = Framebuffer::create(specification);

	Entity mage = active_scene.create("mage");
	mage.add<cmp::SpriteRenderer>(subtexture);
	mage.replace<cmp::Transform>(
	    glm::scale(glm::mat4(1.0f), {0.5f, 0.5f, 1.0f}));

	camera_A = active_scene.create("main camera");
	// camera_A.add<cmp::Camera>(
	//     glm::ortho(-16.0f, 16.0f, -9.0f, 9.0f, -1.0f, 1.0f));
	camera_A.add<cmp::Camera>();

	camera_B = active_scene.create("clip-space camera");
	// camera_B.add<cmp::Camera>(
	//     glm::ortho(-1.0f, 1.0f, -1.0f, 1.0f, -1.0f, 1.0f), false);
	camera_B.add<cmp::Camera>(false);

	class ScriptedCameraController : public ScriptableEntity
	{
	public:
		void on_create() override {}

		void on_destroy() override {}

		void on_update(Timestep dt) override
		{
			glm::mat4 &transform = get<cmp::Transform>().transform;
			float speed          = 0.01f;
			if (Input::is_key_pressed(KeyCode::SOL_a))
				transform[3][0] -= speed * dt;
			if (Input::is_key_pressed(KeyCode::SOL_d))
				transform[3][0] += speed * dt;
			if (Input::is_key_pressed(KeyCode::SOL_s))
				transform[3][1] -= speed * dt;
			if (Input::is_key_pressed(KeyCode::SOL_w))
				transform[3][1] += speed * dt;
		}
	};

	camera_A.add<cmp::NativeScript>().bind<ScriptedCameraController>();
}

void DockspaceLayer::on_update(Timestep dt)
{
	Framebuffer::Specification specification =
	    framebuffer.get()->get_specification();
	if (specification.width != scene_view_size.x ||
	    specification.height != scene_view_size.y)
	{
		framebuffer.get()->resize(scene_view_size);
		camera_controller.on_resize(scene_view_size.x, scene_view_size.y);
		active_scene.on_viewport_resize(scene_view_size);
	}

	if (viewport_focused)
		camera_controller.on_update(dt);

	framebuffer.get()->bind();

	RenderCommand::set_clear_color({0.1f, 0.1f, 0.1f, 1});
	RenderCommand::clear();

	active_scene.on_update(dt);

	framebuffer.get()->unbind();
}

void DockspaceLayer::on_imgui_update()
{
	static bool p_open                        = true;
	static bool opt_padding                   = false;
	static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

	ImGuiWindowFlags window_flags =
	    ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;

	const ImGuiViewport *viewport = ImGui::GetMainViewport();
	ImGui::SetNextWindowPos(viewport->WorkPos);
	ImGui::SetNextWindowSize(viewport->WorkSize);
	ImGui::SetNextWindowViewport(viewport->ID);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
	window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse |
	                ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
	window_flags |=
	    ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;

	if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
		window_flags |= ImGuiWindowFlags_NoBackground;

	if (!opt_padding)
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
	ImGui::Begin("Eclipse Editor", &p_open, window_flags);
	if (!opt_padding)
		ImGui::PopStyleVar();

	ImGui::PopStyleVar(2);

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
			                    true))
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
			scene_view_size = as;

		uint32_t texture_id =
		    framebuffer.get()->get_color_attachment_renderer_id();
		ImGui::Image(reinterpret_cast<void *>(texture_id),
		             {scene_view_size.x, scene_view_size.y}, {0, 1}, {1, 0});

		ImGui::End();
	}

	{
		ImGui::Begin("Settings");

		static bool primary_camera = true;
		if (ImGui::Checkbox("primary camera", &primary_camera))
		{
			camera_A.get<cmp::Camera>().primary = primary_camera;
			camera_B.get<cmp::Camera>().primary = !primary_camera;
		}

		if (primary_camera)
			ImGui::DragFloat3(
			    "main camera's position",
			    glm::value_ptr(camera_A.get<cmp::Transform>().transform[3]));
		else
		{
			ImGui::DragFloat3(
			    "clip-space camera's position",
			    glm::value_ptr(camera_B.get<cmp::Transform>().transform[3]));

			SceneCamera &camera = camera_B.get<cmp::Camera>().camera;
			float orthographic_projection_size = camera.orthographic_size;
			if (ImGui::DragFloat("orthographic projection size",
			                     &orthographic_projection_size))
				camera.set_orthographic_size(orthographic_projection_size);
		}

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