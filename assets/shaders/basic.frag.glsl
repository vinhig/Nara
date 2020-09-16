#version 460 core
out vec3 color;

in vec3 o_color;
in vec2 o_uv;

// From material parameters
layout(binding = 0) uniform sampler2D diffuseTexture;
layout(binding = 1) uniform sampler2D normalTexture;

// From rendergraph parameters
layout(binding = 2) uniform sampler2D depthTexture;

void main() {
  color = (1 - o_uv.x * o_uv.y) * texture(diffuseTexture, o_uv).xyz +
          (o_uv.x * o_uv.y) * texture(normalTexture, o_uv).xyz;
}
