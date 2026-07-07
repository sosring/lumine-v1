#version 330 core

out vec4 fragColor;

uniform sampler2D tex0;
uniform sampler2D tex1;

in vec3 color;
in vec2 texCoord;

void main() {
    fragColor = mix(texture(tex0, texCoord), texture(tex1, texCoord), 0.5f);
}
