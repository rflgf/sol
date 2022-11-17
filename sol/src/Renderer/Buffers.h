#pragma once

#include "Core.h"

#include <array>
#include <vector>

namespace sol
{

struct BufferElement
{
public:
	enum class Type
	{
		BOOL = 1,

		INT,
		INT_2,
		INT_3,
		INT_4,

		UINT,
		UINT_2,
		UINT_3,
		UINT_4,

		FLOAT,
		FLOAT_2,
		FLOAT_3,
		FLOAT_4,

		MAT_3,
		MAT_4
	};

	Type type;
	std::string name;
	size_t size;
	size_t offset;
	bool normalized;

	BufferElement(Type type, std::string name, bool normalized = false)
	    : type(type)
	    , name(name)
	    , size(size_from_type(type))
	    , offset(0)
	    , normalized(normalized)
	{
	}

	BufferElement() = default;

	static size_t size_from_type(Type t);
	static size_t component_count(Type t);
	friend struct BufferLayout;
};

struct BufferLayout
{
protected:
	std::vector<BufferElement> elements;
	size_t stride;

public:
	// credits due to dbf for coming up with this movable initializer_list
	// alternative thingy! :D
	template <size_t N>
	BufferLayout(BufferElement (&&args)[N])
	    : stride(0)
	    , elements(std::make_move_iterator(std::begin(args)),
	               std::make_move_iterator(std::end(args)))
	{
		size_t size = 0;
		for (BufferElement &element : elements)
		{
			element.offset = size;
			size += element.size;
		};

		stride = size;
	}

	BufferLayout() = default;

	size_t get_stride() const { return stride; }

	size_t get_element_count() const { return elements.size(); }

	std::vector<BufferElement>::iterator begin() { return elements.begin(); }
	std::vector<BufferElement>::iterator end() { return elements.end(); }
};

class VertexBuffer
{
protected:
	uint32_t id;

public:
	VertexBuffer() = default;
	VertexBuffer(BufferLayout layout);
	virtual ~VertexBuffer() = default;

	static VertexBuffer *create(const float *vertices, const size_t size);
	virtual void bind() const                     = 0;
	virtual void unbind() const                   = 0;
	virtual void set_layout(BufferLayout &layout) = 0;
	virtual BufferLayout &get_layout()            = 0;
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

class VertexArray
{
protected:
	uint32_t id;

public:
	virtual ~VertexArray() = default;

	static VertexArray *create();
	virtual void add_vertex_buffer(std::shared_ptr<VertexBuffer> vbo) = 0;
	virtual void set_index_buffer(std::shared_ptr<IndexBuffer> ibo)   = 0;
	virtual std::shared_ptr<IndexBuffer> get_index_buffer() const     = 0;
	virtual void bind() const                                         = 0;
	virtual void unbind() const                                       = 0;
};

} // namespace sol