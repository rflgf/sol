#include "ImGuiLayer.h"

#include "Application.h"
#include "Event/ApplicationEvent.h"
#include "Event/KeyboardEvent.h"
#include "Event/MouseEvent.h"

namespace sol
{
ImGuiLayer::ImGuiLayer(const std::string name, bool enabled)
    : Layer(name, enabled)
{
}

ImGuiLayer::~ImGuiLayer() {}

void ImGuiLayer::on_attach()
{
	ImGui::CreateContext();
	ImGui::StyleColorsDark();

	auto &io = ImGui::GetIO();
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;

	Window *window_abstraction = Application::get().get_window();
	SDL_Window *native_window =
	    static_cast<SDL_Window *>(window_abstraction->get_native_window());
	void *native_rendering_context =
	    window_abstraction->get_native_rendering_context();
	ImGui_ImplSDL2_InitForOpenGL(native_window, native_rendering_context);
	ImGui_ImplOpenGL3_Init("#version 410");
}

void ImGuiLayer::on_detatch()
{
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplSDL2_Shutdown();
	ImGui::DestroyContext();
}

void ImGuiLayer::on_update()
{
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplSDL2_NewFrame();
	ImGui::NewFrame();

	static bool active = true;
	ImGui::Begin("ImGui Debug Layer", &active, ImGuiWindowFlags_MenuBar);
	ImGui::ShowDemoWindow();
	ImGui::End();

	ImGui::EndFrame();
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void ImGuiLayer::on_event(Event &event)
{
	ImGuiIO &io = ImGui::GetIO();
	event.handled |=
	    event.is_in_category(Event::Category::MOUSE) & io.WantCaptureMouse;
	event.handled |= event.is_in_category(Event::Category::KEYBOARD) &
	                 io.WantCaptureKeyboard;
	if (event.handled)
		ImGui_ImplSDL2_ProcessEvent(
		    static_cast<const SDL_Event *>(event.underlying_event));
}

} // namespace sol