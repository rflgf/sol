#pragma once

#include "Components.h"
#include "Core.h"
#include "Scene.h"

#include <entt/entt.hpp>

namespace sol
{

struct Entity
{
private:
	entt::entity handle = entt::null;
	Scene *owning_scene = nullptr;

	Entity(Scene &owning_scene)
	    : handle(owning_scene.registry.create())
	    , owning_scene(&owning_scene)
	{
	}

public:
	template <cmp::ComponentType T, typename... Args> T &add(Args &&...args)
	{
		SOL_CORE_ASSERT(!has<T>(), "entity already has component.");

		return owning_scene->registry.emplace<T>(handle,
		                                         std::forward<Args>(args)...);
	}

	template <cmp::ComponentType T, typename... Args> T &replace(Args &&...args)
	{
		SOL_CORE_ASSERT(has<T>(),
		                "entity does not have the queried component.");

		return owning_scene->registry.replace<T>(handle,
		                                         std::forward<Args>(args)...);
	}

	template <cmp::ComponentType T> bool has()
	{
		// return owning_scene->registry.has<T>(handle);
		return owning_scene->registry.any_of<T>(handle);
	}

	template <cmp::ComponentType T> T &get()
	{
		SOL_CORE_ASSERT(has<T>(),
		                "entity does not have the queried component.");

		return owning_scene.registry.get<T>(handle);
	}

	template <cmp::ComponentType T> void remove()
	{
		SOL_CORE_ASSERT(has<T>(),
		                "entity does not have the queried component.");

		owning_scene.registry.erase<T>(handle);
	}

	operator bool() const { return handle != entt::null; }

	friend class Scene;
};

} // namespace sol