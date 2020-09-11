//
// Created by vincent on 11.09.20.
//

#ifndef NARA_SOURCE_LOGIC_CTRANSFORM_H_
#define NARA_SOURCE_LOGIC_CTRANSFORM_H_

#include <glm/glm.hpp>

#include "Ecs.h"

class CTransform : public IComponent {
 private:
  struct UniformStruct {
    float model[16];
  };

  glm::vec3 position;
  glm::vec3 rotation;
  glm::vec3 scale;

  glm::mat4 model;

  uint32_t ubo;
  UniformStruct uniform = {};

  bool updated = false;

 public:
  CTransform(Entity* p_entity) : IComponent(p_entity){};
  ~CTransform(){};

  static const uint64_t uuid = 1;
  const uint64_t UUID() override { return this->uuid; };

  glm::mat4 Model();

  /**
   * Logic initialization.
   */
  void Initialize() override;
  /**
   * Logic update.
   */
  void Update() override;

  void SetPosition(glm::vec3 position);
  void SetRotation(glm::vec3 rotation);
  void SetScale(glm::vec3 scale);

  /**
   * Render initialization.
   * Need access to the current device for GPU manipulation.
   */
  template <typename D>
  void Initialize(D* device) {
    // Loading uniform buffer
    device->CreateUbo((void*)this->uniform, sizeof(UniformStruct));
  };

  /**
   * Render update.
   * Need access to the current device for GPU manipulation.
   */
  template <typename D>
  void Update(D* device) {
    // Loading uniform buffer
    device->CreateUbo((void*)this->uniform, sizeof(UniformStruct));
  };
};

#endif  // NARA_SOURCE_LOGIC_CTRANSFORM_H_