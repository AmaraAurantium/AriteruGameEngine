#version 330 core
in vec3 Normal;
in vec2 TexCoords;  // Make sure your vertex shader outputs this
in vec3 pointworldposition;
out vec4 FragColor;

uniform sampler2D texture_diffuse1;  // Texture uniform
uniform bool hasTexture;             // Flag to check if model has textures

void main()
{
    vec3 lightposition = vec3(100.0f, 100.0f, 0.0f);
    float lightintensity = dot(Normal,normalize(lightposition-pointworldposition))/2.0f+0.5f;
    vec3 baseColor = texture(texture_diffuse1,TexCoords).rgb * lightintensity;

    FragColor = vec4(baseColor,1.0f);
}