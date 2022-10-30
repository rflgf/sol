#pragma once

#include "Renderer/Texture.h"

namespace sol
{

class OpenGLTexture2D : public Texture2D
{
private:
	uint32_t width;
	uint32_t height;
	uint32_t id;

public:
	OpenGLTexture2D(const std::string &path);
	~OpenGLTexture2D();

	uint32_t get_width() const override { return width; };
	uint32_t get_height() const override { return height; };

	void bind(const uint32_t slot) const override;
};

} // namespace sol