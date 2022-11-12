#pragma once

#include "Event/Event.h"
#include "Renderer/OrthographicCamera.h"
#include "Timestep.h"

namespace sol
{

class CameraController
{
	float aspect_ratio;
	float zoom;
	float translation_speed;
	float rotation_speed;
	float rotation;
	glm::vec3 position;
	OrthographicCamera camera;

	static constexpr float zoom_factor = 0.03f;
	static constexpr float zoom_max    = 1.0f;
	static constexpr float zoom_min    = 0.1f;

public:
	CameraController(float aspect_ratio);

	void on_event(Event &e);
	void on_update(Timestep dt);

	const OrthographicCamera &get_camera() { return camera; }
};

} // namespace sol