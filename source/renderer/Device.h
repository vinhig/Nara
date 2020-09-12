//
// Created by vincent on 12.08.20.
//

#ifndef NARA_DEVICE_H
#define NARA_DEVICE_H

#include "Args.h"
#include "BackendOgl.h"
#include "Frame.h"

template <typename T>
class Device {
 private:
  Backend *gl;
  // Clear ARGS
  ClearArgs clear_args_;

 public:
  Device();
  ~Device();
  int InitBackend();
  bool IsOpen();
  bool IsSuitable();
  void PollEvents();
  int Width() { return this->gl->Width(); };
  int Height() { return this->gl->Height(); };

  // Device method ACT
  void Clear();
  uint32_t CreateIbo(unsigned int *data, size_t length);
  uint32_t CreateUbo(void *data, size_t size);
  uint32_t CreateVao(InputLayoutArgs inputLayout);
  uint32_t CreateVbo(float *data, size_t length);
  uint32_t CreateProgram(std::string name);
  uint32_t CreateTexture(TextureSpec textureSpec);
  /**
   * Erase previous frame and init new one.
   */
  Frame *SpawnFrame();
  void UpdateUbo(uint32_t buffer, void *data, size_t size);
  /**
   * Consume current frame.
   */
  void EatFrame(Frame *frame);
  void Swap();

  // Device method SET
  void SetClearArgs(struct ClearArgs args);
};

template class Device<BackendOgl>;

#endif  // NARA_DEVICE_H
