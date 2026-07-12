#version 410 core

out vec4 fragColor;

uniform sampler2D texture_diffuse0;
uniform sampler2D texture_specular0;

in vec2 texCoord;

void main() {
    float ambient = 0.2f;
    vec4 lightColor = vec4(1.0f, 1.0f, 1.0f, 1.0f);

    fragColor = texture(texture_diffuse0, texCoord) * lightColor * ambient;
}
