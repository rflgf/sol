#include "Application.h"

#include "Event/ApplicationEvent.h"
#include "Event/Event.h"
#include "Event/KeyboardEvent.h"
#include "Event/MouseEvent.h"
#include "ImGui/ImGuiLayer.h"

namespace sol
{
Application::Application()
    : camera(-0.9f, 1.6f, 0.9f, -1.6f)
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
		uniform mat4 view_projection;
		out vec3 position;
		void main() { gl_Position = view_projection * vec4(a_Position, 1.0); 
		position = a_Position; }
	)";
	const std::string frag_src = R"(
		#version 330 core
		layout(location = 0) out vec4 color;
		in vec3 position;
		void main() { color = vec4(position * 0.5 + 0.5, 1); }
	)";

	vao = std::unique_ptr<VertexArray>(VertexArray::create());

	float vertices[4 * 3] = {
	    -0.5f, 0.5f,  0.0f, // bottom-left
	    0.5f,  0.5f,  0.0f, // bottom-right
	    -0.5f, -0.5f, 0.0f, // top-left
	    0.5f,  -0.5f, 0.0f, // top-right
	};
	VertexBuffer *vbo = VertexBuffer::create(vertices, sizeof(vertices));

	using ElementType = BufferElement::Type;
	BufferLayout layout({{ElementType::FLOAT_3, "position"}});
	vbo->set_layout(layout);

	uint32_t indices[6] = {2, 0, 3, 0, 3, 1};
	IndexBuffer *ibo =
	    IndexBuffer::create(indices, sizeof(indices) / sizeof(uint32_t));

	vao->add_vertex_buffer(std::shared_ptr<VertexBuffer>(vbo));
	vao->set_index_buffer(std::shared_ptr<IndexBuffer>(ibo));

	shader = std::make_unique<Shader>(vert_src, frag_src);

	imgui_layer.begin();
}

Application::~Application() { imgui_layer.end(); }

void Application::run()
{
	float i = 0.0f;
	while (running)
	{
		RenderCommand::set_clear_color({0.3f, 0.6f, 0.9f, 1.0f});
		RenderCommand::clear();

		Renderer::begin_scene(camera);

		camera.set_rotation(i += 0.2f);
	
		Renderer::submit(*shader, *vao);

		Renderer::end_scene();

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