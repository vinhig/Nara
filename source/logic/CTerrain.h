//
// Created by vincent on 13.09.20.
//

#ifndef NARA_SOURCE_LOGIC_CTERRAIN_H_
#define NARA_SOURCE_LOGIC_CTERRAIN_H_

#include <string>

#include "../renderer/Args.h"
#include "../renderer/Frame.h"
#include "CTransform.h"
#include "Ecs.h"

class CTerrain : public IComponent {
 private:
  uint32_t ibo;
  uint32_t vao;
  uint32_t vbo;

  int count;

  CTransform* transform;

 public:
  int width, height, precision;

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
    vertices.reserve(width * height);
    indices.reserve(width * height * 3);

    float wh = (float)width / 2.0f;               // half width
    float hh = (float)width / 2.0f;               // half height
    float ws = (float)width / (float)precision;   // width step
    float hs = (float)height / (float)precision;  // width step
    for (float w = -wh; w <= wh; w += ws) {
      for (float h = -hh; h <= hh; h += hs) {
        vertices.push_back(w);
        vertices.push_back(0.0);
        vertices.push_back(h);

        vertices.push_back(0.0);
        vertices.push_back(1.0);
        vertices.push_back(0.0);

        vertices.push_back(w);
        vertices.push_back(h);
      }
    }

    for (int w = 0; w < width; w++) {
      for (int h = 0; h < height; h++) {
        // Current 'main' point
        // auto main = w*height;
        // Add it no matter what

        if (!(w % 2)) {
          indices.push_back(w * height + h);
          indices.push_back(w * height + h + 1);
          indices.push_back(w * (height + 1) + h);
        } else {
          indices.push_back(w * height + h);
          indices.push_back(w * (height + 1) + h);
          indices.push_back(w * (height + 1) + h - 1);
        }
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