#include "OpenGLTexture.h"

#include "Core.h"

#include <stb_image.h>

namespace sol
{

OpenGLTexture2D::OpenGLTexture2D(const uint32_t width, const uint32_t height)
    : width(width)
    , height(height)
    , id(0)
    , internal_format(GL_RGBA8)
    , data_format(GL_RGBA)
{
	glCreateTextures(GL_TEXTURE_2D, 1, &id);
	glTextureStorage2D(id, 1, internal_format, width, height);

	glTextureParameteri(id, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTextureParameteri(id, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTextureParameteri(id, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTextureParameteri(id, GL_TEXTURE_WRAP_T, GL_REPEAT);
}

OpenGLTexture2D::OpenGLTexture2D(const std::string &path)
    : width(0)
    , height(0)
    , id(0)
    , internal_format(0)
    , data_format(0)
{
	int width, height, channels;
	stbi_set_flip_vertically_on_load(1);
	stbi_uc *data = stbi_load(path.c_str(), &width, &height, &channels, 0);

	if (channels == 4)
	{
		internal_format = GL_RGBA8;
		data_format     = GL_RGBA;
	}
	else if (channels == 3)
	{
		internal_format = GL_RGB8;
		data_format     = GL_RGB;
	}
	else
		SOL_CORE_ASSERT(0, "bad number of channels in texture");

	SOL_CORE_ASSERT(data, "image could not be loaded: {}", path);

	this->width  = width;
	this->height = height;

	glCreateTextures(GL_TEXTURE_2D, 1, &id);
	glTextureStorage2D(id, 1, internal_format, width, height);

	glTextureParameteri(id, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTextureParameteri(id, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTextureParameteri(id, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTextureParameteri(id, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTextureSubImage2D(id, 0, 0, 0, width, height, data_format,
	                    GL_UNSIGNED_BYTE, data);

	stbi_image_free(data);
}

OpenGLTexture2D::~OpenGLTexture2D() { glDeleteTextures(1, &id); }

void OpenGLTexture2D::bind(const uint8_t slot) const
{
	glBindTextureUnit(slot, id);
}

void OpenGLTexture2D::set_data(const void *data, const uint32_t size) const
{
	uint32_t bytes_per_pixel = data_format == GL_RGBA ? 4 : 3;
	SOL_CORE_ASSERT(size == width * height * bytes_per_pixel,
	                "data must be entire texture!");
	glTextureSubImage2D(id, 0, 0, 0, width, height, data_format,
	                    GL_UNSIGNED_BYTE, data);
}

bool OpenGLTexture2D::operator==(const Texture &other) const
{
	return id == static_cast<const OpenGLTexture2D &>(other).id;
}

} // namespace sol