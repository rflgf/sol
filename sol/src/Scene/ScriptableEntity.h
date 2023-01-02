#pragma once

#include "Entity.h"

namespace sol
{

class Scene;

// NOTE(rafael): atm this is a mem. leak since we never call
// ScriptableEntity::on_destroy, so do that once we have proper scene
// deinitialization.
class ScriptableEntity
{
	Entity entity;

public:
	virtual ~ScriptableEntity() {}

	virtual void on_create() {}
	virtual void on_destroy() {}
	virtual void on_update(Timestep dt) {}

	template <cmp::ComponentType T, typename... Args>
	T &add(Args &&...args)
	{
		return entity.add<T>(std::forward<Args>(args)...);
	}

	template <cmp::ComponentType T, typename... Args>
	T &replace(Args &&...args)
	{
		return entity.replace<T>(std::forward<Args>(args)...);
	}

	template <cmp::ComponentType T>
	bool has()
	{
		return entity.has<T>();
	}

	template <cmp::ComponentType T>
	T &get()
	{
		return entity.get<T>();
	}

	template <cmp::ComponentType T>
	void remove()
	{
		return entity.remove<T>();
	}

	friend class Scene;
};

} // namespace sol