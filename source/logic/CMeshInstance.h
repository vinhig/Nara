//
// Created by vincent on 11.09.20.
//

#ifndef NARA_SOURCE_LOGIC_CMESHINSTANCE_H_
#define NARA_SOURCE_LOGIC_CMESHINSTANCE_H_

#include <string>

#include "../loader/MeshLoader.h"
#include "CTransform.h"
#include "Ecs.h"
#include "Game.h"

class CMeshInstance : public IComponent {
 private:
  uint32_t ibo;
  uint32_t vao;
  uint32_t vbo;

  CTransform* transform;

 public:
  std::string path;
  MeshLoader* loader;
  static const uint64_t uuid = 2;
  const uint64_t UUID() override { return this->uuid; };

  CMeshInstance(Entity* p_entity) : IComponent(p_entity){};
  ~CMeshInstance(){};

  void Initialize() override {
    std::cout << "INITIATING CMeshInstance" << std::endl;
    // Need a transform
    transform = this->entity->GetOrCreate<CTransform>();
  };

  template <typename D>
  void Load(D* device) {
    std::cout << "Coucou depuis Load<Device>" << std::endl;
    // Loading mesh
    MeshSpec mesh = this->loader->Load(this->path);

    this->ibo = device->CreateIbo(mesh.indices, mesh.indicesCount);
    this->vbo = device->CreateVbo(mesh.vertices, mesh.verticesCount);
  };
};

#endif  // NARA_SOURCE_LOGIC_CMESHINSTANCE_H_