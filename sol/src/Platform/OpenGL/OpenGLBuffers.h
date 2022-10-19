#pragma once

#include "Renderer/Buffers.h"

namespace sol
{
class OpenGLVertexBuffer : public VertexBuffer
{
public:
	OpenGLVertexBuffer(float *vertices, size_t size);
	~OpenGLVertexBuffer();

	void bind() const override;
	void unbind() const override;
};

class OpenGLIndexBuffer : public IndexBuffer
{
public:
	OpenGLIndexBuffer(uint32_t *indices, size_t count);
	~OpenGLIndexBuffer();

	void bind() const override;
	void unbind() const override;
};
} // namespace sol