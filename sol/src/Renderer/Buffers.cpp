#include "Buffers.h"

#include "Platform/OpenGL/OpenGLBuffers.h"
#include "Renderer.h"

namespace sol
{

VertexBuffer *VertexBuffer::create(float *vertices, size_t size)
{
	switch (Renderer::get_api())
	{
	case Renderer::API::NONE:
	{
		SOL_CORE_ASSERT(0, "sol does not support Renderer::API::NONE yet");
		return nullptr;
	case Renderer::API::OPEN_GL:
		return new OpenGLVertexBuffer(vertices, size);
	}
	}
	SOL_CORE_ASSERT(0, "Renderer::API not found");
	return nullptr;
}
IndexBuffer *IndexBuffer::create(uint32_t *indices, size_t count)
{
	switch (Renderer::get_api())
	{
	case Renderer::API::NONE:
	{
		SOL_CORE_ASSERT(0, "sol does not support Renderer::API::NONE yet");
		return nullptr;
	case Renderer::API::OPEN_GL:
		return new OpenGLIndexBuffer(indices, count);
	}
	}
	SOL_CORE_ASSERT(0, "Renderer::API not found");
	return nullptr;
}
} // namespace sol