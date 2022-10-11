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

	virtual void on_attach() override { SOL_INFO("example layer attached"); }
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
