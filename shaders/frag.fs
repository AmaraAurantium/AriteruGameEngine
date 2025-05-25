#version 330 core
in vec2 TexCoords;  // Make sure your vertex shader outputs this
out vec4 FragColor;

uniform sampler2D texture_diffuse1;  // Texture uniform
uniform bool hasTexture;             // Flag to check if model has textures

void main()
{
    vec3 baseColor = texture(texture_diffuse1,TexCoords).rgb;
    FragColor = vec4(baseColor,1.0f);
}