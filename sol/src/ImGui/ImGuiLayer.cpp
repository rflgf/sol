#include "ImGuiLayer.h"

#include "Application.h"
#include "Event/ApplicationEvent.h"
#include "Event/KeyboardEvent.h"
#include "Event/MouseEvent.h"

#include <backends/imgui_impl_opengl3.h>
#include <backends/imgui_impl_sdl.h>
#include <imgui.h>

void CherryTheme();

namespace sol
{
void ImGuiLayer::begin()
{
	ImGui::CreateContext();

	auto &io = ImGui::GetIO();
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;

	CherryTheme();

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

} // namespace sol

// credits due to r-lyeh on github. thanks for sharing your theme!
void CherryTheme()
{
// cherry colors, 3 intensities
#define HI(v) ImVec4(0.502f, 0.075f, 0.256f, v)
#define MED(v) ImVec4(0.455f, 0.198f, 0.301f, v)
#define LOW(v) ImVec4(0.232f, 0.201f, 0.271f, v)
// backgrounds (@todo: complete with BG_MED, BG_LOW)
#define BG(v) ImVec4(0.200f, 0.220f, 0.270f, v)
// text
#define TEXT(v) ImVec4(0.860f, 0.930f, 0.890f, v)

	auto &style                           = ImGui::GetStyle();
	style.Colors[ImGuiCol_Text]           = TEXT(0.78f);
	style.Colors[ImGuiCol_TextDisabled]   = TEXT(0.28f);
	style.Colors[ImGuiCol_WindowBg]       = ImVec4(0.13f, 0.14f, 0.17f, 1.00f);
	style.Colors[ImGuiCol_ChildBg]        = BG(0.58f);
	style.Colors[ImGuiCol_PopupBg]        = BG(0.9f);
	style.Colors[ImGuiCol_Border]         = ImVec4(0.31f, 0.31f, 1.00f, 0.00f);
	style.Colors[ImGuiCol_BorderShadow]   = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
	style.Colors[ImGuiCol_FrameBg]        = BG(1.00f);
	style.Colors[ImGuiCol_FrameBgHovered] = MED(0.78f);
	style.Colors[ImGuiCol_FrameBgActive]  = MED(1.00f);
	style.Colors[ImGuiCol_TitleBg]        = LOW(1.00f);
	style.Colors[ImGuiCol_TitleBgActive]  = HI(1.00f);
	style.Colors[ImGuiCol_TitleBgCollapsed] = BG(0.75f);
	style.Colors[ImGuiCol_MenuBarBg]        = BG(0.47f);
	style.Colors[ImGuiCol_ScrollbarBg]      = BG(1.00f);
	style.Colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.09f, 0.15f, 0.16f, 1.00f);
	style.Colors[ImGuiCol_ScrollbarGrabHovered] = MED(0.78f);
	style.Colors[ImGuiCol_ScrollbarGrabActive]  = MED(1.00f);
	style.Colors[ImGuiCol_CheckMark]  = ImVec4(0.71f, 0.22f, 0.27f, 1.00f);
	style.Colors[ImGuiCol_SliderGrab] = ImVec4(0.47f, 0.77f, 0.83f, 0.14f);
	style.Colors[ImGuiCol_SliderGrabActive] =
	    ImVec4(0.71f, 0.22f, 0.27f, 1.00f);
	style.Colors[ImGuiCol_Button]        = ImVec4(0.47f, 0.77f, 0.83f, 0.14f);
	style.Colors[ImGuiCol_ButtonHovered] = MED(0.86f);
	style.Colors[ImGuiCol_ButtonActive]  = MED(1.00f);
	style.Colors[ImGuiCol_Header]        = MED(0.76f);
	style.Colors[ImGuiCol_HeaderHovered] = MED(0.86f);
	style.Colors[ImGuiCol_HeaderActive]  = HI(1.00f);
	style.Colors[ImGuiCol_Separator]     = ImVec4(0.14f, 0.16f, 0.19f, 1.00f);
	style.Colors[ImGuiCol_SeparatorHovered] = MED(0.78f);
	style.Colors[ImGuiCol_SeparatorActive]  = MED(1.00f);
	style.Colors[ImGuiCol_ResizeGrip] = ImVec4(0.47f, 0.77f, 0.83f, 0.04f);
	style.Colors[ImGuiCol_ResizeGripHovered]    = MED(0.78f);
	style.Colors[ImGuiCol_ResizeGripActive]     = MED(1.00f);
	style.Colors[ImGuiCol_PlotLines]            = TEXT(0.63f);
	style.Colors[ImGuiCol_PlotLinesHovered]     = MED(1.00f);
	style.Colors[ImGuiCol_PlotHistogram]        = TEXT(0.63f);
	style.Colors[ImGuiCol_PlotHistogramHovered] = MED(1.00f);
	style.Colors[ImGuiCol_TextSelectedBg]       = MED(0.43f);
	// [...]
	style.Colors[ImGuiCol_ModalWindowDimBg] = BG(0.73f);

	style.WindowPadding     = ImVec2(6, 4);
	style.WindowRounding    = 0.0f;
	style.FramePadding      = ImVec2(5, 2);
	style.FrameRounding     = 3.0f;
	style.ItemSpacing       = ImVec2(7, 1);
	style.ItemInnerSpacing  = ImVec2(1, 1);
	style.TouchExtraPadding = ImVec2(0, 0);
	style.IndentSpacing     = 6.0f;
	style.ScrollbarSize     = 12.0f;
	style.ScrollbarRounding = 16.0f;
	style.GrabMinSize       = 20.0f;
	style.GrabRounding      = 2.0f;

	style.WindowTitleAlign.x = 0.50f;

	style.Colors[ImGuiCol_Border] = ImVec4(0.539f, 0.479f, 0.255f, 0.162f);
	style.FrameBorderSize         = 0.0f;
	style.WindowBorderSize        = 1.0f;

#undef HI
#undef MED
#undef LOW
#undef BG
#undef TEXT
}