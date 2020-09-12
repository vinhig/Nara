//
// Created by vincent on 11.09.20.
//

#include "CMeshInstance.h"

#include "CCamera.h"

void CMeshInstance::Initialize() {
  std::cout << "INITIATING CMeshInstance" << std::endl;
  // Need a transform
  this->transform = this->entity->GetOrCreate<CTransform>();
  this->transform->Initialize();

  // Need a material
  this->material = this->entity->GetOrCreate<CMaterial>();
}

DrawCall CMeshInstance::Draw() {
  // Generating a draw call
  DrawCall call = {};
  call.subtype = DrawCallType::SingleDrawCall;
  DCSingle actualCall = {};

  // Fetch information for this mesh
  actualCall.vao = this->vao;
  actualCall.ibo = this->ibo;
  actualCall.uniforms = new Array<uint32_t>(2);
  actualCall.count = this->count;

  // Fetch information for the uniform buffer
  // From CTransform
  actualCall.uniforms->Add(this->entity->CorrespondingSystem()
                               ->GetFirstActive<CCamera>()
                               ->Uniform());

  actualCall.uniforms->Add(this->transform->Uniform());

  // Fetch textures
  actualCall.textures = this->material->Textures();

  // Feed the union
  call.single = actualCall;

  return call;
}