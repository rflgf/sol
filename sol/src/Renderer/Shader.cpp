#include "Shader.h"

#include "Platform/OpenGL/OpenGLShader.h"
#include "Renderer/RendererAPI.h"

namespace sol
{
Shader *Shader::create(const std::string &vertex_source,
                       const std::string &fragment_source)
{
	switch (RendererAPI::get_type())
	{
	case RendererAPI::API::NONE:
	{
		SOL_CORE_ASSERT(0, "sol does not support RendererAPI::API::NONE yet");
		return nullptr;
	}
	case RendererAPI::API::OPEN_GL:
		return new OpenGLShader(vertex_source, fragment_source);
	}
	SOL_CORE_ASSERT(0, "RendererAPI::API not found");
	return nullptr;
}

} // namespace sol