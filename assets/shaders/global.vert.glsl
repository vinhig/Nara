#version 450 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;

layout(std140, binding = 0) uniform Scene {
  mat4 projection;
  mat4 view;
};

layout(std140, binding = 1) uniform Object { mat4 model; };

// layout(location = 0) out vec4 o_position;

out vec4 o_normal;
out vec4 o_position;

void main() {
  mat4 mvp = projection * view * model;
  vec4 pos = mvp * vec4(position, 1.0);
  o_position = pos;
  gl_Position = pos;
  mvp = transpose(inverse(model));
  o_normal = mvp * vec4(normal, 1.0);
}
