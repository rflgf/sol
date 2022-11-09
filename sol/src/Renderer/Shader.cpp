#include "Shader.h"

#include "Platform/OpenGL/OpenGLShader.h"
#include "Renderer/RendererAPI.h"

#include <fstream>

namespace sol
{

///////////////////////
// Shader /////////////
///////////////////////
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

///////////////////////
// Shader::Library ////
///////////////////////

Shader::Library *Shader::Library::self = nullptr;

void Shader::Library::init()
{
	SOL_CORE_ASSERT(!self,
	                "attempt to create multiple shader library instances.");
	self = new Library();
}

const Shader *Shader::Library::get(const std::string &name)
{
	SOL_CORE_ASSERT(self->shaders.contains(name),
	                "shader '{}' not in shader library.", name);
	return self->shaders[name];
}

void Shader::Library::add(const Shader *shader)
{
	const std::string &name = shader->get_name();

	if (self->shaders.contains(name))
		delete self->shaders[name];

	self->shaders[name] = shader;
}

void Shader::Library::load(const std::string &filepath)
{
	Shader *shader = Shader::create(filepath);
	add(shader);
}

void Shader::Library::load(const std::string &name, const std::string &filepath)
{
	Shader *shader = Shader::create(filepath);

	if (self->shaders.contains(name))
		delete self->shaders[name];

	self->shaders[name] = shader;
}

} // namespace sol