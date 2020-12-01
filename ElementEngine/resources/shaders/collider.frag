#version 450
#extension GL_ARB_separate_shader_objects : enable


layout(set = 2, binding = 0) uniform UniformBufferObject {
    vec4 displayColour;
} ubo;

layout(location = 0) out vec4 outColor;

void main() {

//    outColor = vec4(0.12, 1.0, 0.34, 1.0);
    outColor = ubo.displayColour;
}