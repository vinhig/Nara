//
// Created by vincent on 17.08.20.
//

#ifndef NARA_SOURCE_RENDERER_BACKEND_H_
#define NARA_SOURCE_RENDERER_BACKEND_H_

#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

class Backend {
 public:
  // API methods
  virtual void ClearColor(float red, float green, float blue, float alpha) = 0;
  virtual void Clear(bool color_buffer, bool depth_buffer) = 0;

  // Device methods
  virtual void Init() = 0;
  virtual bool IsOpen() = 0;
  virtual bool IsSuitable() = 0;
  virtual void Destroy() = 0;
  virtual std::string Name() = 0;
  virtual void SwapBuffers() = 0;
};

#endif //NARA_SOURCE_RENDERER_BACKEND_H_
