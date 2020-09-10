//
// Created by vincent on 13.08.20.
//

#ifndef NARA_SOURCE_LOGIC_GAME_H_
#define NARA_SOURCE_LOGIC_GAME_H_

#include "../renderer/BackendOgl33.h"
#include "../renderer/Device.h"

template <class T>
class Game {
 private:
  int width, height;

 public:
  Game();
  ~Game();
  Device<T> *device = nullptr;
  int SetDevice(Device<T> *p_device);
  void Update();
  void Run();
};

template class Game<BackendOgl33>;
template class Game<BackendOgl46>;

#endif  // NARA_SOURCE_LOGIC_GAME_H_
