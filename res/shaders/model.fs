#version 410 core

out vec4 FragColor;

in vec2 TexCoord;
in vec3 FragPos;
in vec3 Normal;

uniform sampler2D texture_diffuse0;
uniform sampler2D texture_specular0;

struct Light {
    vec3 direction;
    vec3 position;
    vec3 color;

    float constant;
    float linear;
    float quadratic;
};

uniform float ambientIntensity;
uniform Light light;

uniform bool wireframe;

void main() {
    vec4 albedo = texture(texture_diffuse0, TexCoord);

    // Ambient Light
    vec3 ambient = ambientIntensity * albedo.rgb;

    // Diffuse Light
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(light.position - FragPos);

    float diff = max(dot(norm, lightDir), 0.0f);
    vec3 diffuse = diff * light.color;

    // Specular Highlights
    float specularStrength = texture(texture_specular0, TexCoord).r;
    vec3 viewDir = normalize(-FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);

    float spec = pow(max(dot(viewDir, reflectDir), 0.0f), 32.0f);
    vec3 specular = specularStrength * spec * light.color;

    vec3 result = (ambient + diffuse) * albedo.rgb + specular;

    if (wireframe) {
        FragColor = vec4(1.0f, 1.0f, 1.0f, 1.0f);
    } else {
        FragColor = clamp(vec4(result, albedo.a), 0.0f, 1.0f);
    }
}
