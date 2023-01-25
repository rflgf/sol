#include "Subtexture2D.h"

namespace sol
{

std::array<glm::vec2, 4> map_coordinates(const glm::vec2 atlas_dimensions,
                                         const glm::vec2 bottom_left,
                                         const glm::vec2 top_right)
{
	return std::array<glm::vec2, 4> {
	    glm::vec2 {bottom_left.x / atlas_dimensions.x,
	               bottom_left.y / atlas_dimensions.y}, // bottom-left
	    glm::vec2 {bottom_left.x / atlas_dimensions.x,
	               top_right.y / atlas_dimensions.y}, // top-left
	    glm::vec2 {top_right.x / atlas_dimensions.x,
	               top_right.y / atlas_dimensions.y}, // top-right
	    glm::vec2 {top_right.x / atlas_dimensions.x,
	               bottom_left.y / atlas_dimensions.y}}; // bottom-right
}

Subtexture2D::Subtexture2D(Atlas atlas, const glm::vec2 top_left,
                           const glm::vec2 bottom_right)
    : atlas(atlas)
    , texture_coordinates(map_coordinates(
          {atlas->get_width(), atlas->get_width()}, top_left, bottom_right))
{
}

Subtexture2D::Subtexture2D(Atlas atlas,
                           std::array<glm::vec2, 4> texture_coordinates)
    : atlas(atlas)
    , texture_coordinates(texture_coordinates)
{
}

Subtexture2D
Subtexture2D::from_coordinates(Atlas atlas, const glm::vec2 coordinates,
                               const glm::vec2 atlas_unit_dimensions,
                               const glm::vec2 subtexture_dimensions)
{
	/*
	 * texture coordinates:
	 *   0,1______1,1
	 *     |      |
	 *     |      |
	 *   0,0______1,0
	 */

	glm::vec2 offset {atlas_unit_dimensions.x / atlas.get()->get_width(),
	                  atlas_unit_dimensions.y / atlas.get()->get_height()};

	glm::vec2 top_right {offset.x * (coordinates.x + subtexture_dimensions.x),
	                     offset.y * (coordinates.y + subtexture_dimensions.y)};
	glm::vec2 bottom_left {offset.x * coordinates.x, offset.y * coordinates.y};

	std::array<glm::vec2, 4> texture_coordinates = {
	    bottom_left,                            // bottom-left
	    glm::vec2 {bottom_left.x, top_right.y}, // top-left
	    top_right,                              // top-right
	    glm::vec2 {top_right.x, bottom_left.y}, // bottom-right
	};

	return {atlas, texture_coordinates};
}

} // namespace sol