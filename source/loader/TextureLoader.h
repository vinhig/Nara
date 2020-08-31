//
// Created by vincent on 28.08.20.
//

#ifndef NARA_SOURCE_LOADER_TEXTURELOADER_H_
#define NARA_SOURCE_LOADER_TEXTURELOADER_H_

#include <string>

#include "../renderer/Args.h"

class TextureLoader {
 public:
  virtual TextureSpec Load(std::string path) = 0;
  virtual void Unload(TextureSpec* texture) = 0;
};

#endif  // NARA_SOURCE_LOADER_TEXTURELOADER_H_