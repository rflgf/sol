#pragma once

#include "Renderer/Camera.h"

namespace sol
{

struct SceneCamera : public Camera
{
public:
	float aspect_ratio      = 1.0f;
	float orthographic_size = 10.0f;
	float orthographic_near = -1.0f;
	float orthographic_far  = 1.0f;

	SceneCamera();
	virtual ~SceneCamera() = default;

	void set_orthographic_size(float size);
	void set_orthographic(float size, float near, float far);
	void set_viewport_size(glm::vec2 new_size);
	void recalculate_projection();
};

} // namespace sol