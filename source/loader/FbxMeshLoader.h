//
// Created by vincent on 30.08.20.
//

#ifndef NARA_SOURCE_LOADER_FBXMESHLOADER_H_
#define NARA_SOURCE_LOADER_FBXMESHLOADER_H_

#include "../renderer/Args.h"
#include "MeshLoader.h"

class FbxMeshLoader : public MeshLoader {
 private:
  /* data */
 public:
  FbxMeshLoader(/* args */);
  ~FbxMeshLoader();

  MeshSpec Load(std::string path) override;
};

#endif  // NARA_SOURCE_LOADER_FBXMESHLOADER_H_