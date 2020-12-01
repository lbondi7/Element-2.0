#version 450
#extension GL_ARB_separate_shader_objects : enable

layout(set = 0, binding = 0) uniform UBO {
    mat4 view;
    mat4 proj;
    vec3 camPos;
} ubos;


layout(set = 1, binding = 0) uniform UniformBufferObject {
    mat4 model;
} ubo;

layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec3 inColor;
layout(location = 2) in vec2 inTexCoord;


void main() {
    gl_Position = ubos.proj * ubos.view * ubo.model * vec4(inPosition, 1.0);
}