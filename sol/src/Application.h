#pragma once

#include "ImGui/ImGuiLayer.h"
#include "Layer.h"
#include "LayerStack.h"
#include "Renderer/Buffers.h"
#include "Renderer/Shader.h"
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

	uint32_t vao;
	std::unique_ptr<VertexBuffer> vbo;
	std::unique_ptr<IndexBuffer> ibo;

public:
	Application();
	virtual ~Application();

	void on_event(Event &event);
	void run();

	void push_layer(Layer *layer);
	void push_overlay(Layer *overlay);

	static Application &get() { return *application_instance; }
	Window *get_window() const { return window.get(); }
};

Application *GetApplication();

} // namespace sol