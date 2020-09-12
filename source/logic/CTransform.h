//
// Created by vincent on 11.09.20.
//

#ifndef NARA_SOURCE_LOGIC_CTRANSFORM_H_
#define NARA_SOURCE_LOGIC_CTRANSFORM_H_

#include <glm/glm.hpp>

#include "../renderer/Frame.h"
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

  static const uint64_t UUID() { return 1; };
  uint64_t m_UUID() override { return 1; };

  /**
   * Logic initialization.
   */
  void Initialize() override;

  /**
   * Render initialization.
   * Need access to the current device for GPU manipulation.
   */
  template <typename D>
  void Initialize(D* device) {
    // Loading uniform buffer
    this->ubo = device->CreateUbo((void*)&this->uniform, sizeof(UniformStruct));
    std::cout << "this->ubo = " << this->ubo << std::endl;
  };

  /**
   * Logic update.
   */
  void Update() override;

  uint32_t Uniform() { return this->ubo; };

  void SetPosition(glm::vec3 position);
  void SetRotation(glm::vec3 rotation);
  void SetScale(glm::vec3 scale);

  /**
   * Render update. Generate a draw call.
   * Doesn't modify anything on the GPU
   */
  DrawCall Draw();

  /**
   * Render update. Modify resources on the GPU.
   * Need access to device.
   */
  template <typename D>
  void Upload(D* device) {
    // Loading uniform buffer
    // device->CreateUbo((void*)this->uniform, sizeof(UniformStruct));
    // std::cout << "Coucou depuis CTransform::Upload<Device>" << std::endl;

    if (this->updated) {
      // Update uniform buffer
    }
  };
};

#endif  // NARA_SOURCE_LOGIC_CTRANSFORM_H_