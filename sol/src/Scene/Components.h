#pragma once

#include "Renderer/Renderer2D.h"
#include "Renderer/Subtexture2D.h"
#include "Scene/Components.h"
#include "Scene/SceneCamera.h"
#include "SceneCamera.h"
#include "Timestep.h"

#include <concepts>
#include <functional>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
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

// meant to be derived by every component class.
struct Component
{
};

template <class T>
concept ComponentType = std::is_base_of<Component, T>::value;

struct Transform : public Component
{
	glm::vec3 translation;
	glm::vec3 rotation;
	glm::vec3 scale;

	Transform()
	    : translation(0)
	    , rotation(0)
	    , scale(1)
	{
	}

	Transform(glm::vec3 translation, glm::vec3 rotation, glm::vec3 scale)
	    : translation(translation)
	    , rotation(rotation)
	    , scale(scale)
	{
	}

	operator glm::mat4()
	{
		return glm::translate(glm::mat4(1), translation) *
		       glm::rotate(glm::mat4(1), rotation.x, glm::vec3(1, 0, 0)) *
		       glm::rotate(glm::mat4(1), rotation.y, glm::vec3(0, 1, 0)) *
		       glm::rotate(glm::mat4(1), rotation.z, glm::vec3(0, 0, 1)) *
		       glm::scale(glm::mat4(1), scale);
	}
	operator const glm::mat4() const
	{
		return glm::translate(glm::mat4(1), translation) *
		       glm::rotate(glm::mat4(1), rotation.x, glm::vec3(1, 0, 0)) *
		       glm::rotate(glm::mat4(1), rotation.y, glm::vec3(0, 1, 0)) *
		       glm::rotate(glm::mat4(1), rotation.z, glm::vec3(0, 0, 1)) *
		       glm::scale(glm::mat4(1), scale);
	}
};

struct Tag : public Component
{
	static constexpr size_t MAX_SIZE = 256;
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
	Subtexture2D sprite;
	glm::vec4 color = glm::vec4 {1};

	SpriteRenderer(Subtexture2D sprite)
	    : sprite(sprite)
	{
	}

	SpriteRenderer()
	    : sprite({Renderer2D::get_white_texture(), {0, 1}, {1, 0}})
	{
	}

	operator Subtexture2D &() { return sprite; }
	operator const Subtexture2D &() const { return sprite; }
};

struct Camera : public Component
{
	SceneCamera camera;
	bool primary            = true;
	bool fixed_aspect_ratio = false;

	Camera(bool set_primary = true, bool fixed_aspect_ratio = false,
	       SceneCamera::Type type = SceneCamera::Type::ORTHOGRAPHIC)
	    : camera(type)
	    , primary(set_primary)
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