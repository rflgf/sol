#pragma once

#include <glm/glm.hpp>

namespace sol
{

struct Camera
{
protected:
	glm::mat4 projection = glm::mat4(1.0f);

public:
	Camera() = default;
	Camera(glm::mat4 projection)
	    : projection(projection)
	{
	}
	virtual ~Camera() = default;

	glm::mat4 &get_projection_matrix() { return projection; }
	glm::mat4 get_projection_matrix() const { return projection; }
};

} // namespace sol