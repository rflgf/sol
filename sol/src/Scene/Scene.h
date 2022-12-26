#pragma once

#include "Components.h"
#include "Timestep.h"

#include <entt/entt.hpp>

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

	Entity create(std::string name = "entity");

	// template <cmp::ComponentType T>
};

} // namespace sol
