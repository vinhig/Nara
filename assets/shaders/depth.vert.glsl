#version 450 core

layout(location = 0) in vec3 position;

layout(std140, binding = 0) uniform Scene {
  mat4 projection;
  mat4 view;
};

layout(std140, binding = 1) uniform Object { mat4 model; };

void main() { gl_Position = projection * view * model * vec4(position, 1.0); }
