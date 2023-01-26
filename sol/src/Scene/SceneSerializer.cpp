#include "SceneSerializer.h"

#include "Entity.h"
#include "Scene/Components.h"

#include <fstream>
#include <glm/ext/scalar_constants.hpp>
#include <yaml-cpp/emitter.h>
#include <yaml-cpp/emittermanip.h>
#include <yaml-cpp/node/parse.h>
#include <yaml-cpp/yaml.h>


// forward declarations --------------------------------------------------------
YAML::Emitter &operator<<(YAML::Emitter &out, const glm::vec4 v);
YAML::Emitter &operator<<(YAML::Emitter &out, const glm::vec3 v);
YAML::Emitter &operator<<(YAML::Emitter &out,
                          const std::array<glm::vec2, 4> &a);
// end of forward declarations -------------------------------------------------

namespace sol
{

void SceneSerializer::serialize_text(const std::string &filepath)
{
	std::string scene_name = "untitled";
	SOL_CORE_INFO("serializing {}...", scene_name);

	YAML::Emitter out;
	out << YAML::BeginMap;

	out << YAML::Key << "scene" << YAML::Value << scene_name;

	out << YAML::Key << "entities" << YAML::BeginSeq;

	scene->registry.each(
	    [&](entt::entity handle)
	    {
		    Entity entity {*scene, handle};
		    if (!entity)
			    return;
		    serialize_entity(out, entity);
	    });

	out << YAML::EndSeq;

	std::ofstream file {filepath};
	file << out.c_str();

	SOL_CORE_INFO("serialization completed.");
}

bool SceneSerializer::deserialize_text(const std::string &filepath)
{
	YAML::Node data = YAML::LoadFile(filepath);

	if (!data["scene"])
		return false;

	std::string scene_name = data["scene"].as<std::string>();
	SOL_CORE_INFO("deserializing {}...", scene_name);

	YAML::Node entities = data["entities"];
	if (entities)
	{
		for (YAML::Node serialized_entity : entities)
		{
			// tag and transform components are compulsory.
			Entity entity = scene->create(
			    serialized_entity["tag component"]["tag"].as<std::string>());
			YAML::Node transform  = serialized_entity["transform component"];
			glm::vec3 translation = {transform["translation"].as<glm::vec3>()};
			glm::vec3 rotation    = {transform["rotation"].as<glm::vec3>()};
			glm::vec3 scale       = {transform["scale"].as<glm::vec3>()};
			entity.replace<cmp::Transform>(translation, rotation, scale);

			// sprite renderer component.
			if (serialized_entity["sprite renderer component"])
			{
				YAML::Node serialized_sprite_renderer =
				    serialized_entity["sprite renderer component"];
				cmp::SpriteRenderer &sprite_renderer =
				    entity.add<cmp::SpriteRenderer>();

				sprite_renderer.color =
				    serialized_sprite_renderer["tint"].as<glm::vec4>();

				if (serialized_sprite_renderer["texture"])
					// TODO(rafael): enqueue for texture loading.
					;
				else
				{
					// using std::vector because it already has serialization
					// support from the yaml-cpp library.
					std::vector coordinates =
					    serialized_sprite_renderer["texture coordinates"]
					        .as<std::vector<glm::vec2>>();
					sprite_renderer.sprite.texture_coordinates = {
					    coordinates[0],
					    coordinates[1],
					    coordinates[2],
					    coordinates[3],
					};
				}
			}

			// camera component.
			if (serialized_entity["camera component"])
			{
				YAML::Node serialized_camera =
				    serialized_entity["camera component"];
				cmp::Camera &camera = entity.add<cmp::Camera>();

				camera.primary = serialized_camera["primary"].as<bool>();
				camera.fixed_aspect_ratio =
				    serialized_camera["fixed aspect ratio"].as<bool>();

				SceneCamera &camera_data = camera.camera;
				SceneCamera::Type projection_type =
				    serialized_camera["projection type"].as<int>() == 0
				        ? SceneCamera::Type::ORTHOGRAPHIC
				        : SceneCamera::Type::PERSPECTIVE;
				camera_data.projection_type = projection_type;
				camera_data.aspect_ratio =
				    serialized_camera["aspect ratio"].as<float>();

				camera_data.perspective_vertical_field_of_view =
				    serialized_camera["perspective vertical field of view"]
				        .as<float>();
				camera_data.perspective_near =
				    serialized_camera["perspective near"].as<float>();
				camera_data.perspective_far =
				    serialized_camera["perspective far"].as<float>();

				camera_data.orthographic_size =
				    serialized_camera["orthographic size"].as<float>();
				camera_data.orthographic_near =
				    serialized_camera["orthographic near"].as<float>();
				camera_data.orthographic_far =
				    serialized_camera["orthographic far"].as<float>();
			}
		}
	}

	SOL_CORE_INFO("deserialization completed.");

	return true;
}

void SceneSerializer::serialize_binary(const std::string &filepath)
{
	SOL_CORE_ASSERT(false, "sol does not support binary serialization yet.");
}

bool SceneSerializer::deserialize_binary(const std::string &filepath)
{
	SOL_CORE_ASSERT(false, "sol does not support binary deserialization yet.");
	return false;
}

void SceneSerializer::serialize_entity(YAML::Emitter &out, Entity e)
{
	out << YAML::BeginMap;
	out << YAML::Key << "id" << static_cast<uint32_t>(e.handle);

	// tag component.
	{
		cmp::Tag &tag = e.get<cmp::Tag>();
		out << YAML::Key << "tag component" << YAML::BeginMap;
		out << YAML::Key << "tag" << tag.tag;
		out << YAML::EndMap;
	}

	// transform component.
	{
		cmp::Transform &transform = e.get<cmp::Transform>();
		out << YAML::Key << "transform component" << YAML::BeginMap;

		glm::vec3 translation = transform.translation;
		out << YAML::Key << "translation" << translation;

		glm::vec3 rotation = transform.rotation;
		out << YAML::Key << "rotation" << rotation;

		glm::vec3 scale = transform.scale;
		out << YAML::Key << "scale" << scale;

		out << YAML::EndMap;
	}

	// sprite renderer component.
	if (e.has<cmp::SpriteRenderer>())
	{
		cmp::SpriteRenderer &sprite_renderer = e.get<cmp::SpriteRenderer>();
		out << YAML::Key << "sprite renderer component" << YAML::BeginMap;

		out << YAML::Key << "tint" << sprite_renderer.color;

		// case: has texture.
		if (*sprite_renderer.sprite.get_atlas() !=
		    *Renderer2D::data.white_texture)
		{
			out << YAML::Key << "texture"
			    << "path/to/resource";
			out << YAML::Key << "texture coordinates"
			    << sprite_renderer.sprite.get_texture_coordinates();
		}

		out << YAML::EndMap;
	}

	// camera component.
	if (e.has<cmp::Camera>())
	{
		cmp::Camera &camera = e.get<cmp::Camera>();
		out << YAML::Key << "camera component" << YAML::BeginMap;

		out << YAML::Key << "primary" << camera.primary;
		out << YAML::Key << "fixed aspect ratio" << camera.fixed_aspect_ratio;

		SceneCamera &camera_data = camera.camera;
		int projection_type =
		    camera_data.projection_type == SceneCamera::Type::ORTHOGRAPHIC ? 0
		                                                                   : 1;
		out << YAML::Key << "projection type" << projection_type;
		out << YAML::Key << "aspect ratio" << camera_data.aspect_ratio;

		out << YAML::Key << "perspective vertical field of view"
		    << camera_data.perspective_vertical_field_of_view;
		out << YAML::Key << "perspective near" << camera_data.perspective_near;
		out << YAML::Key << "perspective far" << camera_data.perspective_far;

		out << YAML::Key << "orthographic size"
		    << camera_data.orthographic_size;
		out << YAML::Key << "orthographic near"
		    << camera_data.orthographic_near;
		out << YAML::Key << "orthographic far" << camera_data.orthographic_far;

		out << YAML::EndMap;
	}

	out << YAML::EndMap;
}

} // namespace sol

