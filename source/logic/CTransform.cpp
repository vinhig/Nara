//
// Created by vincent on 11.09.20.
//

#include "CTransform.h"

#include <string.h>

void CTransform::Initialize() {
  // Default
  this->position = glm::vec3(0.0f, 0.0f, 0.0f);
  this->rotation = glm::vec3(0.0f, 0.0f, 0.0f);
  this->scale = glm::vec3(1.0f, 1.0f, 1.0f);

  // Identity matrix for default
  this->model = glm::mat4();
  memcpy(&this->model[0], &this->model[0][0], 16 * sizeof(float));
}