#version 450
#extension GL_ARB_separate_shader_objects : enable

struct Light
{
   vec4 col;
   vec4 pos;
};

layout(std140, binding = 3) buffer Lights
{
   Light lights[];
};

layout(binding = 1) uniform sampler2D texSampler;

layout(binding = 2) uniform Material {
    float roughness;
    float metallic;
} mat;

layout(location = 0) in vec3 fragVert;
layout(location = 1) in vec3 fragColor;
layout(location = 2) in vec2 fragTexCoord;
layout(location = 3) in vec3 viewPos;

layout(location = 0) out vec4 outColor;

// DO NOT MULTIPLY BY COS THETA
vec3 shadingSpecularGGX(vec3 N, vec3 V, vec3 L, float roughness, vec3 F0)
{
    // see http://www.filmicworlds.com/2014/04/21/optimizing-ggx-shaders-with-dotlh/
    vec3 H = normalize(V + L);

    float dotLH = max(dot(L, H), 0.0);
    float dotNH = max(dot(N, H), 0.0);
    float dotNL = max(dot(N, L), 0.0);
    float dotNV = max(dot(N, V), 0.0);

    float alpha = roughness * roughness;

    // D (GGX normal distribution)
    float alphaSqr = alpha * alpha;
    float denom = dotNH * dotNH * (alphaSqr - 1.0) + 1.0;
    float D = alphaSqr / (denom * denom);
    // no pi because BRDF -> lighting

    // F (Fresnel term)
    float F_a = 1.0;
    float F_b = pow(1.0 - dotLH, 5); // manually?
    vec3 F = mix(vec3(F_b), vec3(F_a), F0);

    // G (remapped hotness, see Unreal Shading)
    float k = (alpha + 2 * roughness + 1) / 8.0;
    float G = dotNL / (mix(dotNL, 1, k) * mix(dotNV, 1, k));
    // '* dotNV' - canceled by normalization

    // orginal G:
    /*
    {
        float k = alpha / 2.0;
        float k2 = k * k;
        float invK2 = 1.0 - k2;
        float vis = 1 / (dotLH * dotLH * invK2 + k2);

        vec3 FV = mix(vec3(F_b), vec3(F_a), F0) * vis;
        vec3 specular = D * FV / 4.0f;
        return specular * dotNL;
    }
    */

    // '/ dotLN' - canceled by lambert
    // '/ dotNV' - canceled by G
    return D * F * G / 4.0;
}

// specular and diffuse contribution of a single light direction
vec3 shadingGGX(vec3 N, vec3 V, vec3 L, vec3 color, float roughness, float metallic)
{
    vec3 diffuse = color * (1 - metallic); // metals have no diffuse
    vec3 specular = mix(vec3(0.04), color, metallic); // fixed spec for non-metals

    float dotNL = max(dot(N, L), 0.0);

    return diffuse * dotNL + shadingSpecularGGX(N, V, L, roughness, specular);
}


void main() {
  
    vec3 normal = normalize(fragColor);
    vec3 viewVec = fragVert - viewPos;
    vec3 viewDir = normalize(fragVert - viewPos);
    vec3 invViewDir = normalize(viewPos - fragVert);

    vec3 totalLight = vec3(0);
    for(int i  = 0; i < 10; ++i)
    {
        vec3 lightPos = lights[i].pos.xyz;
        vec3 lightVec = fragVert - lightPos;
        vec3 lightDir = normalize(fragVert - lightPos);
        vec3 invLightDir = normalize(lightPos - fragVert);
        totalLight += shadingGGX(normal, invViewDir, invLightDir, lights[i].col.xyz, mat.roughness, mat.metallic);
    }

    outColor = texture(texSampler, fragTexCoord) * vec4(totalLight, 1.0);
}

// void main() {

//     vec3 norm = normalize(fragColor);
//     vec3 lightDir = normalize(vec3(1.2, -1.0, -2.0) - fragVert);  

// 	vec3 lightColour = vec3(0.1, 0.4, 0.3);

//     vec3 ambient = mat.ambient.xyz * lightColour;

//     float diff = max(dot(norm, lightDir), 0.0);
//     vec3 diffuse = (diff * mat.diffuse.xyz) * lightColour;

//     vec3 viewDir = normalize(viewPos - fragVert);
//     vec3 reflectDir = reflect(-lightDir, norm);  
    
//     float spec = pow(max(dot(viewDir, reflectDir), 0.0), mat.shininess);
//     vec3 specular = (mat.specular.xyz * spec) * lightColour;

//     vec3 result = (ambient + diffuse + specular) * vec3(1.0);

//     outColor = texture(texSampler, fragTexCoord) *  vec4(result, 1.0);

// 	// float ambientStrength = 0.0;
//     // vec3 ambient = ambientStrength * lightColour;

//     // float diff = max(dot(norm, lightDir), 0.0);
//     // vec3 diffuse = diff * lightColour;

//     // float specularStrength = 0.5;
//     // vec3 viewDir = normalize(viewPos - fragVert);
//     // vec3 reflectDir = reflect(-lightDir, norm);  
    
//     // float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
//     // vec3 specular = specularStrength * spec * lightColour;

//     // vec3 result = (ambient + diffuse + specular) * vec3(1, 0, 0);

//     // outColor = texture(texSampler, fragTexCoord) *  vec4(result, 1.0);
// }