//
// Created by vincent on 12.09.20.
//

#include "CCamera.h"

#include <string.h>

#include <glm/common.hpp>
#include <glm/gtx/transform.hpp>

#include "../renderer/Settings.h"

void CCamera::Initialize() {
  std::cout << "INITIATING CCamera" << std::endl;
  // Default
  this->fov = 70;
  this->position = glm::vec3(4.0f, 4.0f, 4.0f);
  this->lookAt = glm::vec3(0.0f, 0.0f, 0.0f);

  this->view =
      glm::lookAt(this->position, this->lookAt, glm::vec3(0.0f, 1.0f, 0.0f));
  this->projection = glm::perspective(
      (float)glm::radians(this->fov),
      (float)Settings::width / (float)Settings::height, 0.1f, 100.0f);
  memcpy(&uniform.projection[0], &this->projection[0][0], 16 * sizeof(float));
  memcpy(&uniform.view[0], &this->view[0][0], 16 * sizeof(float));
  this->SetActive(true);
}

void CCamera::Update() {
  if (this->updated) {
    this->vectorUpdate.lock();
    this->view =
        glm::lookAt(this->position, this->lookAt, glm::vec3(0.0f, 1.0f, 0.0f));
    this->projection = glm::perspective(
        (float)glm::radians(this->fov),
        (float)Settings::width / (float)Settings::height, 0.1f, 100.0f);

    memcpy(&this->uniform.projection[0], &this->projection[0][0],
           16 * sizeof(float));
    memcpy(&this->uniform.view[0], &this->view[0][0], 16 * sizeof(float));

    this->vectorUpdate.unlock();
    this->updated = false;
    this->uploaded = false;
  }
}

void CCamera::SetPosition(glm::vec3 position) {
  this->vectorUpdate.lock();
  this->position = position;
  this->vectorUpdate.unlock();
}

void CCamera::SetLookAt(glm::vec3 lookAt) {
  this->vectorUpdate.lock();
  this->lookAt = lookAt;
  this->vectorUpdate.unlock();
}

DrawCall CCamera::Draw() {
  DrawCall call = {};
  call.subtype = DrawCallType::NoneDrawCall;

  return call;
};