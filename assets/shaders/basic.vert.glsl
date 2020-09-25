#version 450 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 color;
layout(location = 2) in vec2 uv;

out vec3 o_color;
out vec2 o_uv;

layout(std140, binding = 0) uniform Scene {
  mat4 projection;
  mat4 view;
};

layout(std140, binding = 1) uniform Object { mat4 model; };

void main() {
  o_color = color;
  o_uv = uv;
  gl_Position = projection * view * model * vec4(position, 1.0);
}
