//
// Created by vincent on 13.09.20.
//

#include "CTerrain.h"

#include "CCamera.h"

void CTerrain::Initialize() {
  std::cout << "INITIATING CTerrain" << std::endl;
  // Need a transform
  this->transform = this->entity->GetOrCreate<CTransform>();
  this->transform->Initialize();
}

DrawCall CTerrain::Draw() {
  // Generating a draw call
  DrawCall call = {};
  call.subtype = DrawCallType::SingleDrawCall;
  DCSingle actualCall = {};

  // Fetch information for this mesh
  actualCall.vao = this->vao;
  actualCall.ibo = this->ibo;
  actualCall.uniforms = new Array<uint32_t>(2);
  actualCall.textures = new Array<uint32_t>(0);
  actualCall.count = this->count;

  // Fetch information for the uniform buffer
  // From CTransform
  actualCall.uniforms->Add(this->entity->CorrespondingSystem()
                               ->GetFirstActive<CCamera>()
                               ->Uniform());

  actualCall.uniforms->Add(this->transform->Uniform());

  // Feed the union
  call.single = actualCall;

  return call;
}