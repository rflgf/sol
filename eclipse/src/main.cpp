#define SOL_ENTRY_POINT_SOURCE_FILE
#include <imgui.h>
#include <sol.h>

namespace ecl
{

class DockSpace : public sol::Layer
{
	std::shared_ptr<sol::Texture2D> texture;

public:
	DockSpace(std::shared_ptr<sol::Texture2D> texture)
	    : texture(texture)
	{
	}

	void on_imgui_update() override
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
			ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding,
			                    ImVec2(0.0f, 0.0f));
		ImGui::Begin("Eclipse DockSpace", &p_open, window_flags);
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
				if (ImGui::MenuItem(
				        "Flag: NoDockingInCentralNode", "",
				        (dockspace_flags &
				         ImGuiDockNodeFlags_NoDockingInCentralNode) != 0))
				{
					dockspace_flags ^=
					    ImGuiDockNodeFlags_NoDockingInCentralNode;
				}
				if (ImGui::MenuItem("Flag: AutoHideTabBar", "",
				                    (dockspace_flags &
				                     ImGuiDockNodeFlags_AutoHideTabBar) != 0))
				{
					dockspace_flags ^= ImGuiDockNodeFlags_AutoHideTabBar;
				}
				if (ImGui::MenuItem("Flag: PassthruCentralNode", "",
				                    (dockspace_flags &
				                     ImGuiDockNodeFlags_PassthruCentralNode) !=
				                        0,
				                    opt_fullscreen))
				{
					dockspace_flags ^= ImGuiDockNodeFlags_PassthruCentralNode;
				}
				ImGui::Separator();

				if (ImGui::MenuItem("Close", NULL, false, p_open != NULL))
				{
					p_open = false;
					sol::Application::get().close();
				}

				ImGui::EndMenu();
			}

			ImGui::EndMenuBar();
		}

		{
			ImGui::Begin("statistics");

			ImGui::Text("draw calls: %d",
			            sol::Renderer2D::data->statistics.draw_calls);
			ImGui::Text("quad count: %d",
			            sol::Renderer2D::data->statistics.quad_count);
			ImGui::Text("index count: %d",
			            sol::Renderer2D::data->statistics.total_index_count());
			ImGui::Text("vertex count: %d",
			            sol::Renderer2D::data->statistics.total_vertex_count());

			ImGui::End();
			sol::Renderer2D::data->statistics.reset();
		}

		{
			ImGui::Begin("viewport");

			uint32_t texture_id = texture.get()->get_texture_api_id();
			ImGui::Image(reinterpret_cast<void *>(texture_id), {64, 64});

			ImGui::End();
		}

		ImGui::End();
	}
};

class Eclipse : public sol::Application
{
public:
	Eclipse()
	    : Application()
	{
		layer_stack.push(
		    new DockSpace(sol::Texture2D::create("assets/textures/logo.png")));
	}
	~Eclipse() {}
};

} // namespace ecl

sol::Application *sol::GetApplication() { return new ecl::Eclipse(); }
