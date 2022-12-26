#pragma once

#include "Renderer/Subtexture2D.h"

#include <concepts>
#include <glm/glm.hpp>
#include <typeinfo>

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

} // namespace sol::cmp