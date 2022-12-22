#include "Framebuffer.h"

#include "Platform/OpenGL/OpenGLFramebuffer.h"
#include "RendererAPI.h"

namespace sol
{

std::shared_ptr<Framebuffer> Framebuffer::create(Specification specification)
{
	switch (RendererAPI::get_type())
	{
	case RendererAPI::API::NONE:
	{
		SOL_CORE_ASSERT(0, "sol does not support RendererAPI::API::NONE yet");
		return nullptr;
	}
	case RendererAPI::API::OPEN_GL:
		return std::make_shared<OpenGLFramebuffer>(specification);
	}
	SOL_CORE_ASSERT(0, "RendererAPI::API not found");
	return nullptr;
}

} // namespace sol