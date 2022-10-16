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
	virtual void on_update() override {}
	virtual void on_event(sol::Event &e) override {}
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
