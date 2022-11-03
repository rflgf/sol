#include "Shader.h"

#include "Platform/OpenGL/OpenGLShader.h"
#include "Renderer/RendererAPI.h"
#include "Utils.h"

#include <fstream>

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
		return OpenGLShader::compile(
		    {{Type::FRAGMENT, fragment_source}, {Type::VERTEX, vertex_source}});
	}
	SOL_CORE_ASSERT(0, "RendererAPI::API not found");
	return nullptr;
}

Shader *Shader::create(const char *filepath)
{
	switch (RendererAPI::get_type())
	{
	case RendererAPI::API::NONE:
	{
		SOL_CORE_ASSERT(0, "sol does not support RendererAPI::API::NONE yet");
		return nullptr;
	}
	case RendererAPI::API::OPEN_GL:
	{
		std::unordered_map<Shader::Type, std::string> shaders;
		std::string source      = utils::read_from_file(filepath);
		const char type_token[] = "#type";
		size_t cursor           = 0;
		while ((cursor = source.find(type_token, cursor)) != std::string::npos)
		{
			cursor += strlen(type_token);
			while (source[cursor] == ' ' || source[cursor] == '\t' ||
			       source[cursor] == '\n')
				++cursor;
			size_t end_of_type_value = source.find('\n', cursor);
			std::string type_value_string =
			    source.substr(cursor, end_of_type_value - cursor);
			Type type = type_from_string(type_value_string);
			// next "#type", i.e., next shader source.
			size_t end_of_current_shader = source.find("#type", cursor);
			// case last shader source.
			if (end_of_current_shader == std::string::npos)
				end_of_current_shader = source.size();
			std::string current_shader_source = source.substr(
			    end_of_type_value, end_of_current_shader - end_of_type_value);
			shaders[type] = current_shader_source;
			cursor        = end_of_current_shader;
		}
		SOL_CORE_ASSERT(shaders.contains(Type::VERTEX) &&
		                    shaders.contains(Type::FRAGMENT),
		                "missing required shader step in {}", filepath);
		return OpenGLShader::compile(shaders);
	}
	}
	SOL_CORE_ASSERT(0, "RendererAPI::API not found");
	return nullptr;
}

Shader::Type Shader::type_from_string(std::string_view text)
{
	if (text == "vertex")
		return Type::VERTEX;
	else if (text == "fragment")
		return Type::FRAGMENT;
	else
		return Type::NONE;
}

std::string Shader::type_to_string(Shader::Type value)
{
	switch (value)
	{
		// clang-format off
		case Type::FRAGMENT:      return "fragment";
		case Type::VERTEX:        return "vertex";

		case Type::NONE: default: return "none";
		// clang-format on
	}
}

} // namespace sol