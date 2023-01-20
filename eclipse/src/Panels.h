#pragma once

#include <sol.h>

namespace sol::ecl::pnl
{

class SceneHierarchy
{
public:
	Scene *context = nullptr;
	Entity selection_context;

	void on_imgui_update();
	void draw_entity_node(Entity entity);
	void draw_entity_properties(Entity entity);
};

} // namespace sol::ecl::pnl