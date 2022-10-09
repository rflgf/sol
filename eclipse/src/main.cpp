#include <sol.h>

namespace ecl
{

class Eclipse : public sol::Application
{
public:
	Eclipse()
	    : Application()
	{
	}
	~Eclipse() {}
};

} // namespace ecl

sol::Application *sol::GetApplication() { return new ecl::Eclipse(); }
