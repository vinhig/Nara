//
// Created by vincent on 17.08.20.
//

#ifndef NARA_SOURCE_RENDERER_ARGS_H_
#define NARA_SOURCE_RENDERER_ARGS_H_

#include <GL/glew.h>

#include <vector>

#include "../common/Array.h"

/**
 * Graphics Library Content Type
 */
enum GLCType {
  GLFloat = GL_FLOAT,
  GLInt = GL_INT,
  GLUint = GL_UNSIGNED_INT,
  GLUshort = GL_UNSIGNED_SHORT,
  GLMat4,
  GLVec4,
  GLVec3,
};

/**
 * Graphics Library Buffer Type
 */
enum GLBType {
  GLArrayBuffer = GL_ARRAY_BUFFER,
  GLElementBuffer = GL_ELEMENT_ARRAY_BUFFER,
  GLUniformBuffer = GL_UNIFORM_BUFFER,
};

struct ClearArgs {
  unsigned int framebuffer;
  float color[4];
  bool color_buffer;
  bool depth_buffer;
};

/**
 * Represent vertices shader entry.
 */
struct InputLayoutEntryArgs {
  unsigned int index;
  int size;
  bool normalized;
  size_t stride;
  uint32_t buffer;
  GLCType subtype;
  void* offset;
};

struct InputLayoutArgs {
  Array<InputLayoutEntryArgs> entries;
};

enum TextureFormat {
  RGB = GL_RGB,
  RGBA = GL_RGBA,
  DEPTH = GL_DEPTH_COMPONENT,
};

enum InternalFormat {
  RGB8 = GL_RGB8,
  RGBA8 = GL_RGBA8,
  DEPTH24 = GL_DEPTH_COMPONENT24,
};

struct TextureSpec {
  int width;
  int height;
  unsigned char* data;
  TextureFormat format;
};

struct MeshSpec {
  float* vertices;
  unsigned int* indices;
  int verticesCount;
  int indicesCount;
};

struct RenderTargetArgs {
  int width, height;
  int color;
  bool depth;
  bool clearColor, clearDepth;
};

struct RenderTarget {
  uint32_t framebuffer;
  std::vector<uint32_t> colorTexture;
  uint32_t depthTexture;
  int width, height;
  bool clearColor, clearDepth;
};

#endif  // NARA_SOURCE_RENDERER_ARGS_H_
