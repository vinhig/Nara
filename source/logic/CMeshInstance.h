//
// Created by vincent on 11.09.20.
//

#ifndef NARA_SOURCE_LOGIC_CMESHINSTANCE_H_
#define NARA_SOURCE_LOGIC_CMESHINSTANCE_H_

#include <string>

#include "../loader/MeshLoader.h"
#include "../renderer/Frame.h"
#include "CTransform.h"
#include "Ecs.h"

class CMeshInstance : public IComponent {
 private:
  uint32_t ibo;
  uint32_t vao;
  uint32_t vbo;

  int count;

  CTransform* transform;

 public:
  std::string path;
  MeshLoader* loader;

  static const uint64_t UUID() { return 2; };
  uint64_t m_UUID() override { return 2; };

  CMeshInstance(Entity* p_entity) : IComponent(p_entity){};
  ~CMeshInstance(){};

  /**
   * Logic initialization.
   */
  void Initialize() override {
    std::cout << "INITIATING CMeshInstance" << std::endl;
    // Need a transform
    transform = this->entity->GetOrCreate<CTransform>();
    transform->Initialize();
  };

  /**
   * Render initialization.
   * Need access to the current device for GPU manipulation.
   */
  template <typename D>
  void Initialize(D* device) {
    // std::cout << "Coucou depuis CMeshInstance::Load<Device>" << std::endl;
    // Loading mesh
    MeshSpec mesh = this->loader->Load(this->path);

    this->ibo = device->CreateIbo(mesh.indices, mesh.indicesCount);
    this->vbo = device->CreateVbo(mesh.vertices, mesh.verticesCount);
    this->count = mesh.indicesCount;

    size_t vertexStride = sizeof(float) * 8;

    InputLayoutArgs inputLayout = {};
    Array<InputLayoutEntryArgs> entries(3);
    entries.Add({0, 3, false, vertexStride, this->vbo, GLCType::GLFloat, 0});
    entries.Add({1, 3, false, vertexStride, this->vbo, GLCType::GLFloat,
                 (void*)(sizeof(float) * 3)});
    entries.Add({2, 2, false, vertexStride, this->vbo, GLCType::GLFloat,
                 (void*)(sizeof(float) * 6)});
    inputLayout.entries = entries;

    this->vao = device->CreateVao(inputLayout);

    this->transform->Initialize(device);
  };

  /**
   * Render update. Generate a draw call.
   * Doesn't modify anything on the GPU
   */
  DrawCall Draw() {
    // std::cout << "Coucou depuis CMeshInstance::Draw<Device>" << std::endl;
    // Generating a draw call
    DrawCall call = {};
    call.subtype = DrawCallType::SingleDrawCall;
    DCSingle actualCall = {};

    // Fetch information for this mesh
    actualCall.vao = this->vao;
    actualCall.ibo = this->ibo;
    actualCall.textures = new Array<uint32_t>(0);
    actualCall.uniforms = new Array<uint32_t>(1);
    actualCall.count = this->count;

    // Fetch information for the uniform buffer
    // From CTransform
    actualCall.uniforms->Add(this->transform->Uniform());

    // Feed the union
    call.single = actualCall;

    return call;
  }

  /**
   * Render update. Modify resources on the GPU.
   * Need access to device.
   */
  template <typename D>
  void Upload(D* device){
      // std::cout << "Coucou depuis CMeshInstance::CMeshInstance<Device>" <<
      // std::endl;
  };
};

#ifndef CMESHINSTANCE_IMPLEMENTATION
#define CMESHINSTANCE_IMPLEMENTATION

// const uint64_t CMeshInstance::uuid = 1;

#endif  // CMESHINSTANCE_IMPLEMENTATION

#endif  // NARA_SOURCE_LOGIC_CMESHINSTANCE_H_