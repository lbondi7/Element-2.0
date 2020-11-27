#version 450
#extension GL_ARB_separate_shader_objects : enable


layout(set = 0, binding = 0) uniform UBO {
    mat4 view;
    mat4 proj;
    vec3 camPos;
} ubos;


layout(set = 2, binding = 0) uniform UniformBufferObject {
    mat4 model;
    // mat4 view;
    // mat4 proj;
} ubo;

layout(set = 2, binding = 5) readonly uniform sampler2D UniformBufferObject;

layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec3 inColor;
layout(location = 2) in vec2 inTexCoord;

layout(location = 0) out vec3 fragVert;
layout(location = 1) out vec3 fragColor;
layout(location = 2) out vec2 fragTexCoord;
layout(location = 3) out vec3 camPos;

void main() {

    vec4 pos = ubo.model * vec4(inPosition, 1.0);
    fragVert = pos.xyz;
    //fragColor = (ubo.model * vec4(inColor, 1.0)).xyz;
    fragColor = inColor;
    fragTexCoord = inTexCoord;
    camPos = ubos.camPos;

    gl_Position = ubos.proj * ubos.view * pos;
    
}