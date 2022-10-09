#include <sol.h>

namespace example_app
{

class ExampleApp : public sol::Application
{
public:
	ExampleApp()
	    : Application()
	{
	}
	~ExampleApp() {}
};

} // namespace example_app

sol::Application *sol::GetApplication()
{
	return new example_app::ExampleApp();
}
