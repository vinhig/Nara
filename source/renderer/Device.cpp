//
// Created by vincent on 12.08.20.
//

#include "Device.h"

template<typename T>
Device<T>::Device() = default;

template<typename T>
Device<T>::~Device() {
  // No need to check if gl is null
  delete gl;
};

template<typename T>
int Device<T>::InitBackend() {
  this->gl = new T();
  this->gl->Init();

  return !this->gl->IsSuitable();
}

template<typename T>
bool Device<T>::IsOpen() {
  return this->gl->IsOpen();
}

template<typename T>
bool Device<T>::IsSuitable() {
  return this->gl->IsSuitable();
}

template<typename T>
void Device<T>::PollEvents() {
  glfwPollEvents();
}

template<typename T>
void Device<T>::Clear() {
  this->gl->Clear(this->clear_args_.color_buffer, this->clear_args_.depth_buffer);
}

template<typename T>
void Device<T>::SetClearArgs(struct ClearArgs args) {
  this->gl->ClearColor(args.color[0], args.color[1], args.color[2], args.color[3]);
  // TODO: have to bind the corresponding framebuffer
  this->clear_args_ = args;
}

template<typename T>
uint32_t Device<T>::CreateVao() {
  return 0;
}

template<typename T>
uint32_t Device<T>::CreateVbo(float *data, size_t length) {
  auto buffer = this->gl->CreateBuffer(data, length * sizeof(float));
  return buffer;
}

template<typename T>
uint32_t Device<T>::CreateIbo(int *data, size_t length) {
  auto buffer = this->gl->CreateBuffer(data, length * sizeof(unsigned int));
  return 0;
}

template<typename T>
void Device<T>::Swap() {
  this->gl->SwapBuffers();
}