#include "CameraController.h"

#include "Application.h"
#include "Event/ApplicationEvent.h"
#include "Event/MouseEvent.h"
#include "Input.h"
#include "Renderer/RenderCommand.h"

namespace sol
{

CameraController::CameraController(float aspect_ratio)
    : aspect_ratio(aspect_ratio)
    , zoom(zoom_max)
    , camera(-zoom, aspect_ratio * zoom, zoom, -aspect_ratio * zoom)
    , rotation_speed(20.0f)
    , translation_speed(5.0f)
    , rotation(0.0f)
    , position(0.0f, 0.0f, 0.0f)
{
}

void CameraController::on_event(Event &event)
{
	Event::Dispatcher dispatcher(event);

	dispatcher.dispatch<WindowResizeEvent>(
	    [this](WindowResizeEvent &e) -> bool
	    {
		    on_resize(e.get_width(), e.get_height());
		    RenderCommand::set_viewport(0, 0, e.get_width(), e.get_height());

		    return false;
	    });

	dispatcher.dispatch<MouseScrolledEvent>(
	    [this](MouseScrolledEvent &e) -> bool
	    {
		    zoom -= e.get_y_offset() * zoom_factor;
		    zoom = std::clamp(zoom, zoom_min, zoom_max);
		    camera.set_projection(-zoom, aspect_ratio * zoom, zoom,
		                          -aspect_ratio * zoom);
		    return false;
	    });
}

void CameraController::on_update(Timestep dt)
{
	if (Input::is_key_pressed(sol::KeyCode::SOL_a))
		position.x -= translation_speed * dt.in_seconds();
	if (Input::is_key_pressed(sol::KeyCode::SOL_w))
		position.y -= translation_speed * dt.in_seconds();
	if (sol::Input::is_key_pressed(sol::KeyCode::SOL_d))
		position.x += translation_speed * dt.in_seconds();
	if (Input::is_key_pressed(sol::KeyCode::SOL_s))
		position.y += translation_speed * dt.in_seconds();

	if (Input::is_key_pressed(sol::KeyCode::SOL_q))
		rotation -= rotation_speed * dt.in_seconds();
	if (Input::is_key_pressed(sol::KeyCode::SOL_e))
		rotation += rotation_speed * dt.in_seconds();

	camera.set_rotation(rotation);
	camera.set_position(position);

	translation_speed = zoom;
}

void CameraController::on_resize(uint32_t width, uint32_t height)
{
	aspect_ratio = static_cast<float>(width) / static_cast<float>(height);
	camera.set_projection(-zoom, aspect_ratio * zoom, zoom,
	                      -aspect_ratio * zoom);
}

} // namespace sol