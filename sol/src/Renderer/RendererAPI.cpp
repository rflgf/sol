#include "RendererAPI.h"

#include "Platform/OpenGL/OpenGLRendererAPI.h"

namespace sol
{

// FIXME(rafael): set this at runtime.
RendererAPI::API RendererAPI::api_type = RendererAPI::API::OPEN_GL;

RendererAPI *RendererAPI::create()
{
	switch (RendererAPI::api_type)
	{
	case RendererAPI::API::NONE:
	{
		SOL_CORE_ASSERT(0, "sol does not support RendererAPI::API::NONE yet");
		return nullptr;
	}
	case RendererAPI::API::OPEN_GL:
		return new OpenGLRendererAPI();
	}
	SOL_CORE_ASSERT(0, "RendererAPI::API not found");
	return nullptr;
}

} // namespace sol