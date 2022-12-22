#pragma once

#include "Renderer/Framebuffer.h"

namespace sol
{

class OpenGLFramebuffer : public Framebuffer
{
	uint32_t id;
	uint32_t color_attachment, depth_attachment;
	Specification specification;

public:
	OpenGLFramebuffer(Specification specification);
	virtual ~OpenGLFramebuffer();

	const Specification &get_specification() const override
	{
		return specification;
	}
	Specification &get_specification() override { return specification; }
	uint32_t get_color_attachment_renderer_id() const override
	{
		return color_attachment;
	}

	void bind() const override;
	void unbind() const override;
	void resize(glm::vec2 new_size) override;

	void invalidate();
};

} // namespace sol