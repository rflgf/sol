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

	virtual void bind(const uint32_t slot = 0) const = 0;
};

class Texture2D : public Texture
{
public:
	static std::shared_ptr<Texture2D> create(const std::string &path);
};

} // namespace sol