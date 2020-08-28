//
// Created by vincent on 25.08.20.
//

#include <iostream>
#include <string>

class File {
 public:
  static std::string ReadAllFile(std::string path) {
    FILE *infile;
    char *buffer;
    long numbytes;

    infile = fopen(path.c_str(), "r");

    if (infile == NULL) {
      throw std::runtime_error("Unable to open file '" + path + "'");
    }

    fseek(infile, 0L, SEEK_END);
    numbytes = ftell(infile);

    fseek(infile, 0L, SEEK_SET);

    // buffer = (char *)calloc(numbytes, sizeof(char));
    buffer = (char *)malloc(sizeof(char) * (numbytes + 1));

    if (buffer == NULL) {
      throw std::runtime_error(
          "Unable to allocate sufficient memory for file '" + path + "'");
    };

    fread(buffer, sizeof(char), numbytes, infile);
    fclose(infile);

    buffer[numbytes] = '\0';

    std::string content(buffer);

    free(buffer);

    return content;
  }
};