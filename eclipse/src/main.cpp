#define SOL_ENTRY_POINT_SOURCE_FILE
#include "DockspaceLayer.h"

#include <sol.h>

namespace sol::ecl
{

class Eclipse : public Application
{
public:
	Eclipse()
	    : Application()
	{
		layer_stack.push(new DockspaceLayer());
	}
	~Eclipse() {}
};

} // namespace sol::ecl

sol::Application *sol::GetApplication() { return new sol::ecl::Eclipse(); }
