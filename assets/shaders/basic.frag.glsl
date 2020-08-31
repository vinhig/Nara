#version 330 core
out vec3 color;

in vec3 mcolor;
uniform sampler2D ourTexture;

void main() { color = mcolor; }
