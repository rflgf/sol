#type vertex
#version 420 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec2 tex_coord;

uniform mat4 view_projection;
uniform mat4 transform;

out vec2 tex_coord_out;

void main()
{
	gl_Position   = view_projection * transform * vec4(position, 1.0);
	tex_coord_out = tex_coord;
}

#type fragment
#version 420 core

layout(location = 0) out vec4 color_out;

in vec2 tex_coord_out;

uniform sampler2D tex_id;

void main() { color_out = texture(tex_id, tex_coord_out); }