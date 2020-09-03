#version 330 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 color;
layout(location = 2) in vec2 uv;

out vec3 o_color;
out vec2 o_uv;

void main() {
  o_color = color;
  o_uv = uv;
  float offset = gl_InstanceID / 10.0;
  gl_Position = vec4(position + vec3(offset, offset, 0), 1.0);
}
