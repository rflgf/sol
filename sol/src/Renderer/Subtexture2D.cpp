#include "Subtexture2D.h"

namespace sol
{

std::array<const glm::vec2, 4> map_coordinates(const glm::vec2 atlas_dimensions,
                                               const glm::vec2 bottom_left,
                                               const glm::vec2 top_right)
{
	return std::array<const glm::vec2, 4> {
	    glm::vec2 {bottom_left.x / atlas_dimensions.x,
	               bottom_left.y / atlas_dimensions.y}, // bottom-left
	    glm::vec2 {top_right.x / atlas_dimensions.x,
	               bottom_left.y / atlas_dimensions.y}, // bottom-right
	    glm::vec2 {top_right.x / atlas_dimensions.x,
	               top_right.y / atlas_dimensions.y}, // top-right
	    glm::vec2 {bottom_left.x / atlas_dimensions.x,
	               top_right.y / atlas_dimensions.y}}; // top-left
}

Subtexture2D::Subtexture2D(Atlas atlas, const glm::vec2 top_left,
                           const glm::vec2 bottom_right)
    : atlas(atlas)
    , texture_coordinates(
          map_coordinates({atlas.get()->get_width(), atlas.get()->get_width()},
                          top_left, bottom_right))
{
}

Subtexture2D::Subtexture2D(
    Atlas atlas, const std::array<const glm::vec2, 4> texture_coordinates)
    : atlas(atlas)
    , texture_coordinates(texture_coordinates)
{
}

std::shared_ptr<Subtexture2D>
Subtexture2D::from_coordinates(Atlas atlas, const glm::vec2 index,
                               const glm::vec2 atlas_unit_dimensions,
                               const glm::vec2 subtexture_dimensions)
{
	glm::vec2 offset {atlas_unit_dimensions.x / atlas.get()->get_width(),
	                  atlas_unit_dimensions.y / atlas.get()->get_height()};

	glm::vec2 top_right {offset.x * (index.x + subtexture_dimensions.x),
	                     offset.y * (index.y + subtexture_dimensions.y)};
	glm::vec2 bottom_left {offset.x * index.x, offset.y * index.y};

	std::array<const glm::vec2, 4> texture_coordinates = {
	    bottom_left,                            // bottom-left
	    glm::vec2 {top_right.x, bottom_left.y}, // bottom-right
	    top_right,                              // top-right
	    glm::vec2 {bottom_left.x, top_right.y}  // top-left
	};

	// we're constructing a Subtexture2D from a private constructor, this is
	// just a hacky way to give std::make_shared access to that constructor.
	class MakeSharedEnabler : public Subtexture2D
	{
	public:
		MakeSharedEnabler(
		    Atlas atlas,
		    const std::array<const glm::vec2, 4> texture_coordinates)
		    : Subtexture2D(atlas, texture_coordinates)
		{
		}
	};

	return std::make_shared<MakeSharedEnabler>(atlas, texture_coordinates);
}

} // namespace sol