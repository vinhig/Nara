//
// Created by vincent on 13.09.20.
//

#ifndef NARA_SOURCE_RENDERER_RENDERGRAPH_H_
#define NARA_SOURCE_RENDERER_RENDERGRAPH_H_

#include <cstdint>
#include <glm/glm.hpp>
#include <optional>
#include <random>
#include <vector>

#include "Args.h"
#include "Frame.h"
#include "Settings.h"

// float lerp(float a, float b, float f) { return a + f * (b - a); }

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

  RenderTarget globalPrepass;

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

  // Shaders
  uint32_t basicProgram;
  uint32_t ibasicProgram;
  uint32_t depthProgram;
  uint32_t globalProgram;

  // Uniform buffers
  // Set from Game state
  uint32_t pointOfView;
  std::vector<uint32_t> lightsOfView;

  // Render targets textures
  std::vector<uint32_t> forGlobalTextures;

  float SSAOSamples[64];
  uint32_t SSAONoise;

  /*template <typename D>
  void PrepareSSAO(D* device) {
    std::uniform_real_distribution<float> randomFloats(
        0.0, 1.0);  // random floats between [0.0, 1.0]
    std::default_random_engine generator;
    std::vector<glm::vec3> ssaoKernel;
    for (unsigned int i = 0; i < 64; ++i) {
      glm::vec3 sample(randomFloats(generator) * 2.0 - 1.0,
                       randomFloats(generator) * 2.0 - 1.0,
                       randomFloats(generator));
      sample = glm::normalize(sample);
      float scale = (float)i / 64.0;
      scale = lerp(0.1f, 1.0f, scale * scale);
      sample *= scale;
      ssaoKernel.push_back(sample);
    }

    std::vector<char> ssaoNoise;
    ssaoNoise.reserve(16 * 3);
    // std::vector<glm::vec3> ssaoNoise;
    for (unsigned int i = 0; i < 16; i++) {
      ssaoNoise.push_back((char)randomFloats(generator) * 2.0f - 1.0f);
      ssaoNoise.push_back((char)randomFloats(generator) * 2.0f - 1.0f);
      ssaoNoise.push_back(0);
    }

    // Create a 4x4 texture
    TextureSpec texture = {};
    texture.width = 4;
    texture.height = 4;
    texture.data = data.data();
    texture.format = TextureFormat::RGB;
    texture.wrap = TextureWrap::Repeat;
    device->CreateTexture(texture);
  }*/

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
    std::cout << "Depth prepass creation" << std::endl;
    this->depthPrepass =
        device->CreateRenderTarget({Settings::width, Settings::height, false,
                                    std::nullopt, true, false, true});
    std::vector<InternalFormat> globalColorBufferFormats;
    globalColorBufferFormats.push_back(InternalFormat::RGB8);  // Normal
    globalColorBufferFormats.push_back(InternalFormat::RGB8);  // Position
    globalColorBufferFormats.push_back(InternalFormat::R8);    // SSAO
    // Describe and create render target
    RenderTargetArgs rtDesc = {};
    rtDesc.width = Settings::width;
    rtDesc.height = Settings::height;
    rtDesc.nbColors = (int)globalColorBufferFormats.size();
    rtDesc.formats = globalColorBufferFormats;
    rtDesc.depth = true;
    rtDesc.clearColor = true;
    rtDesc.clearDepth = true;
    std::cout << "Global prepass creation" << std::endl;
    this->globalPrepass = device->CreateRenderTarget(rtDesc);

    std::vector<InternalFormat> diffuseColorBufferFormats;
    diffuseColorBufferFormats.push_back(InternalFormat::RGB8);
    std::cout << "Diffuse prepass creation" << std::endl;
    this->diffusePass = device->CreateRenderTarget(
        {Settings::width, Settings::height, 1, diffuseColorBufferFormats, true,
         true, true});

    // Extract textures from render targets
    this->forGlobalTextures.push_back(this->depthPrepass.depthTexture);

    // Loading shaders
    this->basicProgram = device->CreateProgram("assets/shaders/basic");
    this->ibasicProgram = device->CreateProgram("assets/shaders/ibasic");
    this->depthProgram = device->CreateProgram("assets/shaders/depth");
    this->globalProgram = device->CreateProgram("assets/shaders/global");

    // Compute sample and texture for SSAO
    // this->PrepareSSAO<D>(device);
  }

  template <typename D>
  void Draw(D* device) {
    std::cout << "RenderGraph->Draw()" << std::endl;
    // Compose and eat frame
    Frame* depthFrame = device->SpawnFrame();

    depthFrame->SetPointOfView(this->pointOfView);

    depthFrame->singleDrawCalls = this->singleCalls.data();
    depthFrame->singleDrawCallsCount = this->singleCalls.size();
    depthFrame->instancedDrawCalls = this->instancedCalls.data();
    depthFrame->instancedDrawCallsCount = this->instancedCalls.size();

    depthFrame->SetProgramSingle(this->depthProgram);
    depthFrame->SetProgramInstanced(this->depthProgram);

    depthFrame->SetRenderTarget(this->depthPrepass);

    std::cout << "(Depth Frame)" << std::endl;

    device->EatFrame(depthFrame);

    Frame* globalFrame = device->SpawnFrame();

    globalFrame->SetPointOfView(this->pointOfView);
    globalFrame->SetTextures(forGlobalTextures);

    globalFrame->singleDrawCalls = this->singleCalls.data();
    globalFrame->singleDrawCallsCount = this->singleCalls.size();
    globalFrame->instancedDrawCalls = this->instancedCalls.data();
    globalFrame->instancedDrawCallsCount = this->instancedCalls.size();

    globalFrame->SetProgramSingle(this->globalProgram);
    globalFrame->SetProgramInstanced(this->globalProgram);

    globalFrame->SetRenderTarget(this->globalPrepass);

    std::cout << "(Global Frame)" << std::endl;

    device->EatFrame(globalFrame);

    Frame* diffuseFrame = device->SpawnFrame();

    diffuseFrame->SetPointOfView(this->pointOfView);

    diffuseFrame->singleDrawCalls = this->singleCalls.data();
    diffuseFrame->singleDrawCallsCount = this->singleCalls.size();
    diffuseFrame->instancedDrawCalls = this->instancedCalls.data();
    diffuseFrame->instancedDrawCallsCount = this->instancedCalls.size();

    diffuseFrame->SetProgramSingle(this->basicProgram);
    diffuseFrame->SetProgramInstanced(this->ibasicProgram);

    diffuseFrame->SetRenderTarget(this->diffusePass);

    std::cout << "(Diffuse Frame)" << std::endl;
    device->EatFrame(diffuseFrame);

    device->BlitOnScreen(this->diffusePass);

    singleCalls.clear();
    instancedCalls.clear();
  }

  void SetPointOfView(uint32_t ubo) {
    if (!ubo || ubo > 100) {
      throw std::runtime_error("Suspect uniform buffer object.");
    }
    this->pointOfView = ubo;
  }
};

#endif  // NARA_SOURCE_RENDERER_RENDERGRAPH_H_