//
// Created by vincent on 24.08.20.
//

#ifndef NARA_SOURCE_RENDERER_FRAME_H_
#define NARA_SOURCE_RENDERER_FRAME_H_

#include "../common/Array.h"

// DC stand for draw call

/**
 * Dummy draw call referencing :
 * - texture
 * - vao
 * - uniform buffer
 */
struct DCSingle {
  uint32_t vao;
  uint32_t ibo;
  uint32_t texture;
  int count;
  // TODO next boring stuff
};
typedef struct DCSingle DCSingle;

/**
 * Advanced draw call referencing :
 * - texture array
 * - vao
 * - uniform buffers array
 */
struct DCInstanced {
  // TODO next boring stuff
  int dummy;
};
typedef struct DCInstanced DCInstanced;

/**
 * Frame register actions to be executed in the next frame.
 */
class Frame {
 private:
  // Corresponding program to bind
  // for single draw call
  uint32_t programSingle;

 public:
  Frame();
  ~Frame();

  // Draw calls
  Array<DCSingle> singleDrawCalls;
  Array<DCInstanced> instancedDrawCalls;

  // Manipulate draw calls
  /**
   * Add job that doesn't need to be treated in a specific order.
   */
  void AddAsyncJob();
  /**
   * Add job that need to be treated in a specific order.
   */
  void QueueSyncJob();

  void SetProgram(uint32_t program) { this->programSingle = program; }
  uint32_t GetProgram() { return this->programSingle; }

  void AddDCSingle(DCSingle singleDrawCall);
  void AddDCInstanced(DCInstanced instancedDrawCall);
};

#endif  // NARA_SOURCE_RENDERER_FRAME_H_