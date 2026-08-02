#version 410 core

out vec4 FragColor;

in vec2 TexCoord;
in vec3 FragPos;
in vec3 Normal;

uniform sampler2D texture_diffuse0;
uniform sampler2D texture_specular0;

struct DirLight {
    vec3 direction;
    vec3 color;
};

struct PointLight {
    vec3 position;
    vec3 color;

    float constant;
    float linear;
    float quadratic;
};

struct SpotLight {
    vec3 direction;
    vec3 position;
    vec3 color;

    float cutOff;
    float outerCutOff;
};

uniform float ambientIntensity;
uniform DirLight dirLight;

#define NR_POINT_LIGHTS 1
uniform PointLight pointLights[NR_POINT_LIGHTS];

uniform SpotLight spotLight;

uniform bool wireframe;

vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir, vec4 diffTex, vec4 specTex) {
    vec3 lightDir = normalize(-light.direction);

    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0f);
    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 64.0f);
    // combine results
    vec3 ambient = ambientIntensity * diffTex.rgb;
    vec3 diffuse = diff * diffTex.rgb;
    float specular = spec * specTex.r;

    return (ambient + diffuse + specular) * light.color;
}

vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir, vec4 diffTex, vec4 specTex) {
    vec3 lightDir = normalize(light.position - fragPos);

    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 128.0f);
    // attenuation
    float distance = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));
    // combine results
    vec3 ambient = ambientIntensity * diffTex.rgb;
    vec3 diffuse = diff * diffTex.rgb;
    float specular = spec * specTex.r;

    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;

    return (ambient + diffuse + specular) * light.color;
}

vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir, vec4 diffTex, vec4 specTex) {
    vec3 lightDir = normalize(light.position - FragPos);

    // Diffuse Light
    float diff = max(dot(normal, lightDir), 0.0f);

    // Specular Highlights
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0f), 32.0f);

    // Spot Light
    float theta = dot(lightDir, normalize(-light.direction));
    float epsilon = light.cutOff - light.outerCutOff;
    float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0f, 1.0f);

    // Combine result
    vec3 ambient = ambientIntensity * diffTex.rgb;
    vec3 diffuse = diff * diffTex.rgb;
    float specular = spec * specTex.r;

    diffuse *= intensity;
    specular *= intensity;

    return (ambient + diffuse + specular) * light.color;
}

void main() {
    vec4 diffTex = texture(texture_diffuse0, TexCoord);
    vec4 specTex = texture(texture_specular0, TexCoord);

    vec3 norm = normalize(Normal);
    vec3 viewDir = normalize(-FragPos);

    // Directional light
    vec3 result = CalcDirLight(dirLight, norm, viewDir, diffTex, specTex);

    // Point light
    for (int i = 0; i < NR_POINT_LIGHTS; i++)
        result += CalcPointLight(pointLights[i], norm, FragPos, viewDir, diffTex, specTex);

    // Spot Light
    result += CalcSpotLight(spotLight, norm, FragPos, viewDir, diffTex, specTex);

    if (wireframe) {
        FragColor = vec4(1.0f, 1.0f, 1.0f, 1.0f);
    } else {
        FragColor = clamp(vec4(result, diffTex.a), 0.0f, 1.0f);
    }
}
