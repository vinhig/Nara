//
// Created by vincent on 17.08.20.
//

#ifndef NARA_SOURCE_RENDERER_BACKEND_H_
#define NARA_SOURCE_RENDERER_BACKEND_H_

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>

#include "../common/Array.h"
#include "../renderer/Args.h"

struct Features {
  bool spirv;
  int maxTextures;
};

class Backend {
 public:
  // API methods
  virtual void BlitRenderTarget(uint32_t from, uint32_t to, int srcX0,
                                int srcY0, int srcX1, int srcY1, int dstX0,
                                int dstY0, int dstX1, int dstY1) = 0;
  virtual void ClearColor(float red, float green, float blue, float alpha) = 0;
  virtual void Clear(bool color_buffer, bool depth_buffer) = 0;
  virtual unsigned int CreateBuffer(void* data, size_t size) = 0;
  virtual uint32_t CreateVao(InputLayoutArgs inputLayout) = 0;
  virtual uint32_t CreateProgram(std::string vertexShader,
                                 std::string fragmentShader) = 0;
  virtual uint32_t CreateRenderTarget(uint32_t colorTexture,
                                      uint32_t depthTexture) = 0;
  virtual uint32_t CreateTexture(int width, int height,
                                 InternalFormat internalFormat) = 0;
  virtual uint32_t CreateTexture(TextureSpec textureSpec) = 0;
  virtual void DrawSingle(uint32_t vao, uint32_t ibo, Array<uint32_t>* textures,
                          Array<uint32_t>* uniforms, int count) = 0;
  virtual void DrawInstanced(uint32_t vao, uint32_t ibo,
                             Array<uint32_t>* textures,
                             Array<uint32_t>* uniforms, int count,
                             int primcount) = 0;
  virtual void UpdateBuffer(uint32_t buffer, void* data, size_t size) = 0;
  virtual void UseProgram(uint32_t program) = 0;
  virtual void UseRenderTarget(RenderTarget renderTarget) = 0;
  virtual void UseUniform(uint32_t uniform) = 0;

  // Device methods
  virtual int Height() = 0;
  virtual void Init() = 0;
  virtual bool IsOpen() = 0;
  virtual bool IsSuitable() = 0;
  virtual Features Compatible() const = 0;
  virtual void Destroy() = 0;
  virtual std::string Name() = 0;
  virtual void SwapBuffers() = 0;
  virtual int Width() = 0;
};

#endif  // NARA_SOURCE_RENDERER_BACKEND_H_
