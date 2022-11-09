#pragma once

#include "Renderer/Shader.h"

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <unordered_map>

namespace sol
{

class OpenGLShader : public Shader
{
private:
	uint32_t id;
	std::string name;

	// FIXME(rafael): this, alongside get_uniform_location(string), is a dirty
	// little trick to get uniform locations faster. optimally, all uniform
	// locations would be calculated at shader compilation time.
	mutable std::unordered_map<std::string, int> locations;

public:
	OpenGLShader(const std::string &filepath);
	OpenGLShader(const std::string &name, const std::string &vertex_source,
	             const std::string &fragment_source);
	~OpenGLShader();

	void compile(const std::unordered_map<Step, std::string> &sources);

	static std::unordered_map<Step, std::string>
	parse(const std::string &source);

	static GLenum gl_step_from_sol_internal_step(Shader::Step step);

	void bind() const override;
	void unbind() const override;

	const std::string &get_name() const override { return name; }

	void upload_uniform_int(std::string name,
	                        const int32_t value) const override;
	void upload_uniform_int_array(const std::string &name, int32_t *value,
	                              size_t count) const override;
	void upload_uniform_unsigned_int(std::string name,
	                                 const uint32_t value) const override;
	void upload_uniform_float(std::string name,
	                          const float_t value) const override;
	void upload_uniform_float_2(std::string name,
	                            const glm::vec2 value) const override;
	void upload_uniform_float_3(std::string name,
	                            const glm::vec3 &value) const override;
	void upload_uniform_float_4(std::string name,
	                            const glm::vec4 &value) const override;
	void upload_uniform_matrix_4(std::string name,
	                             const glm::mat4 &value) const override;

	// see comment in Shader::locations.
	int get_uniform_location(const std::string &name) const;
};

} // namespace sol