//
// Created by vincent on 12.08.20.
//

#ifndef NARA_SOURCE_RENDERER_BACKEND0GL33_H
#define NARA_SOURCE_RENDERER_BACKEND0GL33_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>

#include "../common/Array.h"
#include "../common/File.h"
#include "Args.h"
#include "Backend.h"

class BackendOgl33 : public Backend {
 public:
  bool suitable = false;
  GLFWwindow *window{};
  BackendOgl33();
  ~BackendOgl33();

  // API methods
  unsigned int CreateBuffer(void *data, GLBType bufferType,
                            size_t size) override;
  void ClearColor(float red, float green, float blue, float alpha) override;
  void Clear(bool color_buffer, bool depth_buffer) override;
  uint32_t CreateVao(InputLayoutArgs inputLayout) override;
  uint32_t CreateProgram(std::string vertexShader,
                         std::string fragmentShader) override;
  void DrawSingle(uint32_t vao, uint32_t ibo, int count) override;
  void UseProgram(uint32_t program) override;

  // Device methods
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
  }
};

#endif  // NARA_SOURCE_RENDERER_BACKEND0GL33_H
