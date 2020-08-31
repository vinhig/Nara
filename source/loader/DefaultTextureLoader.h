//
// Created by vincent on 30.08.20.
//

#ifndef NARA_SOURCE_LOADER_DEFAULTTEXTURELOADER_H_
#define NARA_SOURCE_LOADER_DEFAULTTEXTURELOADER_H_

#include "TextureLoader.h"

class DefaultTextureLoader /*: public TextureLoader*/ {
 private:
  /* data */
 public:
  DefaultTextureLoader() = default;
  ~DefaultTextureLoader() = default;
  TextureSpec Load(std::string path) /* override*/;
  void Unload(TextureSpec* texture) /* override*/;
};

#endif  // NARA_SOURCE_LOADER_DEFAULTTEXTURELOADER_H_