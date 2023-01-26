#include "SceneCamera.h"

#include "Core.h"
#include "Scene/SceneCamera.h"

#include <glm/ext/matrix_clip_space.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace sol
{

SceneCamera::SceneCamera(Type type)
    : projection_type(type)
{
	if (type == Type::ORTHOGRAPHIC)
		recalculate_orthographic_projection();
	else
		recalculate_perspective_projection();
}

// perspective-related ---------------------------------------------------------
void SceneCamera::set_perspective(float vertical_field_of_view, float near,
                                  float far)
{
	projection_type                    = Type::PERSPECTIVE;
	perspective_vertical_field_of_view = vertical_field_of_view;
	perspective_near                   = near;
	perspective_far                    = far;
	recalculate_perspective_projection();
}

void SceneCamera::set_perspective_vertical_field_of_view(
    float vertical_field_of_view)
{
	perspective_vertical_field_of_view = vertical_field_of_view;
	recalculate_perspective_projection();
}

void SceneCamera::set_perspective_near(float near)
{
	perspective_near = near;
	recalculate_perspective_projection();
}

void SceneCamera::set_perspective_far(float far)
{
	perspective_far = far;
	recalculate_perspective_projection();
}

void SceneCamera::recalculate_perspective_projection()
{
	projection =
	    glm::perspective(perspective_vertical_field_of_view, aspect_ratio,
	                     perspective_near, perspective_far);
}

float SceneCamera::get_perspective_vertical_field_of_view() const
{
	return perspective_vertical_field_of_view;
}

float SceneCamera::get_perspective_near() const { return perspective_near; }

float SceneCamera::get_perspective_far() const { return perspective_far; }
// end of perspective-related --------------------------------------------------

// orthographic-related --------------------------------------------------------
void SceneCamera::set_orthographic(float size, float near, float far)
{
	projection_type   = Type::ORTHOGRAPHIC;
	orthographic_size = size;
	orthographic_near = near;
	orthographic_far  = far;
	recalculate_orthographic_projection();
}

void SceneCamera::set_orthographic_size(float size)
{
	orthographic_size = size;
	recalculate_orthographic_projection();
}

void SceneCamera::set_orthographic_near(float near)
{
	orthographic_near = near;
	recalculate_orthographic_projection();
}

void SceneCamera::set_orthographic_far(float far)
{
	orthographic_far = far;
	recalculate_orthographic_projection();
}

void SceneCamera::recalculate_orthographic_projection()
{
	float left   = -orthographic_size * aspect_ratio * 0.5f;
	float right  = orthographic_size * aspect_ratio * 0.5f;
	float bottom = -orthographic_size * 0.5f;
	float top    = orthographic_size * 0.5f;

	projection = glm::ortho(left, right, bottom, top, orthographic_near,
	                        orthographic_far);
}

float SceneCamera::get_orthographic_size() const
{
	return perspective_vertical_field_of_view;
}

float SceneCamera::get_orthographic_near() const { return orthographic_near; }

float SceneCamera::get_orthographic_far() const { return orthographic_far; }
// end of orthographic-related -------------------------------------------------

void SceneCamera::set_viewport_size(glm::vec2 new_size)
{
	aspect_ratio =
	    static_cast<float>(new_size.x) / static_cast<float>(new_size.y);
	recalculate_projection();
}

void SceneCamera::recalculate_projection()
{
	if (projection_type == Type::ORTHOGRAPHIC)
		recalculate_orthographic_projection();
	else
		recalculate_perspective_projection();
}

void SceneCamera::set_projection_type(Type type)
{
	projection_type = type;
	if (type == Type::ORTHOGRAPHIC)
		recalculate_orthographic_projection();
	else
		recalculate_perspective_projection();
}

void SceneCamera::set_aspect_ratio(float aspect_ratio)
{
	this->aspect_ratio = aspect_ratio;
	recalculate_orthographic_projection();
}

SceneCamera::Type SceneCamera::get_projection_type() const
{
	return projection_type;
};

} // namespace sol