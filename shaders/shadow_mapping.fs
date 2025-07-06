#version 330 core

out vec4 FragColor;


in VS_OUT {
    vec3 Fragpos;
    vec2 TexCoords;
    vec4 FragPosLightSpace;
} fs_in;

in vec3 Normal;

uniform sampler2D texture_diffuse1;
uniform sampler2D texture_normal1;
uniform sampler2D texture_specular1;

uniform sampler2D shadowMap;

uniform vec3 lightPos;
uniform vec3 viewPos;
uniform vec3 lightColor;

float ShadowCalculation(vec4 fragPosLightSpace)
{
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    projCoords = projCoords * 0.5 + 0.5;

    float currentDepth = projCoords.z;

    vec3 normal = Normal;//texture(texture_diffuse1, fs_in.TexCoords).xyz;
    //vec3 lightDir = normalize(lightPos - fs_in.Fragpos);
    vec3 lightDir = normalize(lightPos - vec3(0));
    float bias = max(0.025f * (1.0f - dot(normal, lightDir)), 0.0005f);

    // PCF soft shadow
    float shadow = 0.0;
    vec2 texelSize = 1.0 / textureSize(shadowMap, 0); 
    for(float x = -1.5; x <= 1.5; x += 1.0)
    {
        for(float y = -1.5; y <= 1.5; y += 1.0)
        {
            float pcfDepth = texture(shadowMap, projCoords.xy + vec2(x, y) * texelSize).r;
            shadow += currentDepth - bias > pcfDepth ? 1.0 : 0.0;
        }
    }
    shadow = shadow / 16.0f;

    if(projCoords.z > 1.0)
        shadow = 0.0;

    // float closestDepth = texture(shadowMap, projCoords.xy).r;
    // shadow = currentDepth - 0.0005f > closestDepth ? 1.0 : 0.0;
    return shadow;

}

void main()
{
    vec3 color = texture(texture_diffuse1, fs_in.TexCoords).rgb;

    vec3 normal = normalize(Normal);//texture(texture_normal1, fs_in.TexCoords).xyz;

    vec3 ambient = lightColor * 0.15;

    //vec3 lightDir = normalize(lightPos - fs_in.Fragpos);
    vec3 lightDir = normalize(lightPos - vec3(0));
    float diff = 0.5 * dot(lightDir, normal) + 0.5;
    vec3 diffuse = diff * lightColor;

    vec3 viewDir = normalize(viewPos - fs_in.Fragpos);

    float spec = 0.0;
    vec3 halfwayDir = normalize(viewDir + lightDir);
    spec = pow(max(dot(halfwayDir, normal), 0.0), 64.0);
    vec3 specular = spec * lightColor * texture(texture_specular1, fs_in.TexCoords).rgb;

    float shadow = ShadowCalculation(fs_in.FragPosLightSpace);
    vec3 lighting = (ambient + (1.0 - shadow) * (diffuse + specular)) * color;

    //FragColor = vec4(ambient, 1.0) + (1 - shadow) * vec4(1.0, 1.0, 1.0, 1.0);
    //FragColor = vec4(lighting, 1.0);
    shadow = 1.0 - shadow;
    FragColor = vec4(shadow, shadow, shadow, 1.0);
    //shadow = 1.0 - shadow;
}