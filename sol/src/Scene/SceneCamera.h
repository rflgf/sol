#pragma once

#include "Renderer/Camera.h"
#include "Scene/SceneSerializer.h"

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

private:
	Type projection_type = Type::ORTHOGRAPHIC;
	float aspect_ratio   = 1.0f;

	float perspective_vertical_field_of_view = glm::radians(45.0f);
	float perspective_near                   = 0.01f;
	float perspective_far                    = 1000.0f;

	float orthographic_size = 10.0f;
	float orthographic_near = -1.0f;
	float orthographic_far  = 1.0f;

public:
	SceneCamera(Type type = Type::ORTHOGRAPHIC);
	virtual ~SceneCamera() = default;

	void set_perspective(float vertical_field_of_view, float near, float far);
	void set_perspective_vertical_field_of_view(float vertical_field_of_view);
	void set_perspective_near(float perspective_near);
	void set_perspective_far(float perspective_far);
	void recalculate_perspective_projection();
	float get_perspective_vertical_field_of_view() const;
	float get_perspective_near() const;
	float get_perspective_far() const;

	void set_orthographic(float size, float near, float far);
	void set_orthographic_size(float size);
	void set_orthographic_near(float orthographic_near);
	void set_orthographic_far(float ortohgraphic_far);
	void recalculate_orthographic_projection();
	float get_orthographic_size() const;
	float get_orthographic_near() const;
	float get_orthographic_far() const;

	void set_viewport_size(glm::vec2 new_size);
	void recalculate_projection();
	void set_projection_type(Type type);
	void set_aspect_ratio(float aspect_ratio);
	Type get_projection_type() const;

	friend class SceneSerializer;
};

} // namespace sol