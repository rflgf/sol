#include "Scene.h"

#include "Components.h"
#include "Entity.h"
#include "Renderer/Renderer2D.h"
#include "Scene/Components.h"
#include "Scene/ScriptableEntity.h"

#include <ranges>

namespace sol
{

Scene::Scene() {}

Scene::~Scene() {}

void Scene::on_update(Timestep dt)
{
	registry.view<cmp::NativeScriptContainer>().each(
	    [&](entt::entity entity, cmp::NativeScriptContainer &container)
	    {
		    // TODO(rafael): move this to Scene::on_play or something.
		    for (cmp::NativeScript *component : container)
		    {
			    if (!component->instance)
			    {
				    component->instance = component->instantiate_function();
				    component->instance->entity.handle       = entity;
				    component->instance->entity.owning_scene = this;

				    component->instance->on_create();
			    }

			    component->instance->on_update(dt);
		    }
	    });

	Camera *main_camera = nullptr;
	glm::mat4 camera_transform;
	{
		auto group = registry.group<cmp::Transform, cmp::Camera>();
		for (entt::entity entity : group)
		{
			auto [transform, camera] =
			    group.get<cmp::Transform, cmp::Camera>(entity);
			if (camera.primary)
			{
				main_camera      = &camera.camera;
				camera_transform = transform;
				break;
			}
		}
	}

	if (main_camera)
	{
		Renderer2D::begin_scene(main_camera->projection, camera_transform);

		auto view = registry.view<cmp::Transform, cmp::SpriteRenderer>();

		for (entt::entity entity : view)
		{
			auto [transform, sprite] =
			    view.get<cmp::Transform, cmp::SpriteRenderer>(entity);
			Renderer2D::draw_quad(transform, sprite, sprite.color);
		}

		Renderer2D::end_scene();
	}
}

void Scene::on_viewport_resize(glm::vec2 new_size)
{
	// resize non-fixed aspect ratio cameras.
	auto group = registry.group<cmp::Transform, cmp::Camera>();
	for (entt::entity entity : group)
	{
		auto [transform, camera] =
		    group.get<cmp::Transform, cmp::Camera>(entity);
		if (camera.fixed_aspect_ratio)
			camera.camera.set_viewport_size(new_size);
	}
}

Entity Scene::create(std::string name)
{
	Entity e(*this);
	e.add<cmp::Transform>(glm::vec3 {0}, glm::vec3 {0}, glm::vec3 {1});
	e.add<cmp::Tag>(name);
	return e;
}

} // namespace sol
