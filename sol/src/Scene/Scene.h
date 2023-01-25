#pragma once

// #include "Components.h"
#include "Timestep.h"

#include <entt/entt.hpp>
#include <glm/glm.hpp>

namespace sol
{

namespace ecl::pnl
{
class SceneHierarchy;
}
class Entity;

class Scene
{
public:
	entt::registry registry;

	Scene();
	~Scene();

	void on_update(Timestep dt);
	void on_viewport_resize(glm::vec2 new_size);

	Entity create(std::string name = "entity");
	void destroy(Entity entity);

	friend class SceneSerializer;
	friend class ecl::pnl::SceneHierarchy;
};

} // namespace sol
