//
// Created by vincent on 12.08.20.
//

#ifndef NARA_DEVICE_H
#define NARA_DEVICE_H

#include "Args.h"
#include "BackendOgl33.h"
#include "BackendOgl46.h"
#include "Frame.h"

template <typename T>
class Device {
 private:
  Backend *gl;
  // Clear ARGS
  ClearArgs clear_args_;

  Frame *currentFrame;

 public:
  Device();
  ~Device();
  int InitBackend();
  bool IsOpen();
  bool IsSuitable();
  void PollEvents();

  // Device method ACT
  void Clear();
  uint32_t CreateVao(InputLayoutArgs inputLayout);
  uint32_t CreateVbo(float *data, size_t length);
  uint32_t CreateIbo(int *data, size_t length);
  uint32_t CreateProgram(std::string name);
  /**
   * Erase previous frame and init new one.
   */
  Frame *SpawnFrame();
  /**
   * Consume current frame.
   */
  void EatFrame();
  void Swap();

  // Device method SET
  void SetClearArgs(struct ClearArgs args);
};

template class Device<BackendOgl33>;
template class Device<BackendOgl46>;

#endif  // NARA_DEVICE_H
