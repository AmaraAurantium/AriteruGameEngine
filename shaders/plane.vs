#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec3 Normal;
out vec3 FragPos;
out vec2 TexCoords;

void main()
{
    Normal = transpose(inverse(mat3(model))) * aNormal;
    FragPos = vec3(model * vec4(aPos, 1.0));

    TexCoords = aTexCoords;

    gl_Position = projection * view * model * vec4(aPos, 1.0);
}