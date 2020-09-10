//
// Created by vincent on 28.08.20.
//

#ifndef NARA_SOURCE_LOADER_MESHLOADER_H_
#define NARA_SOURCE_LOADER_MESHLOADER_H_

#include <string>

#include "../renderer/Args.h"

class MeshLoader {
 private:
  /* data */
 public:
  virtual MeshSpec Load(std::string path) = 0;
};

#endif  // NARA_SOURCE_LOADER_MESHLOADER_H_