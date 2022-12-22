#include "OpenGLFramebuffer.h"

#include <glad/glad.h>

namespace sol
{

OpenGLFramebuffer::OpenGLFramebuffer(Specification specification)
    : specification(specification)
{
	invalidate();
}

OpenGLFramebuffer::~OpenGLFramebuffer()
{
	glDeleteFramebuffers(1, &id);
	glDeleteTextures(1, &color_attachment);
	glDeleteTextures(1, &depth_attachment);
}

void OpenGLFramebuffer::invalidate()
{

	if (id != 0)
	{
		glDeleteFramebuffers(1, &id);
		glDeleteTextures(1, &color_attachment);
		glDeleteTextures(1, &depth_attachment);
	}

	glCreateFramebuffers(1, &id);
	glBindFramebuffer(GL_FRAMEBUFFER, id);

	glCreateTextures(GL_TEXTURE_2D, 1, &color_attachment);
	glBindTexture(GL_TEXTURE_2D, color_attachment);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, specification.width,
	             specification.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D,
	                       color_attachment, 0);

	glCreateTextures(GL_TEXTURE_2D, 1, &depth_attachment);
	glBindTexture(GL_TEXTURE_2D, depth_attachment);
	// glTexStorage2D(GL_TEXTURE_2D, 1, depth_attachment, specification.width,
	// specification.height);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH24_STENCIL8, specification.width,
	             specification.height, 0, GL_DEPTH_STENCIL,
	             GL_UNSIGNED_INT_24_8, NULL);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT,
	                       GL_TEXTURE_2D, depth_attachment, 0);

	SOL_CORE_ASSERT(glCheckFramebufferStatus(GL_FRAMEBUFFER) ==
	                    GL_FRAMEBUFFER_COMPLETE,
	                "framebuffer is incomplete!");

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void OpenGLFramebuffer::bind() const
{
	glBindFramebuffer(GL_FRAMEBUFFER, id);
	glViewport(0, 0, specification.width, specification.height);
}

void OpenGLFramebuffer::unbind() const { glBindFramebuffer(GL_FRAMEBUFFER, 0); }

void OpenGLFramebuffer::resize(glm::vec2 new_size)
{
	specification.width  = new_size.x;
	specification.height = new_size.y;
	invalidate();
}

} // namespace sol