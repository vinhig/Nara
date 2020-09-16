//
// Created by vincent on 13.09.20.
//

#ifndef NARA_SOURCE_LOGIC_CTERRAIN_H_
#define NARA_SOURCE_LOGIC_CTERRAIN_H_

#include <string>

#include "../renderer/Args.h"
#include "../renderer/Frame.h"
#include "CMaterial.h"
#include "CTransform.h"
#include "Ecs.h"

class CTerrain : public IComponent {
 private:
  uint32_t ibo;
  uint32_t vao;
  uint32_t vbo;

  int count;

  CTransform* transform;
  CMaterial* material;

 public:
  int width, precision;

  static const uint64_t UUID() { return 5; };
  uint64_t m_UUID() override { return 5; };

  CTerrain(Entity* p_entity) : IComponent(p_entity){};
  ~CTerrain(){};

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
    std::vector<float> vertices;
    std::vector<unsigned int> indices;
    vertices.reserve(width * width * 3 + width * width * 3 + width * width * 2);
    indices.reserve(width * width * 3);

    float wh = (float)width / 2.0f;              // half width
    float ws = (float)width / (float)precision;  // width step

    int nbLine = 0;

    for (float w = -wh; w <= wh; w += ws) {
      for (float h = -wh; h <= wh; h += ws) {
        // std::cout << "Adding {" << w << ", 0.0, " << h << "}" << std::endl;
        vertices.push_back(w);
        vertices.push_back(-1.0);
        vertices.push_back(h);

        vertices.push_back(0.0);
        vertices.push_back(1.0);
        vertices.push_back(0.0);

        vertices.push_back(1 - (w + wh) / (float)width);
        vertices.push_back(/*1 -*/ (h + wh) / (float)width);
      }
      nbLine++;
    }

    for (unsigned int i = 0; i < nbLine - 1; i++) {
      for (unsigned int j = 0; j < nbLine - 1; j++) {
        unsigned int actualIndex = i * nbLine + j;
        indices.push_back(actualIndex);
        indices.push_back(actualIndex + nbLine + 1);
        indices.push_back(actualIndex + nbLine + 1 - 1);
        // } else {
        indices.push_back(actualIndex);
        indices.push_back(actualIndex + 1);
        indices.push_back(actualIndex + nbLine + 1);
      }
    }

    this->ibo = device->CreateIbo(indices.data(), indices.size());
    this->vbo = device->CreateVbo(vertices.data(), vertices.size());

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
    this->count = indices.size();

    this->transform->Initialize(device);
  };

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
  void Upload(D* device){
      // std::cout << "Coucou depuis CMeshInstance::CMeshInstance<Device>" <<
      // std::endl;
  };
};

#endif  // NARA_SOURCE_LOGIC_CTERRAIN_H_