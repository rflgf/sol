#pragma once

#include <glm/glm.hpp>

namespace sol
{

struct OrthographicCamera
{
public:
	glm::vec3 position;
	float rotation;

	glm::mat4 projection_matrix;
	glm::mat4 view_matrix;
	glm::mat4 projection_view_matrix;

	OrthographicCamera(float top, float right, float bottom, float left,
	                   float near = -1.0f, float far = 1.0f);

	void update_projection_view_matrix();

	void set_rotation(float rotation);
	void set_position(glm::vec3 position);
};

} // namespace sol