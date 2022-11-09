#include "Shader.h"

#include "Platform/OpenGL/OpenGLShader.h"
#include "Renderer/RendererAPI.h"

#include <fstream>

namespace sol
{

Shader *Shader::create(const std::string name, const std::string &vertex_source,
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
		return new OpenGLShader(name, fragment_source, vertex_source);
	}
	SOL_CORE_ASSERT(0, "RendererAPI::API not found");
	return nullptr;
}

Shader *Shader::create(const std::string &filepath)
{
	switch (RendererAPI::get_type())
	{
	case RendererAPI::API::NONE:
	{
		SOL_CORE_ASSERT(0, "sol does not support RendererAPI::API::NONE yet");
		return nullptr;
	}
	case RendererAPI::API::OPEN_GL:
		return new OpenGLShader(filepath);
	}
	SOL_CORE_ASSERT(0, "RendererAPI::API not found");
	return nullptr;
}

Shader::Step Shader::step_from_string(std::string_view text)
{
	if (text == "vertex")
		return Step::VERTEX;
	else if (text == "fragment")
		return Step::FRAGMENT;
	else
		return Step::NONE;
}

std::string Shader::step_to_string(Shader::Step value)
{
	switch (value)
	{
		// clang-format off
		case Step::FRAGMENT:      return "fragment";
		case Step::VERTEX:        return "vertex";

		case Step::NONE: default: return "none";
		// clang-format on
	}
}

} // namespace sol