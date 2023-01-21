#include "DockspaceLayer.h"

#include "ImGuiUtils.h"
#include "KeyCodes.h"
#include "Scene/Components.h"
#include "Scene/SceneCamera.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <imgui.h>

namespace sol::ecl
{

DockspaceLayer::DockspaceLayer()
    : camera_controller(1280.0f / 720.0f)
    , scene_view_size(1, 1)
{
	Framebuffer::Specification specification;
	specification.width  = 1280;
	specification.height = 720;
	framebuffer          = Framebuffer::create(specification);

	Entity mage = active_scene.create("mage");
	mage.add<cmp::SpriteRenderer>(Subtexture2D::from_coordinates(
	    Texture2D::create("assets/textures/kenney_tinydungeon/"
	                      "Tilemap/tilemap_packed.png"),
	    glm::vec2 {0, 3}, glm::vec2 {16, 16}, {2, 2}));
	mage.replace<cmp::Transform>(glm::vec3(0.0f), glm::vec3(0.0f),
	                             glm::vec3(0.5f, 0.5f, 1.0f));

	Entity logo = active_scene.create("logo");
	logo.add<cmp::SpriteRenderer>(Subtexture2D::from_coordinates(
	    Texture2D::create("assets/textures/logo.png"), glm::vec2 {0, 0},
	    glm::vec2 {597, 604}, {1, 1}));
	logo.replace<cmp::Transform>(glm::vec3(0.3f, 0.0f, 0.0f), glm::vec3(0.0f),
	                             glm::vec3(1.0f));

	Entity potion = active_scene.create("potion");
	potion.add<cmp::SpriteRenderer>(Subtexture2D::from_coordinates(
	    Texture2D::create(
	        "assets/textures/kenney_tinydungeon/Tiles/tile_0115.png"),
	    glm::vec2 {0, 0}, glm::vec2 {16, 16}, {1, 1}));
	potion.replace<cmp::Transform>(glm::vec3(0.3f, 0.4f, 0.0f), glm::vec3(0.0f),
	                               glm::vec3(1.0f));

	camera_A = active_scene.create("main camera");
	camera_A.add<cmp::Camera>(
	    /*primary camera*/ true,
	    /*fixed aspect ratio*/ true,
	    /*camera type*/ SceneCamera::Type::PERSPECTIVE);

	camera_B = active_scene.create("clip-space camera");
	camera_B.add<cmp::Camera>(/*primary camera*/ false,
	                          /*fixed aspect ratio*/ true);

	class ScriptedCameraController : public ScriptableEntity
	{
	public:
		void on_create() override
		{
			glm::vec3 &position = get<cmp::Transform>().translation;
			position.z          = 1;
		}

		void on_destroy() override {}

		void on_update(Timestep dt) override
		{
			glm::vec3 &position = get<cmp::Transform>().translation;
			float speed         = 0.01f;
			if (Input::is_key_pressed(KeyCode::SOL_a))
				position.x -= speed * dt;
			if (Input::is_key_pressed(KeyCode::SOL_d))
				position.x += speed * dt;
			if (Input::is_key_pressed(KeyCode::SOL_s))
				position.y -= speed * dt;
			if (Input::is_key_pressed(KeyCode::SOL_w))
				position.y += speed * dt;
		}
	};

	camera_A.add<cmp::NativeScript>().bind<ScriptedCameraController>();

	scene_hierarchy.context = &active_scene;

	ImGuiIO &io = ImGui::GetIO();

	io.FontDefault = io.Fonts->AddFontFromFileTTF(
	    "assets/fonts/OpenSans/static/OpenSans/OpenSans-Medium.ttf", 16.0f);
	io.Fonts->AddFontFromFileTTF("assets/fonts/Early GameBoy/Early GameBoy.ttf",
	                             10.0f);

	io.IniFilename = "assets/settings/imgui.ini";
}

void DockspaceLayer::on_update(Timestep dt)
{
	Framebuffer::Specification specification = framebuffer->get_specification();
	if (specification.width != scene_view_size.x ||
	    specification.height != scene_view_size.y)
	{
		framebuffer->resize(scene_view_size);
		camera_controller.on_resize(scene_view_size.x, scene_view_size.y);
		active_scene.on_viewport_resize(scene_view_size);
	}

	if (viewport_focused)
		camera_controller.on_update(dt);

	framebuffer->bind();

	RenderCommand::set_clear_color({0.0f, 0.0f, 0.0f, 1});
	RenderCommand::clear();

	active_scene.on_update(dt);

	framebuffer->unbind();
}

void DockspaceLayer::on_imgui_update()
{
	static bool p_open                        = true;
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

	ImGui::Begin("Eclipse Editor", &p_open, window_flags);

	ImGui::PopStyleVar(2);

	ImGuiIO &io = ImGui::GetIO();
	if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
	{
		ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
		ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
	}

	if (ImGui::BeginMenuBar())
	{
		if (ImGui::BeginMenu("options"))
		{
			ImGui::Separator();

			if (ImGui::MenuItem("close", NULL, false, p_open != NULL))
			{
				p_open = false;
				Application::get().close();
			}

			ImGui::EndMenu();
		}

		ImGui::EndMenuBar();
	}

	scene_hierarchy.on_imgui_update();

	{
		// ImGui::ShowDemoWindow();
		ImGui::Begin("statistics");

		ImGui::Text("draw calls: %d", Renderer2D::data.statistics.draw_calls);
		ImGui::Text("quad count: %d", Renderer2D::data.statistics.quad_count);
		ImGui::Text("index count: %d",
		            Renderer2D::data.statistics.total_index_count());
		ImGui::Text("vertex count: %d",
		            Renderer2D::data.statistics.total_vertex_count());

		ImGui::End();
		Renderer2D::data.statistics.reset();
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

		void *texture_id = std::bit_cast<void *>(
		    uintptr_t {framebuffer->get_color_attachment_renderer_id()});
		ImGui::Image(texture_id, {scene_view_size.x, scene_view_size.y}, {0, 1},
		             {1, 0});

		ImGui::End();
	}

	{
		ImGui::Begin("settings");

		static bool primary_camera = true;
		ImGui::Text("primary camera");
		if (ImGui::Checkbox("##primary camera", &primary_camera))
		{
			camera_A.get<cmp::Camera>().primary = primary_camera;
			camera_B.get<cmp::Camera>().primary = !primary_camera;
		}

		if (primary_camera)
		{
			ImGui::Text("main camera's position");
			ImGui::DragFloat3(
			    "##main camera's position",
			    glm::value_ptr(camera_A.get<cmp::Transform>().translation));
		}
		else
		{
			ImGui::Text("clip-space camera's position");
			ImGui::DragFloat3(
			    "##clip-space camera's position",
			    glm::value_ptr(camera_B.get<cmp::Transform>().translation));

			SceneCamera &camera = camera_B.get<cmp::Camera>().camera;
			float orthographic_projection_size = camera.orthographic_size;

			ImGui::Text("orthographic projection size");
			if (ImGui::DragFloat("##orthographic projection size",
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