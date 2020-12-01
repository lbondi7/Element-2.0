#version 450
#extension GL_ARB_separate_shader_objects : enable


layout(binding = 0) uniform UniformBufferObject {
    mat4 model;
    mat4 view;
    mat4 proj;
    vec3 viewPos;
} ubo;

layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec3 inColor;
layout(location = 2) in vec3 inTexCoord;

layout(location = 0) out vec3 fragVert;
layout(location = 1) out vec3 fragColor;
layout(location = 2) out vec2 fragTexCoord;
layout(location = 3) out vec3 viewPos;

void main() {

    gl_Position = ubo.proj * ubo.view * ubo.model * vec4(inPosition, 1.0);
    
    fragVert = gl_Position.xyz;
    fragColor = inColor;
    fragTexCoord = inTexCoord;
    viewPos = ubo.viewPos;
}