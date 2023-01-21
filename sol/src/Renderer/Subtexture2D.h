#pragma once

#include "Texture.h"

#include <array>
#include <glm/glm.hpp>

namespace sol
{

class Subtexture2D
{
	using Atlas = std::shared_ptr<Texture2D>;

public:
	Subtexture2D(Atlas atlas, const glm::vec2 top_left,
	             const glm::vec2 bottom_right);

	static Subtexture2D from_coordinates(
	    Atlas atlas, const glm::vec2 coordinates,
	    const glm::vec2 atlas_unit_dimensions,
	    const glm::vec2 subtexture_dimensions = {1, 1} /* in atlas units */);

	Atlas get_atlas() const { return atlas; }
	const std::array<const glm::vec2, 4> get_texture_coordinates() const
	{
		return texture_coordinates;
	}

private:
	Subtexture2D(Atlas atlas,
	             const std::array<const glm::vec2, 4> texture_coordinates);

	Atlas atlas;
	// TODO(rafael): maybe use smaller floats for the coordinates since they
	// don't need to be high-res anyway? this might help with keeping the
	// instacing-friendly (which is nice if we're to keep the draw_quad API
	// use exclusively subtextures).
	std::array<const glm::vec2, 4> texture_coordinates;
};

} // namespace sol