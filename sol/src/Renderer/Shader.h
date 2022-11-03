#pragma once

#include <glm/glm.hpp>
#include <string>
#include <unordered_map>

namespace sol
{

class Shader
{
public:
	enum class Type
	{
		NONE = 0,
		VERTEX,
		FRAGMENT
	};

	virtual ~Shader() = default;

	static Shader *create(const char *filepath);

	static Shader *create(const std::string &vertex_source,
	                      const std::string &fragment_source);

	static Type type_from_string(std::string_view text);

	virtual void bind() const   = 0;
	virtual void unbind() const = 0;

	virtual void upload_uniform_int(std::string name,
	                                const int32_t value) const           = 0;
	virtual void upload_uniform_int_array(const std::string &name,
	                                      int32_t *value,
	                                      size_t count) const            = 0;
	virtual void upload_uniform_unsigned_int(std::string name,
	                                         const uint32_t value) const = 0;
	virtual void upload_uniform_float(std::string name,
	                                  const float_t value) const         = 0;
	virtual void upload_uniform_float_2(std::string name,
	                                    const glm::vec2 value) const     = 0;
	virtual void upload_uniform_float_3(std::string name,
	                                    const glm::vec3 &value) const    = 0;
	virtual void upload_uniform_float_4(std::string name,
	                                    const glm::vec4 &value) const    = 0;
	virtual void upload_uniform_matrix_4(std::string name,
	                                     const glm::mat4 &value) const   = 0;
};
} // namespace sol