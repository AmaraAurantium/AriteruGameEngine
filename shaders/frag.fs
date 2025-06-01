#version 330 core
in vec3 Normal;
in vec2 TexCoords;  // Make sure your vertex shader outputs this
in vec3 pointworldposition;
out vec4 FragColor;

uniform sampler2D texture_diffuse1;  // Texture uniform
uniform bool hasTexture;             // Flag to check if model has textures

void main()
{
    vec3 lightposition = vec3(500.0f, 500.0f, 0.0f);
    vec3 normal = normalize(Normal);
    vec3 lightdir = normalize(lightposition-pointworldposition);
    float lightintensity = dot(normal, lightdir)* 0.5f + 0.5f;
    vec3 baseColor = texture(texture_diffuse1,TexCoords).rgb * lightintensity;

    FragColor = vec4(baseColor,1.0f);
}