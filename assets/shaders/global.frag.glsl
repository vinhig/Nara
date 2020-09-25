#version 450 core

layout(location = 0) out vec3 normalColor;
layout(location = 1) out vec3 positionColor;

layout(binding = 0) uniform sampler2D depthTexture;

in vec4 o_normal;
in vec4 o_position;

vec3 computeNormalFromDepth(vec2 texCoords) {
  float depth = texture(depthTexture, texCoords).r;

  vec2 offset1 = vec2(0.0, 0.001);
  vec2 offset2 = vec2(0.001, 0.0);

  float depth1 = texture(depthTexture, texCoords + offset1).r;
  float depth2 = texture(depthTexture, texCoords + offset2).r;

  vec3 p1 = vec3(offset1, depth1 - depth);
  vec3 p2 = vec3(offset2, depth2 - depth);

  vec3 normal = cross(p1, p2);
  normal.z = -normal.z;

  return normalize(normal);
}

void main() {
  float width = 1024;
  float height = 768;

  // Screen position to texture coordinates
  vec2 texCoords = vec2(gl_FragCoord.x / width, gl_FragCoord.y / height);
  // normalColor = computeNormalFromDepth(texCoords);
  normalColor = normalize(o_normal.xyz);
  positionColor = o_position.xyz;
}