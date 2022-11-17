#step vertex
#version 420 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec2 texture_coordinates;

uniform mat4 view_projection;
uniform mat4 transform;

out vec2 vert_out_texture_coordinates;

void main()
{
	gl_Position = view_projection * transform * vec4(position, 1.0);
	vert_out_texture_coordinates = texture_coordinates;
}

#step fragment
#version 420 core

layout(location = 0) out vec4 frag_out_color;

in vec2 vert_out_texture_coordinates;

uniform vec4 tint;
uniform sampler2D texture_slot;

void main()
{
	frag_out_color = texture(texture_slot, vert_out_texture_coordinates) * tint;
}