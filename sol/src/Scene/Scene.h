#pragma once

// #include "Components.h"
#include "Timestep.h"

#include <entt/entt.hpp>
#include <glm/glm.hpp>

namespace sol
{

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

	// template <cmp::ComponentType T>
};

} // namespace sol
