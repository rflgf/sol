#pragma once

#include "Renderer/Buffers.h"

namespace sol
{

class OpenGLVertexBuffer : public VertexBuffer
{
private:
	BufferLayout layout;

public:
	OpenGLVertexBuffer(const size_t size);
	OpenGLVertexBuffer(const float *vertices, const size_t size);
	~OpenGLVertexBuffer();

	void bind() const override;
	void unbind() const override;
	void set_layout(BufferLayout &layout) override;
	BufferLayout &get_layout() override { return layout; }
	void set_data(const void *data, size_t size) override;
};

class OpenGLIndexBuffer : public IndexBuffer
{
public:
	OpenGLIndexBuffer(uint32_t *indices, size_t count);
	~OpenGLIndexBuffer();

	void bind() const override;
	void unbind() const override;
};

class OpenGLVertexArray : public VertexArray
{
private:
	uint32_t vbo;
	std::shared_ptr<IndexBuffer> ibo;
	std::vector<std::shared_ptr<VertexBuffer>> vbos;

public:
	OpenGLVertexArray();
	~OpenGLVertexArray();

	void add_vertex_buffer(std::shared_ptr<VertexBuffer> vbo) override;
	void set_index_buffer(std::shared_ptr<IndexBuffer> ibo) override;
	std::shared_ptr<IndexBuffer> get_index_buffer() const override;
	void bind() const override;
	void unbind() const override;
};

} // namespace sol