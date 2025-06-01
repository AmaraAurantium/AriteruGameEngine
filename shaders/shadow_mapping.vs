#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;

out VS_OUT {
    vec3 Fragpos;
    vec2 TexCoords;
    vec4 FragPosLightSpace;
} vs_out;

out vec3 Normal;

uniform mat4 projection;
uniform mat4 model;
uniform mat4 view;
uniform mat4 lightSpaceMatrix;

void main()
{
    vs_out.Fragpos = vec3(model * vec4(aPos, 1.0));
    vs_out.TexCoords = aTexCoords;
    vs_out.FragPosLightSpace = lightSpaceMatrix * model * vec4(aPos, 1.0);

    Normal = transpose(inverse(mat3(model))) * aNormal;  
    gl_Position = projection * view * model * vec4(aPos, 1.0);
}