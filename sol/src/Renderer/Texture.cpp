#include "Texture.h"

#include "Platform/OpenGL/OpenGLTexture.h"
#include "Renderer.h"

namespace sol
{

std::shared_ptr<Texture2D> Texture2D::create(const std::string &path)
{
	switch (Renderer::get_api_type())
	{
	case RendererAPI::API::NONE:
	{
		SOL_CORE_ASSERT(0, "sol does not support RendererAPI::API::NONE yet");
		return nullptr;
	}
	case RendererAPI::API::OPEN_GL:
		return std::make_shared<OpenGLTexture2D>(path);
	}
	SOL_CORE_ASSERT(0, "RendererAPI::API not found");
	return nullptr;
}

std::shared_ptr<Texture2D> Texture2D::create(const uint32_t width,
                                             const uint32_t height)
{
	switch (Renderer::get_api_type())
	{
	case RendererAPI::API::NONE:
	{
		SOL_CORE_ASSERT(0, "sol does not support RendererAPI::API::NONE yet");
		return nullptr;
	}
	case RendererAPI::API::OPEN_GL:
		return std::make_shared<OpenGLTexture2D>(width, height);
	}
	SOL_CORE_ASSERT(0, "RendererAPI::API not found");
	return nullptr;
}

} // namespace sol