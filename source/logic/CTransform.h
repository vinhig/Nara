//
// Created by vincent on 11.09.20.
//

#ifndef NARA_SOURCE_LOGIC_CTRANSFORM_H_
#define NARA_SOURCE_LOGIC_CTRANSFORM_H_

#include <glm/common.hpp>
#include <glm/glm.hpp>

#include "Component.h"

class CTransform : public IComponent {
 private:
  glm::vec3 position;
  glm::vec3 rotation;
  glm::vec3 scale;

  bool updated = false;

 public:
  CTransform(/* args */);
  ~CTransform();

  glm::mat4 Model();

  void Initialize() override {};

  void SetPosition(glm::vec3 position);
  void SetRotation(glm::vec3 rotation);
  void SetScale(glm::vec3 scale);
};

#endif  // NARA_SOURCE_LOGIC_CTRANSFORM_H_