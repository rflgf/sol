#include "ImGuiLayer.h"

#include "Application.h"
#include "Event/ApplicationEvent.h"
#include "Event/KeyboardEvent.h"
#include "Event/MouseEvent.h"

#include <imgui.h>
// ImGui needs to be included before ImGuizmo. this is here to prevent
// clang-format from doing its reordering thing.
#define SOL_DUMMY

#include <ImGuizmo.h>
#include <backends/imgui_impl_opengl3.h>
#include <backends/imgui_impl_sdl.h>

void CherryTheme();

namespace sol
{
void ImGuiLayer::begin()
{
	ImGui::CreateContext();

	auto &io = ImGui::GetIO();
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;

	load_style_settings();

	Window *window_abstraction = Application::get().get_window();
	SDL_Window *native_window =
	    static_cast<SDL_Window *>(window_abstraction->get_native_window());
	void *native_rendering_context = window_abstraction->get_rendering_context()
	                                     ->get_native_rendering_context();
	ImGui_ImplSDL2_InitForOpenGL(native_window, native_rendering_context);
	ImGui_ImplOpenGL3_Init("#version 420");
}

void ImGuiLayer::end()
{
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplSDL2_Shutdown();
	ImGui::DestroyContext();
}

void ImGuiLayer::on_update_begin()
{
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplSDL2_NewFrame();
	ImGui::NewFrame();
	ImGuizmo::BeginFrame();
}

void ImGuiLayer::on_update_end()
{
	ImGui::EndFrame();
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void ImGuiLayer::on_event(Event &event)
{
	// FIXME(rafael): event.handled should be changed based on whether the imgui
	// layer actually handled the event.
	ImGui_ImplSDL2_ProcessEvent(
	    static_cast<const SDL_Event *>(event.underlying_event));
	if (block_events && event.get_event_type() != Event::Type::WINDOW_CLOSE)
		event.handled = true;
}

void ImGuiLayer::load_style_settings()
{
	auto &style = ImGui::GetStyle();

	style.WindowPadding     = ImVec2(4, 4);
	style.CellPadding       = ImVec2(4, 2);
	style.WindowRounding    = 0.0f;
	style.FramePadding      = ImVec2(8, 2);
	style.FrameRounding     = 4.0f;
	style.ItemSpacing       = ImVec2(0, 0);
	style.ItemInnerSpacing  = ImVec2(4, 1);
	style.TouchExtraPadding = ImVec2(0, 0);
	style.IndentSpacing     = 8.0f;
	style.ScrollbarSize     = 16.0f;
	style.ScrollbarRounding = 16.0f;
	style.GrabMinSize       = 16.0f;
	style.GrabRounding      = 2.0f;

	style.WindowTitleAlign.x       = 0.50f;
	style.WindowMenuButtonPosition = ImGuiDir_Right;
	ImGui::PushStyleVar(ImGuiStyleVar_WindowMinSize, ImVec2(274, 10));
	style.FrameBorderSize  = 0.0f;
	style.WindowBorderSize = 1.0f;

	ImGui::StyleColorsLight();
}

} // namespace sol