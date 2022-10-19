#include "OpenGLBuffers.h"

#include <glad/glad.h>

namespace sol
{

// OpenGLVertexBuffer

OpenGLVertexBuffer::OpenGLVertexBuffer(float *vertices, size_t size)
{
	glGenBuffers(1, &id);
	glBindBuffer(GL_ARRAY_BUFFER, id);
	glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);
}

OpenGLVertexBuffer::~OpenGLVertexBuffer() { glDeleteBuffers(1, &id); }

void OpenGLVertexBuffer::bind() const { glBindBuffer(GL_ARRAY_BUFFER, id); }

void OpenGLVertexBuffer::unbind() const { glBindBuffer(GL_ARRAY_BUFFER, 0); }

// OpenGLIndexBuffer

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

} // namespace sol