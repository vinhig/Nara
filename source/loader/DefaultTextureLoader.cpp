//
// Created by vincent on 30.08.20.
//

#include "DefaultTextureLoader.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>

#include <iostream>
#include <string>

TextureSpec DefaultTextureLoader::Load(std::string path) {
  std::cout << "Loading texture: " << path << std::endl;
  int width, height, nrChannels;
  stbi_set_flip_vertically_on_load(true);
  unsigned char* data =
      stbi_load(path.c_str(), &width, &height, &nrChannels, 0);

  switch (nrChannels) {
    case 4:
      return {width, height, data, TextureFormat::RGBA, TextureWrap::Repeat};
    case 3:
      return {width, height, data, TextureFormat::RGB, TextureWrap::Repeat};

    default:
      throw std::runtime_error("Unknown image format.");
      break;
  }
}

void DefaultTextureLoader::Unload(TextureSpec* texture) {}