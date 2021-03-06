#version 450 core
out vec3 color;

in vec3 o_color;
in vec2 o_uv;

layout(binding = 0) uniform sampler2D diffuseTexture;
layout(binding = 1) uniform sampler2D normalTexture;

void main() {
  color = (1 - o_uv.x * o_uv.y) * texture(diffuseTexture, o_uv).xyz +
          (o_uv.x * o_uv.y) * texture(normalTexture, o_uv).xyz;
}
