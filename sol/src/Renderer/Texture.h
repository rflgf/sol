#pragma once

#include <memory>
#include <string>

namespace sol
{

class Texture
{
public:
	virtual uint32_t get_width() const  = 0;
	virtual uint32_t get_height() const = 0;

	virtual void bind(const uint8_t slot = 0) const                    = 0;
	virtual void set_data(const void *data, const uint32_t size) const = 0;
	virtual uint32_t get_texture_api_id() const                        = 0;

	virtual bool operator==(const Texture &other) const = 0;
};

class Texture2D : public Texture
{
public:
	static std::shared_ptr<Texture2D> create(const std::string &path);
	static std::shared_ptr<Texture2D> create(const uint32_t width,
	                                         const uint32_t height);
};

} // namespace sol