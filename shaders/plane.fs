#version 330 core

out vec4 FragColor;

in vec3 Normal;
in vec3 FragPos;
in vec2 TexCoords;

void main()
{
    FragColor = vec4(0.8, 0.8, 0.8, 1.0);
}