//
// Created by vincent on 13.08.20.
//

#include "Game.h"

#include "../common/Macros.h"

template <class T>
Game<T>::Game() = default;

template <class T>
Game<T>::~Game() = default;

template <class T>
int Game<T>::SetDevice(Device<T> *p_device) {
  if (p_device && p_device->IsSuitable()) {
    this->device = p_device;
    return 0;
  } else {
    return 1;
  }
}

template <class T>
void Game<T>::Update() {}

template <class T>
void Game<T>::Run() {
  float vertices[] = {
      -1.0f, 1.0f, 0.0f, 1.0f, 1.0f, 0.0, 1.0f, -1.0f, 0.0f, -1.0f, -1.0f, 0.0f,
  };
  int indices[] = {0, 1, 2, 2, 3, 0};
  uint32_t vbo = this->device->CreateVbo(vertices, _countof(vertices));
  // uint32_t ibo = this->device->CreateIbo(indices, _countof(indices));

  InputLayoutArgs inputLayout = {};
  Array<InputLayoutEntryArgs> entries(3);
  entries.Add({0, 3, false, 0, vbo, GLCType::GLFLoat});
  inputLayout.entries = entries;

  uint32_t vao = this->device->CreateVao(inputLayout);

  uint32_t basicProgram = this->device->CreateProgram("assets/shaders/basic");

  while (this->device->IsOpen()) {
    this->Update();
    /*ClearArgs args = {.framebuffer = 0,
                      .color = {1.0f, 0.0f, 1.0f, 1.0f},
                      .color_buffer = true,
                      .depth_buffer = true};*/
    this->device->SetClearArgs({0, {1.0f, 0.0f, 1.0f, 1.0f}, true, true});
    this->device->Clear();

    Frame *currentFrame = this->device->SpawnFrame();
    currentFrame->AddDCSingle({vao});
    // Some work on the frame
    this->device->EatFrame();

    this->device->Swap();
    this->device->PollEvents();
  }
}
