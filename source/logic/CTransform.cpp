//
// Created by vincent on 11.09.20.
//

#include "CTransform.h"

#include <string.h>

#include <glm/gtx/transform.hpp>

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
    this->vectorUpdate.lock();
    auto translation = glm::translate(this->position);
    auto scale = glm::scale(this->scale);
    auto rotationX = glm::rotate(glm::radians(this->rotation.x),
                                 glm::vec3(1.0f, 0.0f, 0.0f));
    auto rotationY = glm::rotate(glm::radians(this->rotation.y),
                                 glm::vec3(0.0f, 1.0f, 0.0f));
    auto rotationZ = glm::rotate(glm::radians(this->rotation.z),
                                 glm::vec3(0.0f, 0.0f, 1.0f));
    auto rotation = rotationX * rotationY * rotationZ;
    this->model = translation * rotation * scale;
    memcpy(&this->uniform.model[0], &this->model[0][0], 16 * sizeof(float));

    this->vectorUpdate.unlock();
    this->uploaded = false;
    this->updated = false;
  }
}

void CTransform::SetPosition(glm::vec3 position) {
  this->vectorUpdate.lock();
  this->position = position;
  this->updated = true;
  this->vectorUpdate.unlock();
};
void CTransform::SetRotation(glm::vec3 rotation) {
  this->vectorUpdate.lock();
  this->rotation = rotation;
  this->updated = true;
  this->vectorUpdate.unlock();
};
void CTransform::SetScale(glm::vec3 scale) {
  this->vectorUpdate.lock();
  this->scale = scale;
  this->updated = true;
  this->vectorUpdate.unlock();
};

DrawCall CTransform::Draw() {
  DrawCall call = {};
  call.subtype = DrawCallType::NoneDrawCall;

  return call;
};