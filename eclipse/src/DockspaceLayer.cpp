#include "DockspaceLayer.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <imgui.h>

namespace sol::ecl
{

DockspaceLayer::DockspaceLayer()
    : camera_controller(1280.0f / 720.0f)
    , scene_view_size(1, 1)
    , active_scene(std::make_shared<Scene>())
{
	Framebuffer::Specification specification;
	specification.width  = 1280;
	specification.height = 720;
	framebuffer          = Framebuffer::create(specification);
	camera               = EditorCamera(30.0f, 1.778f, 0.1f, 1000.0f);

	Entity mage = active_scene->create("mage");
	mage.add<cmp::SpriteRenderer>(Subtexture2D::from_coordinates(
	    Texture2D::create("assets/textures/kenney_tinydungeon/"
	                      "Tilemap/tilemap_packed.png"),
	    glm::vec2 {0, 3}, glm::vec2 {16, 16}, {2, 2}));
	mage.replace<cmp::Transform>(glm::vec3(0.0f), glm::vec3(0.0f),
	                             glm::vec3(0.5f, 0.5f, 1.0f));

	Entity camera_A = active_scene->create("main camera");
	camera_A.add<cmp::Camera>(
	    /*primary camera*/ true,
	    /*fixed aspect ratio*/ true,
	    /*camera type*/ SceneCamera::Type::PERSPECTIVE);

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

	scene_hierarchy.context = active_scene.get();

	ImGuiIO &io = ImGui::GetIO();

	io.FontDefault = io.Fonts->AddFontFromFileTTF(
	    "assets/fonts/OpenSans/static/OpenSans/OpenSans-Medium.ttf", 16.0f);
	io.Fonts->AddFontFromFileTTF("assets/fonts/Early GameBoy/Early GameBoy.ttf",
	                             10.0f);

	io.IniFilename = "assets/settings/imgui.ini";
}

void DockspaceLayer::on_update(Timestep dt)
{
	// gizmo operation keyboard control.
	if (!ImGuizmo::IsUsing())
	{
		if (Input::is_key_pressed(KeyCode::SOL_ESCAPE))
			gizmo_operation = static_cast<ImGuizmo::OPERATION>(-1);
		else if (Input::is_key_pressed(KeyCode::SOL_g))
			gizmo_operation = ImGuizmo::OPERATION::TRANSLATE;
		else if (Input::is_key_pressed(KeyCode::SOL_r))
			gizmo_operation = ImGuizmo::OPERATION::ROTATE;
		else if (Input::is_key_pressed(KeyCode::SOL_s))
			gizmo_operation = ImGuizmo::OPERATION::SCALE;
	}

	Framebuffer::Specification specification = framebuffer->get_specification();
	if (specification.width != scene_view_size.x ||
	    specification.height != scene_view_size.y)
	{
		framebuffer->resize(scene_view_size);
		// camera_controller.on_resize(scene_view_size.x, scene_view_size.y);
		camera.set_viewport_size(scene_view_size.x, scene_view_size.y);
		active_scene->on_viewport_resize(scene_view_size);
	}

	// if (viewport_focused)
	// 	camera_controller.on_update(dt);

	camera.on_update(dt);

	framebuffer->bind();

	RenderCommand::set_clear_color({0.0f, 0.0f, 0.0f, 1});
	RenderCommand::clear();

	// active_scene->on_update(dt);
	active_scene->on_update_editor(dt, camera);

	framebuffer->unbind();
}

void DockspaceLayer::on_imgui_update()
{
	// ImGuizmo::BeginFrame();
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
			auto new_scene = [&]()
			{
				// TODO(rafael): confirm save dialog.
				// for now, only one scene can be run at the same time. in the
				// future, make it so opening and creating scenes only opens
				// them in a separate tab.
				active_scene                      = std::make_shared<Scene>();
				scene_hierarchy.context           = active_scene.get();
				scene_hierarchy.selection_context = {};
			};
			ImGui::Separator();
			if (ImGui::MenuItem("new", "Ctrl+N", false, p_open != NULL))
				new_scene();

			if (ImGui::MenuItem("open...", "Ctrl+O", false, p_open != NULL))
			{
				std::string filepath = utils::FileDialog::open_file(
				    {{"sol scene", "scene,yaml"}},
				    utils::current_working_directory("assets/scenes"));
				if (!filepath.empty())
				{
					new_scene();
					SceneSerializer serializer(active_scene);
					serializer.deserialize_text(filepath);
					active_scene->on_viewport_resize(scene_view_size);
				}
			}

			ImGui::Separator();

			if (ImGui::MenuItem("save as...", "Ctrl+Shift+S", false,
			                    p_open != NULL))
			{
				std::string filepath = utils::FileDialog::save_file(
				    "untitled", {{"sol scene", "scene,yaml"}},
				    utils::current_working_directory("assets/scenes"));
				if (!filepath.empty())
				{
					SceneSerializer serializer(active_scene);
					serializer.serialize_text(filepath);
				}
			}

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
		ImGui::Begin("statistics");

		ImGui::Text("draw calls: %d",
		            Renderer2D::get_statistics().get_draw_calls());
		ImGui::Text("quad count: %d",
		            Renderer2D::get_statistics().get_quad_count());
		ImGui::Text("index count: %d",
		            Renderer2D::get_statistics().total_index_count());
		ImGui::Text("vertex count: %d",
		            Renderer2D::get_statistics().total_vertex_count());

		ImGui::End();
		Renderer2D::get_statistics().reset();
	}

