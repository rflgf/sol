#step vertex
#version 420 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec2 texture_coordinates;
layout(location = 2) in vec4 color;
layout(location = 3) in float tiling_factor;
layout(location = 4) in float texture_slot;

uniform mat4 view_projection;

out vec2 vert_out_texture_coordinates;
out vec4 vert_out_color;
out float vert_out_tiling_factor;
out float vert_out_texture_slot;

void main()
{
	gl_Position                  = view_projection * vec4(position, 1.0);
	vert_out_texture_coordinates = texture_coordinates;
	vert_out_color               = color;
	vert_out_texture_slot        = texture_slot;
	vert_out_tiling_factor       = tiling_factor;
}

#step fragment
#version 420 core

layout(location = 0) out vec4 frag_out_color;

in vec2 vert_out_texture_coordinates;
in vec4 vert_out_color;
in float vert_out_tiling_factor;
in float vert_out_texture_slot;

uniform sampler2D
    textures[16]; // TODO(rafael): change this based on the platform.

void main()
{
	frag_out_color =
	    texture(textures[int(vert_out_texture_slot)],
	            vert_out_texture_coordinates * vert_out_tiling_factor) *
	    vert_out_color;
}