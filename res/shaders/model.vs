#version 410 core

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec2 aUV;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec2 TexCoord;
out vec3 FragPos;
out vec3 Normal;

void main() {
    gl_Position = projection * view * model * vec4(aPos, 1.0f);

    FragPos = vec3(view * model * vec4(aPos, 1.0f)); // World space coord
    Normal = mat3(transpose(inverse(view * model))) * aNormal; // It transforms mesh's normal correctly when model is scaled or sheared
    TexCoord = aUV;
}
