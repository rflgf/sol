#pragma once

#include "Renderer/Subtexture2D.h"
#include "SceneCamera.h"
#include "Timestep.h"

#include <concepts>
#include <functional>
#include <glm/glm.hpp>
#include <memory>
#include <ranges>
#include <type_traits>
#include <typeindex>
#include <typeinfo>
#include <unordered_map>

namespace sol
{
class Entity;
class ScriptableEntity;
} // namespace sol

namespace sol::cmp
{

struct Component
{
};

template <class T>
concept ComponentType = std::is_base_of<Component, T>::value;

struct Transform : public Component
{
	Transform(glm::mat4 transform)
	    : transform(transform)
	{
	}
	glm::mat4 transform;

	operator glm::mat4 &() { return transform; }
	operator const glm::mat4 &() const { return transform; }
};

struct Tag : public Component
{
	std::string tag;

	Tag()           = default;
	Tag(Tag &other) = default;
	Tag(std::string tag)
	    : tag(tag)
	{
	}
};

struct SpriteRenderer : public Component
{
	SpriteRenderer(std::shared_ptr<const Subtexture2D> sprite)
	    : sprite(sprite)
	{
	}
	std::shared_ptr<const Subtexture2D> sprite;

	operator std::shared_ptr<const Subtexture2D> &() { return sprite; }
	operator const std::shared_ptr<const Subtexture2D> &() const
	{
		return sprite;
	}
};

struct Camera : public Component
{
	SceneCamera camera;
	bool primary            = false;
	bool fixed_aspect_ratio = false;

	Camera(bool set_primary = true, bool fixed_aspect_ratio = false)
	    : primary(set_primary)
	    , fixed_aspect_ratio(fixed_aspect_ratio)
	{
	}

	Camera(const Camera &camera) = default;
};

template <class T>
concept Bindable = std::is_base_of<ScriptableEntity, T>::value;

struct NativeScript : public Component
{
	ScriptableEntity *instance = nullptr;

	ScriptableEntity *(*instantiate_function)();
	void (*deinstantiate_function)(NativeScript *);

	template <Bindable T, typename... Args>
	void bind(Args &&...args)
	{
		instantiate_function = [args...]() {
			return static_cast<ScriptableEntity *>(
			    new T(std::forward<Args>(args)...));
		};
		deinstantiate_function = [](NativeScript *component)
		{
			delete component->instance;
			component->instance = nullptr;
		};
	}
};

template <class T>
concept NativeScriptDerived =
    ComponentType<T> && std::convertible_to<T *, NativeScript *>;

// NOTE(rafael): this is the actual component that is forwarded to the Scene's
// registry and which each entity only has one of. it holds and manages
// NativeScript objects, of which there can be multiple per entity.
struct NativeScriptContainer : public Component
{
	std::unordered_map<std::type_index, NativeScript *> scripts {};

	auto begin() { return (scripts | std::views::values).begin(); }
	auto end() { return (scripts | std::views::values).end(); }
};

} // namespace sol::cmp