#include "OpenGLTexture.h"

#include "Core.h"

#include <glad/glad.h>
#include <stb_image.h>

namespace sol
{

OpenGLTexture2D::OpenGLTexture2D(const std::string &path)
    : width(0)
    , height(0)
    , id(0)
{
	int width, height, channels;
	GLenum internal_format = 0, data_format = 0;
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
	{
		SOL_CORE_ASSERT(0, "bad number of channels in texture");
	}

	SOL_CORE_ASSERT(data, "image could not be loaded: {}", path);
	this->width  = width;
	this->height = height;

	glCreateTextures(GL_TEXTURE_2D, 1, &id);
	glTextureStorage2D(id, 1, internal_format, width, height);

	glTextureParameteri(id, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTextureParameteri(id, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTextureParameteri(id, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTextureParameteri(id, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTextureSubImage2D(id, 0, 0, 0, width, height, data_format,
	                    GL_UNSIGNED_BYTE, data);

	stbi_image_free(data);
}

OpenGLTexture2D::~OpenGLTexture2D() { glDeleteTextures(1, &id); }

void OpenGLTexture2D::bind(const uint32_t slot) const
{
	glBindTextureUnit(slot, id);
}

} // namespace sol