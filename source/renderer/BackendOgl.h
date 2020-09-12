//
// Created by vincent on 12.08.20.
//

#ifndef NARA_SOURCE_RENDERER_BACKEND0GL_H
#define NARA_SOURCE_RENDERER_BACKEND0GL_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>

#include "../common/Array.h"
#include "../common/File.h"
#include "Args.h"
#include "Backend.h"

class BackendOgl : public Backend {
 private:
  int width, height;

  uint32_t previousIbo = 0;
  uint32_t previousTexture = 0;
  uint32_t previousUniform = 0;
  uint32_t previousVao = 0;

 public:
  bool suitable = false;
  GLFWwindow *window{};
  BackendOgl();
  ~BackendOgl();

  // API methods
  uint32_t CreateBuffer(void *data, size_t size) override;
  void ClearColor(float red, float green, float blue, float alpha) override;
  void Clear(bool color_buffer, bool depth_buffer) override;
  uint32_t CreateVao(InputLayoutArgs inputLayout) override;
  uint32_t CreateProgram(std::string vertexShader,
                         std::string fragmentShader) override;
  uint32_t CreateTexture(TextureSpec textureSpec) override;
  void DrawSingle(uint32_t vao, uint32_t ibo, Array<uint32_t> *textures,
                  Array<uint32_t> *uniforms, int count) override;
  void DrawInstanced(uint32_t vao, uint32_t ibo, Array<uint32_t> *textures,
                     Array<uint32_t> *uniforms, int count,
                     int primcount) override;
  void FeedTexture(unsigned char *data) override;
  void UpdateBuffer(uint32_t buffer, void *data, size_t size) override;
  void UseProgram(uint32_t program) override;

  // Device methods
  int Height() override { return this->height; };
  void Init() override;
  bool IsOpen() override;
  bool IsSuitable() override;
  Features Compatible() const {
    Features current = {false, 32};
    return current;
  }
  void Destroy() override;
  std::string Name() override;
  void SwapBuffers() override;
  static void GLAPIENTRY MessageCallback(GLenum source, GLenum type, GLuint id,
                                         GLenum severity, GLsizei length,
                                         const GLchar *message,
                                         const void *userParam) {
    fprintf(stderr,
            "GL CALLBACK: %s type = 0x%x, severity = 0x%x, message = %s\n",
            (type == GL_DEBUG_TYPE_ERROR ? "** GL ERROR **" : ""), type,
            severity, message);
    if (type == GL_DEBUG_TYPE_ERROR) {
      throw std::runtime_error("An OpenGL occured. See logs above.");
    }
  }
  int Width() override { return this->width; };
};

#endif  // NARA_SOURCE_RENDERER_BACKEND0GL_H
