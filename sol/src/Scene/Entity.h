#pragma once

#include "Components.h"
#include "Core.h"
#include "Scene.h"

#include <entt/entt.hpp>
#include <stdint.h>

namespace sol
{

namespace ecl::pnl
{
class SceneHierarchy;
}

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

	Entity(Scene &scene, entt::entity handle)
	    : owning_scene(&scene)
	    , handle(handle)
	{
	}

public:
	Entity() = default;

	template <cmp::ComponentType T, typename... Args>
	T &add(Args &&...args)
	{
		SOL_CORE_ASSERT(!has<T>(), "entity already has component: {}.",
		                typeid(T).name());

		return owning_scene->registry.emplace<T>(handle,
		                                         std::forward<Args>(args)...);
	}

	template <cmp::ComponentType T, typename... Args>
	T &replace(Args &&...args)
	{
		SOL_CORE_ASSERT(has<T>(),
		                "entity does not have the queried component: {}.",
		                typeid(T).name());

		return owning_scene->registry.replace<T>(handle,
		                                         std::forward<Args>(args)...);
	}

	template <cmp::ComponentType T>
	bool has()
	{
		return owning_scene->registry.any_of<T>(handle);
	}

	template <cmp::ComponentType T>
	T &get()
	{
		SOL_CORE_ASSERT(has<T>(),
		                "entity does not have the queried component: {}.",
		                typeid(T).name());

		return owning_scene->registry.get<T>(handle);
	}

	template <cmp::ComponentType T>
	void remove()
	{
		SOL_CORE_ASSERT(has<T>(),
		                "entity does not have the queried component: {}.",
		                typeid(T).name());

		owning_scene->registry.erase<T>(handle);
	}

	// ---------- NativeScript-related -----------
	template <cmp::NativeScriptDerived T, typename... Args>
	cmp::NativeScript &add(Args &&...args)
	{
		std::unordered_map<std::type_index, cmp::NativeScript *> *scripts =
		    nullptr;
		if (!owning_scene->registry.any_of<cmp::NativeScriptContainer>(handle))
			scripts = &owning_scene->registry
			               .emplace<cmp::NativeScriptContainer>(handle)
			               .scripts;
		else
			scripts =
			    &owning_scene->registry.get<cmp::NativeScriptContainer>(handle)
			         .scripts;

		SOL_CORE_ASSERT(!scripts->contains(std::type_index(typeid(T))),
		                "entity already has inserted component: {}.",
		                typeid(T).name());

		return *(*scripts
		              ->emplace(
		                  std::piecewise_construct,
		                  std::forward_as_tuple(std::type_index(typeid(T))),
		                  std::forward_as_tuple(
		                      new T(std::forward<Args>(args)...)))
		              .first)
		            .second;
	}

	template <cmp::NativeScriptDerived T>
	bool has()
	{
		return owning_scene->registry.any_of<cmp::NativeScriptContainer>(
		           handle) &&
		       owning_scene->registry.get<cmp::NativeScriptContainer>(handle)
		           .scripts.contains(std::type_index(typeid(T)));
	}

	template <cmp::NativeScriptDerived T>
	T &get()
	{
		SOL_CORE_ASSERT(
		    owning_scene->registry.any_of<cmp::NativeScriptContainer>(handle) &&
		        owning_scene->registry.get<cmp::NativeScriptContainer>(handle)
		            .scripts.contains(std::type_index(typeid(T))),
		    "entity does not have the queried component: {}.",
		    typeid(T).name());

		return owning_scene->registry.get<cmp::NativeScriptContainer>(handle)
		    .scripts[std::type_index(typeid(T))];
	}

	template <cmp::NativeScriptDerived T>
	void remove()
	{
		SOL_CORE_ASSERT(
		    owning_scene->registry.any_of<cmp::NativeScriptContainer>(handle) &&
		        owning_scene->registry.get<cmp::NativeScriptContainer>(handle)
		            .scripts.contains(std::type_index(typeid(T))),
		    "entity does not have the queried component: {}.",
		    typeid(T).name());

		owning_scene->registry.get<cmp::NativeScriptContainer>(handle)
		    .scripts.erase(std::type_index(typeid(T)));
	}
	// ------ end of NativeScript-related --------

	bool operator==(const Entity &rhs) const
	{
		return handle == rhs.handle && owning_scene == rhs.owning_scene;
	}
	bool operator!=(const Entity &rhs) const { return !(*this == rhs); }
	operator bool() const { return handle != entt::null; }
	operator entt::entity() const { return handle; }
	operator entt::entity &() { return handle; }
	operator uint32_t() const { return static_cast<uint32_t>(handle); }

	friend class Scene;
	friend class SceneSerializer;
	friend class ecl::pnl::SceneHierarchy;
};

} // namespace sol