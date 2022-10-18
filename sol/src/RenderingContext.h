#pragma once

namespace sol
{
class RenderingContext
{
public:
	virtual void init()                                = 0;
	virtual void deinit()                              = 0;
	virtual void swap_buffers()                        = 0;
	virtual void *get_native_rendering_context() const = 0;
};
} // namespace sol