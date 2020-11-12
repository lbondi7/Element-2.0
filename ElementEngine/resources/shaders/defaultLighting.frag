#version 450
#extension GL_ARB_separate_shader_objects : enable
#extension GL_ARB_viewport_array : enable

struct Light
{
    vec3 col;
    vec3 pos;

    // general light
    vec3 attenuation;
    // constant;
    // linear;
    // quadratic;

    //spot light specifics
    vec3 direction;
    float cutOff;
    float outerCutOff;

    float intensity;

    int type;
};

layout(std140, set = 1, binding = 0) buffer Lights
{
   Light lights[];
};

layout(set = 1, binding = 1) uniform LightConstants{
    int lightCount;
} lightConst;

layout(set = 2,binding = 1) uniform sampler2D texSampler;

layout(set = 2, binding = 2) uniform Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
} mat;

layout(location = 0) in vec3 fragVert;
layout(location = 1) in vec3 fragColor;
layout(location = 2) in vec2 fragTexCoord;
layout(location = 3) in vec3 camPos;

layout(location = 0) out vec4 outColor;

vec3 CalculateLight(Light light, vec3 normal, vec3 fragPos, vec3 viewDir);

void main() {

    vec3 norm = normalize(fragColor);
    // vec3 lightDir = normalize(vec3(1.2, -1.0, -2.0) - fragVert);  

	// vec3 lightColour = vec3(0.1, 0.4, 0.3);

    // vec3 ambient = mat.ambient.xyz * lightColour;

    // float diff = max(dot(norm, lightDir), 0.0);
    // vec3 diffuse = (diff * mat.diffuse.xyz) * lightColour;

    // vec3 viewDir = normalize(viewPos - fragVert);
    // vec3 reflectDir = reflect(-lightDir, norm);  
    
    // float spec = pow(max(dot(viewDir, reflectDir), 0.0), mat.shininess);
    // vec3 specular = (mat.specular.xyz * spec) * lightColour;

    //vec3 result = (ambient + diffuse + specular) * vec3(1.0);

    vec3 result = vec3(0);

    for(int i = 0; i < lightConst.lightCount; i++)
        result += CalculateLight(lights[i], norm, fragVert, normalize(camPos - fragVert));

    outColor = texture(texSampler, fragTexCoord) * vec4(result, 1.0);
    
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

// }

// function prototypes


vec3 CalculateLight(Light light, vec3 normal, vec3 fragPos, vec3 viewDir){

    vec3 lightDir;
    
    if(light.type == 1){
        lightDir = normalize(-light.direction);
    }    
    else{
        lightDir = normalize(light.pos - fragPos);
    }

    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), mat.shininess);

    // combine results
    vec3 ambient = mat.ambient * light.col * light.intensity;
    vec3 diffuse = mat.diffuse * diff * light.col * light.intensity;
    vec3 specular = mat.specular * spec * light.col * light.intensity;

    if(light.type != 1){

        float distance = length(light.pos - fragPos);
        float attenuation = 1.0 / (light.attenuation.x + light.attenuation.y * distance + light.attenuation.z * (distance * distance));  
        

        ambient *= attenuation;
        diffuse *= attenuation;
        specular *= attenuation; 

        if(light.type == 2)
        {
                // spotlight intensity
            float theta = dot(lightDir, normalize(-light.direction)); 
            float epsilon = light.cutOff - light.outerCutOff;
            float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);

            ambient *= intensity;
            diffuse *= intensity;
            specular *= intensity;
        }
    }

    // ambient *= intensity;
    // diffuse *= intensity;
    // specular *= intensity;

    return (ambient + diffuse + specular);

}

// void main()
// {    
//     // properties
//     vec3 norm = normalize(Normal);
//     vec3 viewDir = normalize(viewPos - FragPos);
    
//     // == =====================================================
//     // Our lighting is set up in 3 phases: directional, point lights and an optional flashlight
//     // For each phase, a calculate function is defined that calculates the corresponding color
//     // per lamp. In the main() function we take all the calculated colors and sum them up for
//     // this fragment's final color.
//     // == =====================================================
//     // phase 1: directional lighting
//     vec3 result = CalcDirLight(dirLight, norm, viewDir);
//     // phase 2: point lights
//     for(int i = 0; i < NR_POINT_LIGHTS; i++)
//         result += CalcPointLight(pointLights[i], norm, FragPos, viewDir);    
//     // phase 3: spot light
//     result += CalcSpotLight(spotLight, norm, FragPos, viewDir);    
    
//     FragColor = vec4(result, 1.0);
// }

// calculates the color when using a directional light.
// vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir)
// {
//     vec3 lightDir = normalize(-light.direction);
//     // diffuse shading
//     float diff = max(dot(normal, lightDir), 0.0);
//     // specular shading
//     vec3 reflectDir = reflect(-lightDir, normal);
//     float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
//     // combine results
//     vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoords));
//     vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, TexCoords));
//     vec3 specular = light.specular * spec * vec3(texture(material.specular, TexCoords));
//     return (ambient + diffuse + specular);
// }

// // calculates the color when using a point light.
// vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
// {
    // vec3 lightDir = normalize(light.position - fragPos);
    // // diffuse shading
    // float diff = max(dot(normal, lightDir), 0.0);
    // // specular shading
    // vec3 reflectDir = reflect(-lightDir, normal);
    // float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    // // attenuation
    // float distance = length(light.position - fragPos);
    // float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));    
    // // combine results
    // vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoords));
    // vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, TexCoords));
    // vec3 specular = light.specular * spec * vec3(texture(material.specular, TexCoords));
    // ambient *= attenuation;
    // diffuse *= attenuation;
    // specular *= attenuation;
    // return (ambient + diffuse + specular);
// }

// // calculates the color when using a spot light.
// vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
// {
//     vec3 lightDir = normalize(light.position - fragPos);
//     // diffuse shading
//     float diff = max(dot(normal, lightDir), 0.0);
//     // specular shading
//     vec3 reflectDir = reflect(-lightDir, normal);
//     float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
//     // attenuation
//     float distance = length(light.position - fragPos);
//     float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));    
//     // spotlight intensity
//     float theta = dot(lightDir, normalize(-light.direction)); 
//     float epsilon = light.cutOff - light.outerCutOff;
//     float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);
//     // combine results
//     vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoords));
//     vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, TexCoords));
//     vec3 specular = light.specular * spec * vec3(texture(material.specular, TexCoords));

//     ambient *= attenuation * intensity;
//     diffuse *= attenuation * intensity;
//     specular *= attenuation * intensity;

//     return (ambient + diffuse + specular);
// }