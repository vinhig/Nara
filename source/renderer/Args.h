//
// Created by vincent on 17.08.20.
//

#ifndef NARA_SOURCE_RENDERER_ARGS_H_
#define NARA_SOURCE_RENDERER_ARGS_H_

#include <GL/glew.h>

#include "../common/Array.h"

/**
 * Graphics Library Content Type
 */
enum GLCType {
  GLFLoat = GL_FLOAT,
  GLInt = GL_INT,
  GLUint = GL_UNSIGNED_INT,
  GLUshort = GL_UNSIGNED_SHORT
};
typedef enum GLCType GLCType;

/**
 * Graphics Library Buffer Type
 */
enum GLBType {
  GLArrayBuffer = GL_ARRAY_BUFFER,
  GLElementBuffer = GL_ELEMENT_ARRAY_BUFFER,
};
typedef enum GLBType GLBType;

struct ClearArgs {
  unsigned int framebuffer;
  float color[4];
  bool color_buffer;
  bool depth_buffer;
};

/**
 * Represent shader entry.
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
};

struct TextureSpec {
  int width;
  int height;
  unsigned char* data;
  TextureFormat format;
};

#endif  // NARA_SOURCE_RENDERER_ARGS_H_
