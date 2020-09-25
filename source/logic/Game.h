//
// Created by vincent on 13.08.20.
//

#ifndef NARA_SOURCE_LOGIC_GAME_H_
#define NARA_SOURCE_LOGIC_GAME_H_

#include <deque>
#include <mutex>
#include <thread>
#include <vector>

#include "../renderer/BackendOgl.h"
#include "../renderer/Device.h"
#include "Ecs.h"

enum JobType {
  ComponentUpdate,
  SoundAction,
  FileReadWrite,
};

struct Job {
  union {
    IComponent *component;
  };
  JobType subtype;
};

template <class T>
class Game {
 private:
  int width, height;

  // uint currentFrame = 0;

  std::vector<std::thread> workers;
  std::deque<Job> jobs;

  std::recursive_mutex findAJob;

  System *world;

  bool running;

 public:
  Game();
  ~Game();
  Device<T> *device = nullptr;
  int SetDevice(Device<T> *p_device);
  void Update();
  void Run();
  void Work(int workerID);
};

template class Game<BackendOgl>;

#endif  // NARA_SOURCE_LOGIC_GAME_H_
