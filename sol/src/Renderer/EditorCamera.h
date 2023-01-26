#pragma once

#include "Event/Event.h"
#include "Renderer/Camera.h"
#include "Timestep.h"

#include <glm/glm.hpp>


namespace sol
{

class EditorCamera : public Camera
{
private:
	float field_of_view = 45.0f, aspect_ratio = 9.0f / 4.0f, near_clip = 0.1f,
	      far_clip = 500.0f;

	glm::mat4 view_matrix;
	glm::vec3 position    = {0.0f, 0.0f, 0.0f};
	glm::vec3 focal_point = {0.0f, 0.0f, 0.0f};

	glm::vec2 initial_mouse_position = {0.0f, 0.0f};

	float distance = 10.0f;
	float pitch = 0.0f, yaw = 0.0f;

	float viewport_width = 1280, viewport_height = 720;

public:
	EditorCamera() = default;
	EditorCamera(float field_of_view, float aspectRatio, float near_clip,
	             float far_clip);

	void on_update(Timestep ts);
	void on_event(Event &e);

	float get_distance() const { return distance; }
	void set_distance(float distance) { distance = distance; }

	void set_viewport_size(float width, float height)
	{
		viewport_width  = width;
		viewport_height = height;
		update_projection();
	}

	const glm::mat4 &get_view_matrix() const { return view_matrix; }
	glm::mat4 get_view_projection() const { return projection * view_matrix; }

	glm::vec3 get_up_direction() const;
	glm::vec3 get_right_direction() const;
	glm::vec3 get_forward_direction() const;
	const glm::vec3 &get_position() const { return position; }
	glm::quat get_orientation() const;

	float get_pitch() const { return pitch; }
	float get_yaw() const { return yaw; }

	void update_projection();
	void update_view();

	void mouse_pan(const glm::vec2 &delta);
	void mouse_rotate(const glm::vec2 &delta);
	void mouse_zoom(float delta);

	glm::vec3 calculate_position() const;

	std::pair<float, float> pan_speed() const;
	float rotation_speed() const;
	float zoom_speed() const;
};

} // namespace sol