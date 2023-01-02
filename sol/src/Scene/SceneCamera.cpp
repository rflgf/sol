#include "SceneCamera.h"

#include <glm/gtc/matrix_transform.hpp>

namespace sol
{

SceneCamera::SceneCamera() { recalculate_projection(); }

void SceneCamera::set_orthographic_size(float size)
{
	orthographic_size = size;
	recalculate_projection();
}

void SceneCamera::set_orthographic(float size, float near, float far)
{
	orthographic_size = size;
	orthographic_near = near;
	orthographic_far  = far;
	recalculate_projection();
}

void SceneCamera::set_viewport_size(glm::vec2 new_size)
{
	aspect_ratio =
	    static_cast<float>(new_size.x) / static_cast<float>(new_size.y);
	recalculate_projection();
}

void SceneCamera::recalculate_projection()
{
	float left   = -orthographic_size * aspect_ratio * 0.5f;
	float right  = orthographic_size * aspect_ratio * 0.5f;
	float bottom = -orthographic_size * 0.5f;
	float top    = orthographic_size * 0.5f;

	projection = glm::ortho(left, right, bottom, top, orthographic_near,
	                        orthographic_far);
}

} // namespace sol