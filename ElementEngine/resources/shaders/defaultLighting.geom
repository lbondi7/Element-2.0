#version 450
#extension GL_ARB_separate_shader_objects : enable
#extension GL_ARB_viewport_array : enable

layout (triangles) in;
layout (triangle_strip, max_vertices = 4) out;

struct CameraData {
    mat4 view;
    mat4 proj;
    vec3 cameraPos;
};

// layout(binding = 0) buffer StorageBufferObject {
//     CameraData sbos[];
// };

layout(binding = 0) uniform StorageBufferObject {
    mat4 view[2];
    mat4 proj[2];
    vec3 cameraPos[2];
}vbo;

layout(binding = 1) uniform cameraConsts {
    int cameraCount;
} camConst;

layout(location = 0) in vec3 inPos[];
layout(location = 1) in vec3 inColor[];
layout(location = 2) in vec2 inTexCoord[];

layout(location = 0) out vec3 fragVert;
layout(location = 1) out vec3 fragColor;
layout(location = 2) out vec2 fragTexCoord;
layout(location = 3) out vec3 viewPos;

void main() {


	for(int i = 0; i < gl_in.length(); i++)
	{	
		fragColor = inColor[i];
		fragTexCoord = inTexCoord[i];
		fragVert = inPos[i];
		mat4 projViewMat = vbo.proj[gl_ViewportIndex] * vbo.view[gl_ViewportIndex];
		viewPos = vbo.cameraPos[gl_ViewportIndex];
		gl_Position = projViewMat * vec4(inPos[i], 1.0);
		// Set the viewport index that the vertex will be emitted to
    	gl_PrimitiveID = gl_PrimitiveIDIn;
		gl_ViewportIndex = gl_InvocationID;
		EmitVertex();
	}
	EndPrimitive();
    
}