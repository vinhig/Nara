#version 330 core
out vec3 color;

in vec3 o_color;
in vec2 o_uv;

uniform sampler2D diffuseTexture;

void main() { color = texture(diffuseTexture, o_uv).xyz * o_color; }
