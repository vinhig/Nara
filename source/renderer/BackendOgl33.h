//
// Created by vincent on 12.08.20.
//

#ifndef NARA_BACKEND_H
#define NARA_BACKEND_H

#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "Backend.h"

class BackendOgl33 : public Backend {
 public:
  bool suitable = false;
  GLFWwindow *window{};
  BackendOgl33();
  ~BackendOgl33();

  // API methods
  void ClearColor(float red, float green, float blue, float alpha) override;
  void Clear(bool color_buffer, bool depth_buffer) override;

  // Device methods
  void Init() override;
  bool IsOpen() override;
  bool IsSuitable() override;
  void Destroy() override;
  std::string Name() override;
  void SwapBuffers() override;
};

#endif //NARA_BACKEND_H
