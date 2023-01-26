#include "EditorCamera.h"

#include "Event/MouseEvent.h"
#include "Input.h"
#include "KeyCodes.h"

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>

namespace sol
{

EditorCamera::EditorCamera(float field_of_view, float aspect_ratio,
                           float near_clip, float far_clip)
    : field_of_view(field_of_view)
    , aspect_ratio(aspect_ratio)
    , near_clip(near_clip)
    , far_clip(far_clip)
    , Camera(glm::perspective(glm::radians(field_of_view), aspect_ratio,
                              near_clip, far_clip))
{
	update_view();
}

void EditorCamera::update_projection()
{
	aspect_ratio = viewport_width / viewport_height;
	projection   = {glm::perspective(glm::radians(field_of_view), aspect_ratio,
	                                 near_clip, far_clip)};
}

void EditorCamera::update_view()
{
	// yaw = pitch = 0.0f;
	// lock the camera's rotation
	position = calculate_position();

	glm::quat orientation = get_orientation();
	view_matrix =
	    glm::translate(glm::mat4(1.0f), position) * glm::toMat4(orientation);
	view_matrix = glm::inverse(view_matrix);
}

std::pair<float, float> EditorCamera::pan_speed() const
{
	// max = 2.4f
	float x        = std::min(viewport_width / 1000.0f, 2.4f);
	float x_factor = 0.0366f * (x * x) - 0.1778f * x + 0.3021f;

	// max = 2.4f
	float y        = std::min(viewport_height / 1000.0f, 2.4f);
	float y_factor = 0.0366f * (y * y) - 0.1778f * y + 0.3021f;

	return {x_factor, y_factor};
}

float EditorCamera::rotation_speed() const { return 0.8f; }

float EditorCamera::zoom_speed() const
{
	float distance = this->distance * 0.2f;
	distance       = std::max(distance, 0.0f);
	float speed    = distance * distance;
	// max speed = 100
	speed = std::min(speed, 100.0f);
	return speed;
}

void EditorCamera::on_update(Timestep ts)
{
	if (Input::is_key_pressed(KeyCode::SOL_LALT))
	{
		const glm::vec2 &mouse {Input::get_mouse_x(), Input::get_mouse_x()};
		glm::vec2 delta        = (mouse - initial_mouse_position) * 0.003f;
		initial_mouse_position = mouse;

		if (Input::is_mouse_button_pressed(MouseButtonCode::SOL_MB_MIDDLE))
			mouse_pan(delta);
		else if (Input::is_mouse_button_pressed(MouseButtonCode::SOL_MB_LEFT))
			mouse_rotate(delta);
		else if (Input::is_mouse_button_pressed(MouseButtonCode::SOL_MB_RIGHT))
			mouse_zoom(delta.y);
	}

	update_view();
}

void EditorCamera::on_event(Event &e)
{
	Event::Dispatcher dispatcher(e);
	dispatcher.dispatch<MouseScrolledEvent>(
	    [this](MouseScrolledEvent &e) -> bool
	    {
		    float delta = e.get_y_offset() * 0.1f;
		    mouse_zoom(delta);
		    update_view();
		    return false;
	    });
}

void EditorCamera::mouse_pan(const glm::vec2 &delta)
{
	auto [xSpeed, ySpeed] = pan_speed();
	focal_point += -get_right_direction() * delta.x * xSpeed * distance;
	focal_point += get_up_direction() * delta.y * ySpeed * distance;
}

void EditorCamera::mouse_rotate(const glm::vec2 &delta)
{
	float yawSign = get_up_direction().y < 0 ? -1.0f : 1.0f;
	yaw += yawSign * delta.x * rotation_speed();
	pitch += delta.y * rotation_speed();
}

void EditorCamera::mouse_zoom(float delta)
{
	distance -= delta * zoom_speed();
	if (distance < 1.0f)
	{
		focal_point += get_forward_direction();
		distance = 1.0f;
	}
}

glm::vec3 EditorCamera::get_up_direction() const
{
	return glm::rotate(get_orientation(), glm::vec3(0.0f, 1.0f, 0.0f));
}

glm::vec3 EditorCamera::get_right_direction() const
{
	return glm::rotate(get_orientation(), glm::vec3(1.0f, 0.0f, 0.0f));
}

glm::vec3 EditorCamera::get_forward_direction() const
{
	return glm::rotate(get_orientation(), glm::vec3(0.0f, 0.0f, -1.0f));
}

glm::vec3 EditorCamera::calculate_position() const
{
	return focal_point - get_forward_direction() * distance;
}

glm::quat EditorCamera::get_orientation() const
{
	return glm::quat(glm::vec3(-pitch, -yaw, 0.0f));
}

} // namespace sol