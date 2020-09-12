//
// Created by vincent on 11.09.20.
//

#include "CMeshInstance.h"

void CMeshInstance::Initialize() {
  std::cout << "INITIATING CMeshInstance" << std::endl;
  // Need a transform
  transform = this->entity->GetOrCreate<CTransform>();
  transform->Initialize();
}

DrawCall CMeshInstance::Draw() {
  // Generating a draw call
  DrawCall call = {};
  call.subtype = DrawCallType::SingleDrawCall;
  DCSingle actualCall = {};

  // Fetch information for this mesh
  actualCall.vao = this->vao;
  actualCall.ibo = this->ibo;
  actualCall.textures = new Array<uint32_t>(0);
  actualCall.uniforms = new Array<uint32_t>(1);
  actualCall.count = this->count;

  // Fetch information for the uniform buffer
  // From CTransform
  actualCall.uniforms->Add(this->transform->Uniform());

  // TODO: fetch textures and material detail

  // Feed the union
  call.single = actualCall;

  return call;
}