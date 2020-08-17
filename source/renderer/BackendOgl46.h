//
// Created by vincent on 12.08.20.
//

#ifndef NARA_BACKENDOGL46_H
#define NARA_BACKENDOGL46_H

#include "Backend.h"

class BackendOgl46 : public Backend {
 public:
  bool suitable = false;
  GLFWwindow *window{};
  BackendOgl46();
  ~BackendOgl46();

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

#endif //NARA_BACKENDOGL46_H
