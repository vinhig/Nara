//
// Created by vincent on 25.08.20.
//

#include <iostream>
#include <fstream>
#include <string>

class File {
 public:
  static std::string ReadAllFile(std::string path) {
    // C++ Version
    std::string content;
    std::string line;
    std::ifstream file (path);
    if (file.is_open()) {
      while (std::getline(file, line)) {
        content += line + "\n";
      }
      file.close();
    } else {
      throw std::runtime_error("Unable to open file: '" + path + "'.");
    }
    return content;
  }
};