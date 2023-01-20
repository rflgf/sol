#pragma once

#include "Renderer/Camera.h"

namespace sol
{

struct SceneCamera : public Camera
{
public:
	enum class Type
	{
		ORTHOGRAPHIC,
		PERSPECTIVE
	};

	float aspect_ratio   = 1.0f;
	Type projection_type = Type::ORTHOGRAPHIC;

	float perspective_vertical_field_of_view = glm::radians(45.0f);
	float perspective_near                   = 0.01f;
	float perspective_far                    = 1000.0f;

	float orthographic_size = 10.0f;
	float orthographic_near = -1.0f;
	float orthographic_far  = 1.0f;

	SceneCamera(Type type = Type::ORTHOGRAPHIC);
	virtual ~SceneCamera() = default;

	void set_vertical_field_of_view(float vertical_field_of_view);
	void set_perspective(float vertical_field_of_view, float near, float far);
	void recalculate_perspective_projection();

	void set_orthographic_size(float size);
	void set_orthographic(float size, float near, float far);
	void recalculate_orthographic_projection();

	void set_viewport_size(glm::vec2 new_size);
	void recalculate_projection();
	void set_projection_type(Type type);
};

} // namespace sol