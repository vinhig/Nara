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
  Array<uint32_t>* textures;
  Array<uint32_t>* uniforms;
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
  DCSingle target;
  int primcount;
};
typedef struct DCInstanced DCInstanced;

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
  // for single draw call
  uint32_t programSingle;
  uint32_t programInstanced;

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

  void SetProgramSingle(uint32_t program) { this->programSingle = program; }
  void SetProgramInstanced(uint32_t program) {
    this->programInstanced = program;
  }
  uint32_t GetProgramSingle() { return this->programSingle; }
  uint32_t GetProgramInstanced() { return this->programInstanced; }

  void AddDCSingle(DCSingle singleDrawCall);
  void AddDCInstanced(DCInstanced instancedDrawCall);
};

#endif  // NARA_SOURCE_RENDERER_FRAME_H_