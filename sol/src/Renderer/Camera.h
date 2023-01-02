#pragma once

#include <glm/glm.hpp>

namespace sol
{

struct Camera
{
	glm::mat4 projection = glm::mat4(1);

	Camera() = default;
	Camera(glm::mat4 projection)
	    : projection(projection)
	{
	}
	virtual ~Camera() = default;
};

} // namespace sol