	{
		ImGui::Begin("scene viewport");

		viewport_focused = ImGui::IsWindowFocused();
		viewport_hovered = ImGui::IsWindowHovered();

		Application::get().get_imgui_layer().set_block_events(
		    !viewport_focused && !viewport_hovered);

		ImVec2 available_size = ImGui::GetContentRegionAvail();
		glm::vec2 as {available_size.x, available_size.y};
		if (scene_view_size != as && as.x > 0 && as.y > 0)
			scene_view_size = as;

		void *texture_id = std::bit_cast<void *>(
		    uintptr_t {framebuffer->get_color_attachment_renderer_id()});
		ImGui::Image(texture_id, {scene_view_size.x, scene_view_size.y}, {0, 1},
		             {1, 0});

		// ImGuizmo-related stuff.
		if (Entity selected_entity = scene_hierarchy.selection_context)
		{
			// get active camera. TODO(rafael): fix this when we have a
			// proper editor camera.
			bool should_snap = Input::is_key_pressed(KeyCode::SOL_LCTRL);
			float snap =
			    gizmo_operation == ImGuizmo::OPERATION::ROTATE ? 45.0f : 0.5f;
			float snap_values[] = {snap, snap, snap};

			ImGuizmo::SetOrthographic(false);
			ImGuizmo::SetDrawlist();

			ImGuizmo::SetRect(ImGui::GetWindowPos().x, ImGui::GetWindowPos().y,
			                  ImGui::GetWindowWidth(),
			                  ImGui::GetWindowHeight());

			cmp::Transform &selected_entitys_transform_component =
			    selected_entity.get<cmp::Transform>();
			glm::mat4 transform   = selected_entitys_transform_component;
			glm::mat4 camera_view = camera.get_view_matrix();

			ImGuizmo::SetGizmoSizeClipSpace(1.0f / camera.get_distance());
			ImGuizmo::Manipulate(glm::value_ptr(camera_view),
			                     glm::value_ptr(camera.get_projection_matrix()),
			                     gizmo_operation, ImGuizmo::LOCAL,
			                     glm::value_ptr(transform), nullptr,
			                     should_snap ? snap_values : nullptr);

			if (ImGuizmo::IsUsing())
			{
				glm::vec3 rotation =
				    selected_entitys_transform_component.rotation;
				ImGuizmo::DecomposeMatrixToComponents(
				    glm::value_ptr(transform),
				    glm::value_ptr(
				        selected_entitys_transform_component.translation),
				    glm::value_ptr(rotation),
				    glm::value_ptr(selected_entitys_transform_component.scale));
				selected_entitys_transform_component.rotation = {
				    glm::radians(rotation.x), glm::radians(rotation.y),
				    glm::radians(rotation.z)};
			}
		}

		ImGui::End();
	}

	ImGui::End();
}

void DockspaceLayer::on_event(Event &event)
{
	// if (!event.handled)
	// 	camera_controller.on_event(event);
	if (!event.handled)
		camera.on_event(event);
}

} // namespace sol::ecl