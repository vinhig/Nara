//
// Created by vincent on 12.08.20.
//

#include "Device.h"

template <typename T>
Device<T>::Device() = default;

template <typename T>
Device<T>::~Device() {
  // No need to check if gl is null
  delete gl;
};

template <typename T>
int Device<T>::InitBackend() {
  this->gl = new T();
  this->gl->Init();

  return !this->gl->IsSuitable();
}

template <typename T>
bool Device<T>::IsOpen() {
  return this->gl->IsOpen();
}

template <typename T>
bool Device<T>::IsSuitable() {
  return this->gl->IsSuitable();
}

template <typename T>
void Device<T>::PollEvents() {
  glfwPollEvents();
}

template <typename T>
void Device<T>::Clear() {
  this->gl->Clear(this->clear_args_.color_buffer,
                  this->clear_args_.depth_buffer);
}

template <typename T>
void Device<T>::SetClearArgs(struct ClearArgs args) {
  this->gl->ClearColor(args.color[0], args.color[1], args.color[2],
                       args.color[3]);
  // TODO: have to bind the corresponding framebuffer
  this->clear_args_ = args;
}

template <typename T>
uint32_t Device<T>::CreateIbo(unsigned int *data, size_t length) {
  auto buffer = this->gl->CreateBuffer(data, length * sizeof(unsigned int));
  return buffer;
}

template <typename T>
uint32_t Device<T>::CreateUbo(void *data, size_t size) {
  auto buffer = this->gl->CreateBuffer(data, size);
  return buffer;
}

template <typename T>
uint32_t Device<T>::CreateVao(InputLayoutArgs inputLayout) {
  return this->gl->CreateVao(inputLayout);
}

template <typename T>
uint32_t Device<T>::CreateVbo(float *data, size_t length) {
  auto buffer = this->gl->CreateBuffer(data, length * sizeof(float));
  return buffer;
}

template <typename T>
uint32_t Device<T>::CreateProgram(std::string name) {
  if (this->gl->Compatible().spirv) {
    throw std::runtime_error("Unimplemented");
  } else {
    uint32_t program =
        this->gl->CreateProgram(name + ".vert.glsl", name + ".frag.glsl");
    return program;
  }
};

template <typename T>
uint32_t Device<T>::CreateTexture(TextureSpec textureSpec) {
  return this->gl->CreateTexture(textureSpec);
}

template <typename T>
Frame *Device<T>::SpawnFrame() {
  // delete currentFrame;
  return new Frame();
}

template <typename T>
void Device<T>::EatFrame(Frame *frame) {
  this->gl->UseProgram(frame->GetProgramSingle());
  uint32_t previousUniformBuffer = 0;
  for (size_t i = 0; i < frame->singleDrawCalls.Count(); i++) {
    auto drawCall = frame->singleDrawCalls[i];
    this->gl->DrawSingle(drawCall.vao, drawCall.ibo, drawCall.textures,
                         drawCall.uniforms, drawCall.count);
  }

  this->gl->UseProgram(frame->GetProgramInstanced());
  for (size_t i = 0; i < frame->instancedDrawCalls.Count(); i++) {
    auto drawCall = frame->instancedDrawCalls[i];
    this->gl->DrawInstanced(drawCall.target.vao, drawCall.target.ibo,
                            drawCall.target.textures, drawCall.target.uniforms,
                            drawCall.target.count, drawCall.primcount);
  }
}

template <typename T>
void Device<T>::Swap() {
  this->gl->SwapBuffers();
}