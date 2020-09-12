//
// Created by vincent on 11.09.20.
//

#include "CTransform.h"

#include <string.h>

void CTransform::Initialize() {
  std::cout << "INITIATING CTransform" << std::endl;
  // Default
  this->position = glm::vec3(0.0f, 0.0f, 0.0f);
  this->rotation = glm::vec3(0.0f, 0.0f, 0.0f);
  this->scale = glm::vec3(1.0f, 1.0f, 1.0f);

  // Identity matrix for default
  this->model = glm::mat4(1.0f);
  memcpy(&this->uniform.model[0], &this->model[0][0], 16 * sizeof(float));
}

void CTransform::Update() {
  if (this->updated) {
    std::cout << "Updating CTransform" << std::endl;
  }
}