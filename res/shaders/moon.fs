#version 410

out vec4 FragColor;

uniform sampler2D texture_diffuse0;
uniform sampler2D texture_specular0;

uniform vec3 lightColor;

in vec3 Normal;
in vec2 TexCoord;

void main() {
    FragColor = texture(texture_diffuse0, TexCoord) * vec4(lightColor, 1.0f);
}
