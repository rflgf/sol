#include <sol.h>

namespace example_app
{

class ExampleLayer : public sol::Layer
{
private:
	std::unique_ptr<sol::VertexArray> vao;
	std::unique_ptr<sol::Shader> shader;
	sol::OrthographicCamera camera;
	float speed;
	float rotation;
	glm::vec3 position;

public:
	ExampleLayer()
	    : Layer("example layer")
	    , camera(-0.9f, 1.6f, 0.9f, -1.6f)
	    , speed(3.0f)
	    , rotation(0.0f)
	    , position(0.0f)
	{
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

		vao = std::unique_ptr<sol::VertexArray>(sol::VertexArray::create());

		float vertices[4 * 3] = {
		    -0.5f, 0.5f,  0.0f, // bottom-left
		    0.5f,  0.5f,  0.0f, // bottom-right
		    -0.5f, -0.5f, 0.0f, // top-left
		    0.5f,  -0.5f, 0.0f, // top-right
		};
		sol::VertexBuffer *vbo =
		    sol::VertexBuffer::create(vertices, sizeof(vertices));

		using ElementType = sol::BufferElement::Type;
		sol::BufferLayout layout({{ElementType::FLOAT_3, "position"}});
		vbo->set_layout(layout);

		uint32_t indices[6]   = {2, 0, 3, 0, 3, 1};
		sol::IndexBuffer *ibo = sol::IndexBuffer::create(
		    indices, sizeof(indices) / sizeof(uint32_t));

		vao->add_vertex_buffer(std::shared_ptr<sol::VertexBuffer>(vbo));
		vao->set_index_buffer(std::shared_ptr<sol::IndexBuffer>(ibo));

		shader = std::make_unique<sol::Shader>(vert_src, frag_src);
	}

	void on_attach() override {}
	void on_detatch() override {}

	void on_update(sol::Timestep dt) override
	{
		SOL_CORE_INFO("{}ms", dt.in_seconds());

		if (sol::Input::is_mouse_button_pressed(
		        sol::MouseButtonCode::SOL_MB_LEFT))
			SOL_INFO("Left mouse button pressed!");

		if (sol::Input::is_key_pressed(sol::KeyCode::SOL_a))
			position.x -= speed * dt.in_seconds();
		if (sol::Input::is_key_pressed(sol::KeyCode::SOL_w))
			position.y += speed * dt.in_seconds();
		if (sol::Input::is_key_pressed(sol::KeyCode::SOL_d))
			position.x += speed * dt.in_seconds();
		if (sol::Input::is_key_pressed(sol::KeyCode::SOL_s))
			position.y -= speed * dt.in_seconds();

		if (sol::Input::is_key_pressed(sol::KeyCode::SOL_q))
			rotation -= speed * dt.in_seconds();
		if (sol::Input::is_key_pressed(sol::KeyCode::SOL_e))
			rotation += speed * dt.in_seconds();

		camera.set_rotation(rotation);
		camera.set_position(position);

		sol::Renderer::begin_scene(camera);

		sol::Renderer::submit(*shader, *vao);

		sol::Renderer::end_scene();
	}

	void on_event(sol::Event &e) override
	{
		if (e.get_event_type() == sol::Event::Type::KEY_PRESSED)
			SOL_INFO("{}", e);
	}

	void on_imgui_update() override
	{
		ImGui::Begin("example app");
		ImGui::ShowDemoWindow();
		ImGui::End();
	}
};

class ExampleApp : public sol::Application
{

public:
	ExampleApp()
	    : Application()
	{
		layer_stack.push(new ExampleLayer());
	}
	~ExampleApp() {}
};

} // namespace example_app

sol::Application *sol::GetApplication()
{
	return new example_app::ExampleApp();
}
