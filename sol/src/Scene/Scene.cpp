#include "Scene.h"

#include "Components.h"
#include "Entity.h"
#include "Renderer/Renderer2D.h"

namespace sol
{

Scene::Scene() {}

Scene::~Scene() {}

void Scene::on_update(Timestep dt)
{
	auto group = registry.group<cmp::Transform>(entt::get<cmp::SpriteRenderer>);

	for (auto entity : group)
	{
		auto [transform, sprite] =
		    group.get<cmp::Transform, cmp::SpriteRenderer>(entity);
		Renderer2D::draw_quad(transform, sprite);
	}
}

Entity Scene::create(std::string name)
{
	Entity e(*this);

	e.add<cmp::Transform>(glm::mat4 {1});
	e.add<cmp::Tag>(name);
	return e;
}

} // namespace sol
