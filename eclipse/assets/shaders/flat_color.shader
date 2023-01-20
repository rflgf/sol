#step vertex
#version 420 core

layout(location = 0) in vec3 position;

uniform mat4 view_projection;
uniform mat4 transform;

void main() { gl_Position = view_projection * transform * vec4(position, 1.0); }

#step fragment
#version 420 core

layout(location = 0) out vec4 color_out;

uniform vec4 color;

void main() { color_out = color; }