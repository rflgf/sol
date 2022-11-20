#include "Buffers.h"

#include "Platform/OpenGL/OpenGLBuffers.h"
#include "RendererAPI.h"

namespace sol
{

///////////////////////
// VertexBuffer ///////
///////////////////////

std::shared_ptr<VertexBuffer> VertexBuffer::create(const size_t size)
{
	switch (RendererAPI::get_type())
	{
	case RendererAPI::API::NONE:
	{
		SOL_CORE_ASSERT(0, "sol does not support RendererAPI::API::NONE yet");
		return nullptr;
	}
	case RendererAPI::API::OPEN_GL:
		return std::make_shared<OpenGLVertexBuffer>(size);
	}
	SOL_CORE_ASSERT(0, "RendererAPI::API not found");
	return nullptr;
}

std::shared_ptr<VertexBuffer> VertexBuffer::create(const float *vertices,
                                                   const size_t size)
{
	switch (RendererAPI::get_type())
	{
	case RendererAPI::API::NONE:
	{
		SOL_CORE_ASSERT(0, "sol does not support RendererAPI::API::NONE yet");
		return nullptr;
	}
	case RendererAPI::API::OPEN_GL:
		return std::make_shared<OpenGLVertexBuffer>(vertices, size);
	}
	SOL_CORE_ASSERT(0, "RendererAPI::API not found");
	return nullptr;
}

///////////////////////
// IndexBuffer ////////
///////////////////////

std::shared_ptr<IndexBuffer> IndexBuffer::create(uint32_t *indices,
                                                 size_t count)
{
	switch (RendererAPI::get_type())
	{
	case RendererAPI::API::NONE:
	{
		SOL_CORE_ASSERT(0, "sol does not support RendererAPI::API::NONE yet");
		return nullptr;
	}
	case RendererAPI::API::OPEN_GL:
		return std::make_shared<OpenGLIndexBuffer>(indices, count);
	}
	SOL_CORE_ASSERT(0, "RendererAPI::API not found");
	return nullptr;
}

///////////////////////
// BufferComponent ////
///////////////////////

size_t BufferElement::size_from_type(BufferElement::Type t)
{
	switch (t)
	{
	case BufferElement::Type::BOOL:
		return sizeof(bool);

	case BufferElement::Type::INT:
		return sizeof(int) * 1;
	case BufferElement::Type::INT_2:
		return sizeof(int) * 2;
	case BufferElement::Type::INT_3:
		return sizeof(int) * 3;
	case BufferElement::Type::INT_4:
		return sizeof(int) * 4;

	case BufferElement::Type::UINT:
		return sizeof(unsigned int) * 1;
	case BufferElement::Type::UINT_2:
		return sizeof(unsigned int) * 2;
	case BufferElement::Type::UINT_3:
		return sizeof(unsigned int) * 3;
	case BufferElement::Type::UINT_4:
		return sizeof(unsigned int) * 4;

	case BufferElement::Type::FLOAT:
		return sizeof(float) * 1;
	case BufferElement::Type::FLOAT_2:
		return sizeof(float) * 2;
	case BufferElement::Type::FLOAT_3:
		return sizeof(float) * 3;
	case BufferElement::Type::FLOAT_4:
		return sizeof(float) * 4;

	case BufferElement::Type::MAT_3:
		return sizeof(float) * 3 * 3;
	case BufferElement::Type::MAT_4:
		return sizeof(float) * 4 * 4;

	default:
	{
		SOL_CORE_ERROR("unkown BufferElement::Type");
		return 0;
	}
	}
}

size_t BufferElement::component_count(BufferElement::Type t)
{
	switch (t)
	{
	case BufferElement::Type::BOOL:
		return 1;

	case BufferElement::Type::INT:
		return 1;
	case BufferElement::Type::INT_2:
		return 2;
	case BufferElement::Type::INT_3:
		return 3;
	case BufferElement::Type::INT_4:
		return 4;

	case BufferElement::Type::UINT:
		return 1;
	case BufferElement::Type::UINT_2:
		return 2;
	case BufferElement::Type::UINT_3:
		return 3;
	case BufferElement::Type::UINT_4:
		return 4;

	case BufferElement::Type::FLOAT:
		return 1;
	case BufferElement::Type::FLOAT_2:
		return 2;
	case BufferElement::Type::FLOAT_3:
		return 3;
	case BufferElement::Type::FLOAT_4:
		return 4;

	case BufferElement::Type::MAT_3:
		return 3 * 3;
	case BufferElement::Type::MAT_4:
		return 4 * 4;

	default:
	{
		SOL_CORE_ERROR("unkown BufferElement::Type");
		return 0;
	}
	}
}

///////////////////////
// VertexArray ////////
///////////////////////

VertexArray *VertexArray::create()
{
	switch (RendererAPI::get_type())
	{
	case RendererAPI::API::NONE:
	{
		SOL_CORE_ASSERT(0, "sol does not support RendererAPI::API::NONE yet");
		return nullptr;
	}
	case RendererAPI::API::OPEN_GL:
		return new OpenGLVertexArray();
	}
	SOL_CORE_ASSERT(0, "RendererAPI::API not found");
	return nullptr;
}

} // namespace sol