#include <glm/gtc/type_ptr.hpp>
#include <imgui.h>
#include <sol.h>

namespace example_app
{

class ExampleLayer : public sol::Layer
{
private:
	std::unique_ptr<sol::VertexArray> vao;
	std::unique_ptr<sol::Shader> shader;
	std::shared_ptr<sol::Texture> texture;
	sol::OrthographicCamera camera;
	float speed;
	float rotation;
	glm::vec3 position;
	glm::vec3 color;

public:
	ExampleLayer()
	    : Layer("example layer")
	    , camera(-0.9f, 1.6f, 0.9f, -1.6f)
	    , speed(3.0f)
	    , rotation(0.0f)
	    , position(0.0f)
	{
		vao = std::unique_ptr<sol::VertexArray>(sol::VertexArray::create());

		float vertices[4 * (3 + 2)] = {
		    // clang-format off
			/* world coord. */   /* texture coord. */
		    -0.5f,  0.5f, 0.0f,    0.0f, 0.0f,         // bottom-left
		     0.5f,  0.5f, 0.0f,    1.0f, 0.0f,         // bottom-right
		    -0.5f, -0.5f, 0.0f,    0.0f, 1.0f,         // top-left
		     0.5f, -0.5f, 0.0f,    1.0f, 1.0f,         // top-right
		    // clang-format on
		};
		sol::VertexBuffer *vbo =
		    sol::VertexBuffer::create(vertices, sizeof(vertices));

		using ElementType = sol::BufferElement::Type;
		sol::BufferLayout layout({{ElementType::FLOAT_3, "position"},
		                          {ElementType::FLOAT_2, "tex_coord"}});
		vbo->set_layout(layout);

		uint32_t indices[6]   = {2, 0, 3, 0, 3, 1};
		sol::IndexBuffer *ibo = sol::IndexBuffer::create(
		    indices, sizeof(indices) / sizeof(uint32_t));

		vao->add_vertex_buffer(std::shared_ptr<sol::VertexBuffer>(vbo));
		vao->set_index_buffer(std::shared_ptr<sol::IndexBuffer>(ibo));

		shader = std::unique_ptr<sol::Shader>(
		    sol::Shader::create("assets/shaders/texture.shader"));
		texture = sol::Texture2D::create("assets/test.png");
		shader->bind();
		shader->upload_uniform_int("tex_id", 0);
	}

	void on_attach() override {}
	void on_detatch() override {}
	void on_event(sol::Event &e) override {}

	void on_update(sol::Timestep dt) override
	{
		// SOL_CORE_INFO("{}ms", dt.in_seconds());

		// if (sol::Input::is_mouse_button_pressed(
		//         sol::MouseButtonCode::SOL_MB_LEFT))
		// 	SOL_INFO("Left mouse button pressed!");

		if (sol::Input::is_key_pressed(sol::KeyCode::SOL_a))
			position.x -= speed * dt.in_seconds();
		if (sol::Input::is_key_pressed(sol::KeyCode::SOL_w))
			position.y -= speed * dt.in_seconds();
		if (sol::Input::is_key_pressed(sol::KeyCode::SOL_d))
			position.x += speed * dt.in_seconds();
		if (sol::Input::is_key_pressed(sol::KeyCode::SOL_s))
			position.y += speed * dt.in_seconds();

		if (sol::Input::is_key_pressed(sol::KeyCode::SOL_q))
			rotation -= speed * dt.in_seconds();
		if (sol::Input::is_key_pressed(sol::KeyCode::SOL_e))
			rotation += speed * dt.in_seconds();

		glm::mat4 transform =
		    glm::translate(glm::mat4(1.0f), position) *
		    glm::rotate(glm::mat4(1.0f), rotation, glm::vec3(0.0f, 0.0f, 1.0f));

		sol::Renderer::begin_scene(camera);

		texture->bind();
		sol::Renderer::submit(*shader, *vao, transform);

		sol::Renderer::end_scene();
	}

	void on_imgui_update() override
	{
		ImGui::Begin("example app");
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