// implementations of (de)serialization and conversions between YAML-related
// data types and sol-unrelated (e.g. STL or glm) data types.
YAML::Emitter &operator<<(YAML::Emitter &out, const glm::vec4 v)
{
	out << YAML::Flow;
	out << YAML::BeginSeq << v.x << v.y << v.z << v.w << YAML::EndSeq;
	return out;
}

YAML::Emitter &operator<<(YAML::Emitter &out, const glm::vec3 v)
{
	out << YAML::Flow;
	out << YAML::BeginSeq << v.x << v.y << v.z << YAML::EndSeq;
	return out;
}

YAML::Emitter &operator<<(YAML::Emitter &out, const std::array<glm::vec2, 4> &a)
{
	out << YAML::Flow;
	out << YAML::BeginSeq;

	for (int i = 0; i < 4; ++i)
		out << YAML::BeginSeq << a[i].x << a[i].y << YAML::EndSeq;

	out << YAML::EndSeq;
	return out;
}

namespace YAML
{

template <>
struct convert<glm::vec2>
{
	static bool decode(const Node &node, glm::vec2 &rhs)
	{
		if (!node.IsSequence() || node.size() != 2)
			return false;

		rhs.x = node[0].as<double>();
		rhs.y = node[1].as<double>();
		return true;
	}
};

template <>
struct convert<glm::vec3>
{
	static bool decode(const Node &node, glm::vec3 &rhs)
	{
		if (!node.IsSequence() || node.size() != 3)
			return false;

		rhs.x = node[0].as<double>();
		rhs.y = node[1].as<double>();
		rhs.z = node[2].as<double>();
		return true;
	}
};

template <>
struct convert<glm::vec4>
{
	static bool decode(const Node &node, glm::vec4 &rhs)
	{
		if (!node.IsSequence() || node.size() != 4)
			return false;

		rhs.x = node[0].as<double>();
		rhs.y = node[1].as<double>();
		rhs.z = node[2].as<double>();
		rhs.w = node[3].as<double>();
		return true;
	}
};

} // namespace YAML