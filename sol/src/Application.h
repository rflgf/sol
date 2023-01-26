#pragma once

#include "ImGui/ImGuiLayer.h"
#include "Layer.h"
#include "LayerStack.h"
#include "Renderer/Buffers.h"
#include "Renderer/OrthographicCamera.h"
#include "Renderer/Renderer.h"
#include "Renderer/Shader.h"
#include "Scene/Scene.h"
#include "Window.h"

#include <memory>

namespace sol

{

class Application
{
private:
	static inline Application *application_instance = nullptr;

protected:
	bool running = true;
	std::unique_ptr<Window> window;
	LayerStack layer_stack;
	ImGuiLayer imgui_layer;

public:
	Application();
	virtual ~Application();

	void on_event(Event &event);
	void run();
	void close();

	void push_layer(Layer *layer);
	void push_overlay(Layer *overlay);

	static Application &get() { return *application_instance; }
	Window *get_window() const { return window.get(); }
	const ImGuiLayer &get_imgui_layer() const { return imgui_layer; }
	ImGuiLayer &get_imgui_layer() { return imgui_layer; }
};

Application *GetApplication();

} // namespace sol