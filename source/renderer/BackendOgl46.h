//
// Created by vincent on 12.08.20.
//

#ifndef NARA_SOURCE_RENDERER_BACKEND0GL46_H
#define NARA_SOURCE_RENDERER_BACKEND0GL46_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>

#include "../common/Array.h"
#include "Args.h"
#include "Backend.h"
#include "BackendOgl33.h"

class BackendOgl46 : public Backend {
 public:
  bool suitable_ = false;
  GLFWwindow* window{};
  BackendOgl46();
  ~BackendOgl46();

  // API methods
  unsigned int CreateBuffer(void* data, GLBType bufferType,
                            size_t size) override;
  void ClearColor(float red, float green, float blue, float alpha) override;
  void Clear(bool color_buffer, bool depth_buffer) override;
  uint32_t CreateVao(InputLayoutArgs inputLayout) override { return 0; };
  uint32_t CreateProgram(std::string vertexShader,
                         std::string fragmentShader) override {
    return 0;
  }
  void DrawSingle(uint32_t vao) override{};

  // Device methods
  void Init() override;
  bool IsOpen() override;
  bool IsSuitable() override;
  Features Compatible() const {
    Features current = {true, 32};
    return current;
  }
  void Destroy() override;
  std::string Name() override;
  void SwapBuffers() override;
};

#endif  // NARA_SOURCE_RENDERER_BACKEND0GL46_H
