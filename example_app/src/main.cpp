#include <SDL.h>
#include <sol.h>

namespace example_app
{

class ExampleLayer : public sol::Layer
{
public:
	ExampleLayer()
	    : Layer("example layer")
	{
	}

	virtual void on_attach() override {}
	virtual void on_detatch() override {}
	virtual void on_update() override
	{
		if (sol::Input::is_mouse_button_pressed(
		        sol::MouseButtonCode::SOL_MB_LEFT))
			SOL_INFO("Left mouse button pressed!");
	}
	virtual void on_event(sol::Event &e) override
	{
		if (e.get_event_type() == sol::Event::Type::KEY_PRESSED)
			SOL_INFO("{}", e);
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
