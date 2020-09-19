//
// Created by vincent on 24.08.20.
//

#ifndef NARA_SOURCE_RENDERER_FRAME_H_
#define NARA_SOURCE_RENDERER_FRAME_H_

#include <vector>

#include "../common/Array.h"
#include "Args.h"

// DC stand for draw call

enum RenderTag {
  Level1 = 0,  // Just visible
  Level2 = 1,  // Visible and cast shadow
};

/**
 * Dummy draw call referencing :
 * - texture
 * - vao
 * - uniform buffer
 */
struct DCSingle {
  uint32_t vao;
  uint32_t ibo;
  Array<uint32_t>* textures;
  Array<uint32_t>* uniforms;
  int count;
  RenderTag level;
  // TODO next boring stuff
};

/**
 * Advanced draw call referencing :
 * - texture array
 * - vao
 * - uniform buffers array
 */
struct DCInstanced {
  // TODO next boring stuff
  DCSingle target;
  int primcount;
};

enum DrawCallType {
  InstancedDrawCall,
  SingleDrawCall,
  NoneDrawCall,
};

struct DrawCall {
  union {
    DCInstanced instanced;
    DCSingle single;
  };
  DrawCallType subtype;
};

/**
 * Frame register actions to be executed in the next frame.
 */
class Frame {
 private:
  // Corresponding program to bind
  uint32_t programSingle;
  uint32_t programInstanced;

  // Bindless ressources
  uint32_t pointOfView;
  std::vector<uint32_t> textures;

  RenderTarget renderTarget;

 public:
  Frame();
  ~Frame();

  // Draw calls
  DCSingle* singleDrawCalls;
  size_t singleDrawCallsCount;
  DCInstanced* instancedDrawCalls;
  size_t instancedDrawCallsCount;

  // Manipulate draw calls
  /**
   * Add job that doesn't need to be treated in a specific order.
   */
  void AddAsyncJob();
  /**
   * Add job that need to be treated in a specific order.
   */
  void QueueSyncJob();

  uint32_t GetPointOfView() { return this->pointOfView; };
  uint32_t GetProgramSingle() { return this->programSingle; }
  uint32_t GetProgramInstanced() { return this->programInstanced; }
  RenderTarget GetRenderTarget() { return this->renderTarget; }
  std::vector<uint32_t> GetTextures() { return this->textures; }

  void SetProgramSingle(uint32_t program) {
    // Set
    this->programSingle = program;
  }
  void SetProgramInstanced(uint32_t program) {
    this->programInstanced = program;
  }
  void SetPointOfView(uint32_t pointOfView) {
    // Here, a pointOfView may be a Camera or a Light
    this->pointOfView = pointOfView;
  }
  void SetRenderTarget(RenderTarget renderTarget) {
    this->renderTarget = renderTarget;
  };
  void SetTextures(std::vector<uint32_t> textures) {
    this->textures = textures;
  }

  // void AddDCSingle(DCSingle singleDrawCall);
  // void AddDCInstanced(DCInstanced instancedDrawCall);
};

#endif  // NARA_SOURCE_RENDERER_FRAME_H_