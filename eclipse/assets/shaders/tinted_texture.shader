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
out flat float vert_out_texture_slot;

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
in flat float vert_out_texture_slot;

uniform sampler2D
    textures[16]; // TODO(rafael): change this based on the platform.

void main()
{
	frag_out_color = vert_out_color;
	vec2 tiled_coordinates =
	    vert_out_texture_coordinates * vert_out_tiling_factor;

	switch (int(vert_out_texture_slot))
	{
	case 0:
		frag_out_color *= texture(textures[0], tiled_coordinates);
		break;
	case 1:
		frag_out_color *= texture(textures[1], tiled_coordinates);
		break;
	case 2:
		frag_out_color *= texture(textures[2], tiled_coordinates);
		break;
	case 3:
		frag_out_color *= texture(textures[3], tiled_coordinates);
		break;
	case 4:
		frag_out_color *= texture(textures[4], tiled_coordinates);
		break;
	case 5:
		frag_out_color *= texture(textures[5], tiled_coordinates);
		break;
	case 6:
		frag_out_color *= texture(textures[6], tiled_coordinates);
		break;
	case 7:
		frag_out_color *= texture(textures[7], tiled_coordinates);
		break;
	case 8:
		frag_out_color *= texture(textures[8], tiled_coordinates);
		break;
	case 9:
		frag_out_color *= texture(textures[9], tiled_coordinates);
		break;
	case 10:
		frag_out_color *= texture(textures[10], tiled_coordinates);
		break;
	case 11:
		frag_out_color *= texture(textures[11], tiled_coordinates);
		break;
	case 12:
		frag_out_color *= texture(textures[12], tiled_coordinates);
		break;
	case 13:
		frag_out_color *= texture(textures[13], tiled_coordinates);
		break;
	case 14:
		frag_out_color *= texture(textures[14], tiled_coordinates);
		break;
	case 15:
		frag_out_color *= texture(textures[15], tiled_coordinates);
		break;
	}

	if (frag_out_color.a == 0.0)
		discard;
}