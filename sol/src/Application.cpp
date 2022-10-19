#include "Application.h"

#include "Event/ApplicationEvent.h"
#include "Event/Event.h"
#include "Event/KeyboardEvent.h"
#include "Event/MouseEvent.h"
#include "ImGui/ImGuiLayer.h"

namespace sol
{

Shader *shader;

Application::Application()
{
	SOL_CORE_ASSERT(!application_instance,
	                "attempt to instantiate multiple Application");
	application_instance = this;

	window = std::unique_ptr<Window>(Window::create());
	window->set_event_callback(
	    [this](Event &e) -> bool
	    {
		    on_event(e);
		    return true;
	    });

	// disposable
	const std::string vert_src = R"(
		#version 330 core
		layout(location = 0) in vec3 a_Position;
		out vec3 position;
		void main() { gl_Position = vec4(a_Position, 1.0); 
		position = a_Position; }
	)";
	const std::string frag_src = R"(
		#version 330 core
		layout(location = 0) out vec4 color;
		in vec3 position;
		void main() { color = vec4(position * 0.5 + 0.5, 1); }
	)";

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	float vertices[4 * 3] = {
	    -0.5f, 0.5f,  0.0f, // bottom-left
	    0.5f,  0.5f,  0.0f, // bottom-right
	    -0.5f, -0.5f, 0.0f, // top-left
	    0.5f,  -0.5f, 0.0f, // top-right
	};
	vbo = std::unique_ptr<VertexBuffer>(
	    VertexBuffer::create(vertices, sizeof(vertices)));

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);

	unsigned int indices[6] = {2, 0, 3, 0, 3, 1};
	ibo                     = std::unique_ptr<IndexBuffer>(
        IndexBuffer::create(indices, sizeof(indices) / sizeof(uint32_t)));

	shader = new Shader(vert_src, frag_src);

	imgui_layer.begin();
}

Application::~Application() { imgui_layer.end(); }

void Application::run()
{
	while (running)
	{
		glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		shader->bind();

		glBindVertexArray(vao);
		glDrawElements(GL_TRIANGLES, ibo->get_count(), GL_UNSIGNED_INT,
		               nullptr);

		for (Layer *layer : layer_stack)
			if (layer->is_enabled())
				layer->on_update();

		imgui_layer.on_update_begin();
		for (Layer *layer : layer_stack)
			if (layer->is_enabled())
				layer->on_imgui_update();
		imgui_layer.on_update_end();

		window->on_update();
	}

	layer_stack = LayerStack();
}

void Application::on_event(Event &event)
{
	imgui_layer.on_event(event);
	if (event.handled)
		return;
	Event::Dispatcher dispatcher(event);

	dispatcher.dispatch<WindowCloseEvent>(
	    [this](Event &e) -> bool
	    {
		    running = false;
		    return true;
	    });

	for (auto it = layer_stack.end(); it != layer_stack.begin();)
	{
		--it;
		Layer &layer = *(*it);
		if (layer.is_enabled())
		{
			layer.on_event(event);
			if (event.handled)
				break;
		}
	}

	// SOL_CORE_TRACE("{}", event);
}

void Application::push_layer(Layer *layer) { layer_stack.push(layer); }

void Application::push_overlay(Layer *overlay)
{
	layer_stack.push_overlay(overlay);
}

} // namespace sol