//
// Created by vincent on 13.09.20.
//

#ifndef NARA_SOURCE_RENDERER_RENDERGRAPH_H_
#define NARA_SOURCE_RENDERER_RENDERGRAPH_H_

#include <cstdint>
#include <vector>

#include "Args.h"
#include "Frame.h"
#include "Settings.h"

class RenderGraph {
 private:
  // Prepass render target
  /**
   * Depth pre-pass from player view
   */
  RenderTarget depthPrepass;
  /**
   * Depth pre-pass from ligths view
   */
  RenderTarget lightPrepass[3];

  // Color render target
  /**
   * Diffuse pass.
   * Depends on depthPrepass and lightPrepass[0->3]
   */
  RenderTarget diffusePass;

  // Post effect render target
  /**
   * Distortion (eg: heat 'waves')
   */
  RenderTarget distortionPass;

  uint32_t basicProgram;
  uint32_t ibasicProgram;
  uint32_t depthProgram;

 public:
  std::vector<DCSingle> singleCalls;
  std::vector<DCInstanced> instancedCalls;

  RenderGraph(/* args */);
  ~RenderGraph();

  template <typename D>
  void Initialize(D* device) {
    singleCalls.reserve(128);
    instancedCalls.reserve(128);
    // Create a render target for each render target
    this->depthPrepass = device->CreateRenderTarget(
        {Settings::width, Settings::height, false, true, false, true});

    this->diffusePass = device->CreateRenderTarget(
        {Settings::width, Settings::height, true, true, true, true});

    this->basicProgram = device->CreateProgram("assets/shaders/basic");
    this->ibasicProgram = device->CreateProgram("assets/shaders/ibasic");
    this->depthProgram = device->CreateProgram("assets/shaders/depth");
  }

  template <typename D>
  void Draw(D* device) {
    // Compose and eat frame
    Frame* depthFrame = device->SpawnFrame();

    depthFrame->singleDrawCalls = this->singleCalls.data();
    depthFrame->singleDrawCallsCount = this->singleCalls.size();
    depthFrame->instancedDrawCalls = this->instancedCalls.data();
    depthFrame->instancedDrawCallsCount = this->instancedCalls.size();

    depthFrame->SetProgramSingle(this->depthProgram);
    depthFrame->SetProgramInstanced(this->depthProgram);

    depthFrame->SetRenderTarget(this->depthPrepass);

    device->EatFrame(depthFrame);

    Frame* diffuseFrame = device->SpawnFrame();

    diffuseFrame->singleDrawCalls = this->singleCalls.data();
    diffuseFrame->singleDrawCallsCount = this->singleCalls.size();
    diffuseFrame->instancedDrawCalls = this->instancedCalls.data();
    diffuseFrame->instancedDrawCallsCount = this->instancedCalls.size();

    diffuseFrame->SetProgramSingle(this->basicProgram);
    diffuseFrame->SetProgramInstanced(this->ibasicProgram);

    diffuseFrame->SetRenderTarget(this->diffusePass);

    device->EatFrame(diffuseFrame);

    device->BlitOnScreen(this->diffusePass);

    singleCalls.clear();
    instancedCalls.clear();
  }
};

#endif  // NARA_SOURCE_RENDERER_RENDERGRAPH_H_