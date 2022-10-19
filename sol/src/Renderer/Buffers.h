#pragma once

#include "Core.h"

namespace sol
{

class VertexBuffer
{
protected:
	uint32_t id;

public:
	VertexBuffer()          = default;
	virtual ~VertexBuffer() = default;

	static VertexBuffer *create(float *vertices, size_t size);
	virtual void bind() const   = 0;
	virtual void unbind() const = 0;
};

class IndexBuffer
{
protected:
	uint32_t id;
	size_t count;

public:
	IndexBuffer(size_t count)
	    : count(count)
	{
	}
	virtual ~IndexBuffer() = default;
	static IndexBuffer *create(uint32_t *indices, size_t count);
	virtual void bind() const   = 0;
	virtual void unbind() const = 0;
	size_t get_count() const { return count; }
};

} // namespace sol