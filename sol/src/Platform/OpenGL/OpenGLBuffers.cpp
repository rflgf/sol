#include "OpenGLBuffers.h"

#include <glad/glad.h>

namespace sol
{

///////////////////////
// utils //////////////
///////////////////////

GLenum OpenGL_base_type_from(BufferElement::Type t)
{
	switch (t)
	{
	case BufferElement::Type::BOOL:
		return GL_BOOL;

	case BufferElement::Type::INT:
		return GL_INT;
	case BufferElement::Type::INT_2:
		return GL_INT;
	case BufferElement::Type::INT_3:
		return GL_INT;
	case BufferElement::Type::INT_4:
		return GL_INT;

	case BufferElement::Type::UINT:
		return GL_UNSIGNED_INT;
	case BufferElement::Type::UINT_2:
		return GL_UNSIGNED_INT;
	case BufferElement::Type::UINT_3:
		return GL_UNSIGNED_INT;
	case BufferElement::Type::UINT_4:
		return GL_UNSIGNED_INT;

	case BufferElement::Type::FLOAT:
		return GL_FLOAT;
	case BufferElement::Type::FLOAT_2:
		return GL_FLOAT;
	case BufferElement::Type::FLOAT_3:
		return GL_FLOAT;
	case BufferElement::Type::FLOAT_4:
		return GL_FLOAT;

	case BufferElement::Type::MAT_3:
		return GL_FLOAT;
	case BufferElement::Type::MAT_4:
		return GL_FLOAT;

	default:
	{
		SOL_CORE_ERROR("unkown BufferElement::Type");
		return 0;
	}
	}
}

///////////////////////
// OpenGLVertexBuffer /
///////////////////////

OpenGLVertexBuffer::OpenGLVertexBuffer(const float *vertices, const size_t size)
{
	glGenBuffers(1, &id);
	glBindBuffer(GL_ARRAY_BUFFER, id);
	glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);
}

OpenGLVertexBuffer::~OpenGLVertexBuffer() { glDeleteBuffers(1, &id); }

void OpenGLVertexBuffer::bind() const { glBindBuffer(GL_ARRAY_BUFFER, id); }

void OpenGLVertexBuffer::unbind() const { glBindBuffer(GL_ARRAY_BUFFER, 0); }

void OpenGLVertexBuffer::set_layout(BufferLayout &layout)
{
	this->layout = layout;
}

///////////////////////
// OpenGLIndexBuffer //
///////////////////////

OpenGLIndexBuffer::OpenGLIndexBuffer(uint32_t *indices, size_t count)
    : IndexBuffer(count)
{
	glGenBuffers(1, &id);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(uint32_t), indices,
	             GL_STATIC_DRAW);
}

OpenGLIndexBuffer::~OpenGLIndexBuffer() { glDeleteBuffers(1, &id); }

void OpenGLIndexBuffer::bind() const
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id);
}

void OpenGLIndexBuffer::unbind() const
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

///////////////////////
// OpenGLVertexArray //
///////////////////////

OpenGLVertexArray::OpenGLVertexArray() { glCreateVertexArrays(1, &id); }

OpenGLVertexArray::~OpenGLVertexArray() { glDeleteVertexArrays(1, &id); }

void OpenGLVertexArray::add_vertex_buffer(std::shared_ptr<VertexBuffer> vbo)
{
	BufferLayout &layout = vbo->get_layout();
	SOL_CORE_ASSERT(layout.get_element_count(),
	                "VertexBuffer can't be bound with an empty layout");

	glBindVertexArray(id);
	vbo->bind();

	size_t index = 0;
	for (BufferElement &e : layout)
	{
		glEnableVertexAttribArray(index);

		size_t component_count       = BufferElement::component_count(e.type);
		GLenum OGL_type              = OpenGL_base_type_from(e.type);
		GLboolean normalized         = e.normalized ? GL_TRUE : GL_FALSE;
		size_t stride                = layout.get_stride();
		const void *offset_in_buffer = reinterpret_cast<const void *>(e.offset);

		glVertexAttribPointer(index++, component_count, OGL_type, normalized,
		                      stride, offset_in_buffer);
	}

	vbos.push_back(vbo);
}

void OpenGLVertexArray::set_index_buffer(std::shared_ptr<IndexBuffer> ibo)
{
	glBindVertexArray(id);
	ibo->bind();

	this->ibo = ibo;
}

std::shared_ptr<IndexBuffer> OpenGLVertexArray::get_index_buffer() const
{
	return ibo;
}

void OpenGLVertexArray::bind() const { glBindVertexArray(id); }

void OpenGLVertexArray::unbind() const { glBindVertexArray(0); }

} // namespace sol