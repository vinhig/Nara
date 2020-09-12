//
// Created by vincent on 12.09.20.
//

#ifndef NARA_SOURCE_LOGIC_CCAMERA_H_
#define NARA_SOURCE_LOGIC_CCAMERA_H_

#include <glm/glm.hpp>
#include <mutex>

#include "../renderer/Frame.h"
#include "Ecs.h"

class CCamera : public IComponent {
 private:
  struct UniformStruct {
    float projection[16];
    float view[16];
  };
  glm::mat4 projection;
  glm::mat4 view;

  std::recursive_mutex vectorUpdate;
  std::recursive_mutex matrixUpdate;
  glm::vec3 lookAt;
  glm::vec3 position;

  float fov;

  uint32_t ubo;
  UniformStruct uniform = {};

  // Should logically update model matrix
  bool updated = false;
  // Should upload to buffer
  bool uploaded = true;

 public:
  CCamera(Entity* p_entity) : IComponent(p_entity){};
  ~CCamera(){};

  static const uint64_t UUID() { return 4; };
  uint64_t m_UUID() override { return 4; };

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
  void SetLookAt(glm::vec3 position);

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

    if (!this->uploaded) {
      this->vectorUpdate.lock();
      // Update uniform buffer
      device->UpdateUbo(this->ubo, (void*)&this->uniform,
                        sizeof(UniformStruct));
      this->uploaded = true;
      this->vectorUpdate.unlock();
    }
  };
};

#endif  // NARA_SOURCE_LOGIC_CCAMERA_H_