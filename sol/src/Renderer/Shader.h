#pragma once

#include <glm/glm.hpp>
#include <memory>
#include <string>
#include <unordered_map>

namespace sol
{

class Shader
{
public:
	enum class Step
	{
		NONE = 0,
		VERTEX,
		FRAGMENT
	};

	class Library
	{
	private:
		Library() = default;
		static Library *self;
		std::unordered_map<std::string, std::shared_ptr<const Shader>> shaders;

	public:
		static void init();

		static std::shared_ptr<const Shader> get(const std::string &name);
		static void add(std::shared_ptr<const Shader> shader);
		static void load(const std::string &filepath);
		static void load(const std::string &name, const std::string &filepath);
	};

	virtual ~Shader() = default;

	static std::shared_ptr<const Shader> create(const std::string &filepath);
	static std::shared_ptr<const Shader>
	create(const std::string name, const std::string &vertex_source,
	       const std::string &fragment_source);

	std::shared_ptr<const Shader>
	compile(const std::unordered_map<Step, std::string> &shader_steps);

	static Step step_from_string(std::string_view text);
	static std::string step_to_string(Step value);

	virtual const std::string &get_name() const = 0;

	virtual void bind() const   = 0;
	virtual void unbind() const = 0;

	virtual void set_int(std::string name, const int32_t value) const       = 0;
	virtual void set_int_array(const std::string &name, int32_t *value,
	                           size_t count) const                          = 0;
	virtual void set_unsigned_int(std::string name,
	                              const uint32_t value) const               = 0;
	virtual void set_float(std::string name, const float_t value) const     = 0;
	virtual void set_float_2(std::string name, const glm::vec2 value) const = 0;
	virtual void set_float_3(std::string name,
	                         const glm::vec3 &value) const                  = 0;
	virtual void set_float_4(std::string name,
	                         const glm::vec4 &value) const                  = 0;
	virtual void set_matrix_4(std::string name,
	                          const glm::mat4 &value) const                 = 0;
};

} // namespace sol