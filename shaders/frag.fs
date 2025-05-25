#version 330 core
in vec2 TexCoords;  // Make sure your vertex shader outputs this
out vec4 FragColor;

uniform sampler2D texture_diffuse1;  // Texture uniform
uniform bool hasTexture;             // Flag to check if model has textures

void main()
{
    if (hasTexture) {
        FragColor = texture(texture_diffuse1, TexCoords);
    } else {
        FragColor = vec4(1.0f, 0.777f, 0.545f, 1.0f);  // Your peach fallback
    }
}