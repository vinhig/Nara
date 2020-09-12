//
// Created by vincent on 12.09.20.
//

#ifndef NARA_SOURCE_LOGIC_CMATERIAL_H_
#define NARA_SOURCE_LOGIC_CMATERIAL_H_

#include "../common/Array.h"
#include "../loader/DefaultTextureLoader.h"
#include "Ecs.h"

class CMaterial : public IComponent {
 private:
  uint32_t diffuseTexture;
  uint32_t normalTexture;
  Array<uint32_t>* textures;

 public:
  std::string diffusePath;
  std::string normalPath;
  TextureLoader* loader;

  static const uint64_t UUID() { return 3; };
  uint64_t m_UUID() override { return 3; };

  CMaterial(Entity* p_entity) : IComponent(p_entity){};
  ~CMaterial();

  /**
   * Logic initialization.
   */
  void Initialize() override{};

  Array<uint32_t>* Textures() { return this->textures; };

  /**
   * Render initialization.
   * Need access to the current device for GPU manipulation.
   */
  template <typename D>
  void Initialize(D* device) {
    // Diffuse texture
    {
      TextureSpec textureSpec = loader->Load(this->diffusePath);
      this->diffuseTexture = device->CreateTexture(textureSpec);
      // loader->Unload(&textureSpec);
    }
    {
      TextureSpec textureSpec = loader->Load(this->normalPath);
      this->normalTexture = device->CreateTexture(textureSpec);
      // loader->Unload(&textureSpec);
    }

    this->textures = new Array<uint32_t>(2);
    this->textures->Add(this->diffuseTexture);
    this->textures->Add(this->normalTexture);
  }
};

#endif  // NARA_SOURCE_LOGIC_CMATERIAL_H_