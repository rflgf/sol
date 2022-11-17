#pragma once

#include "Renderer/Texture.h"

#include <glad/glad.h>

namespace sol
{

class OpenGLTexture2D : public Texture2D
{
private:
	uint32_t width;
	uint32_t height;
	uint32_t id;
	GLenum internal_format;
	GLenum data_format;

public:
	OpenGLTexture2D(const std::string &path);
	OpenGLTexture2D(const uint32_t width, const uint32_t height);
	~OpenGLTexture2D();

	uint32_t get_width() const override { return width; };
	uint32_t get_height() const override { return height; };

	void bind(const uint32_t slot) const override;
	void set_data(const void *data, const uint32_t size) const override;
};

} // namespace sol