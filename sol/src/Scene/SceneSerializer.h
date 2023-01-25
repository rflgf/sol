#pragma once

#include "Scene.h"

#include <yaml-cpp/yaml.h>

namespace sol
{

class SceneSerializer
{
private:
	std::shared_ptr<Scene> scene;

public:
	SceneSerializer(std::shared_ptr<Scene> scene)
	    : scene(scene)
	{
	}

	void serialize_text(const std::string &filepath);
	bool deserialize_text(const std::string &filepath);

	void serialize_binary(const std::string &filepath);
	bool deserialize_binary(const std::string &filepath);

	void serialize_entity(YAML::Emitter &, Entity e);
};

} // namespace sol