//
// Created by vincent on 24.08.20.
//

#include "Frame.h"

Frame::Frame() = default;

Frame::~Frame() {
  singleDrawCalls.Dispose();
  instancedDrawCalls.Dispose();
}

void Frame::AddAsyncJob() {}

void Frame::QueueSyncJob() {}

void Frame::AddDCSingle(DCSingle singleDrawCall) {
  this->singleDrawCalls.Add(singleDrawCall);
}

void Frame::AddDCInstanced(DCInstanced instancedDrawCall) {}