#version 450
#extension GL_ARB_separate_shader_objects : enable

layout(location = 0) in vec4 fragColor;

layout(location = 0) out vec4 outColor;

void main() {
    vec4 col = fragColor;
    col.r = 1.0;
	col.b = 0.0;
	col.g = 0.0;
    outColor = col;
}