#pragma once

#include "Core.h"

#include <glm/glm.hpp>

namespace sol
{

class Framebuffer
{
public:
	struct Specification
	{
		uint32_t width, height;
		uint32_t samples       = 1;
		bool swap_chain_target = false;
	};

public:
	virtual ~Framebuffer();
	static std::shared_ptr<Framebuffer> create(Specification specification);

	virtual const Specification &get_specification() const    = 0;
	virtual Specification &get_specification()                = 0;
	virtual uint32_t get_color_attachment_renderer_id() const = 0;

	virtual void bind() const               = 0;
	virtual void unbind() const             = 0;
	virtual void resize(glm::vec2 new_size) = 0;
};

} // namespace sol