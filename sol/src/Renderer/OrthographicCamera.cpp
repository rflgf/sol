#include "OrthographicCamera.h"

#include <glm/gtc/matrix_transform.hpp>

namespace sol
{

OrthographicCamera::OrthographicCamera(float top, float right, float bottom,
                                       float left, float near, float far)
    : projection_matrix(glm::ortho(left, right, bottom, top, near, far))
    , rotation(0.0f)
    , position(0.0f)
{
	update_projection_view_matrix();
}

void OrthographicCamera::update_projection_view_matrix()
{
	glm::mat4 rotation = glm::rotate(
	    glm::mat4(1.0f), glm::radians(this->rotation), glm::vec3(0, 0, 1));
	glm::mat4 translation  = glm::translate(glm::mat4(1.0f), position);
	glm::mat4 transform    = translation * rotation;
	view_matrix            = glm::inverse(transform);
	projection_view_matrix = projection_matrix * view_matrix;
}

void OrthographicCamera::set_rotation(float rotation)
{
	this->rotation = rotation;
	update_projection_view_matrix();
}

void OrthographicCamera::set_position(glm::vec3 position)
{
	this->position = position;
	update_projection_view_matrix();
}

void OrthographicCamera::set_projection(float top, float right, float bottom,
                                        float left, float near, float far)
{
	this->projection_matrix = glm::ortho(left, right, bottom, top, near, far);
	update_projection_view_matrix();
}

} // namespace sol