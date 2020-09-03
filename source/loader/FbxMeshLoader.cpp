//
// Created by vincent on 30.08.20.
//

#include "FbxMeshLoader.h"

#include <ofbx.h>
#include <string.h>

#include <stdexcept>

FbxMeshLoader::FbxMeshLoader(/* args */) {}

FbxMeshLoader::~FbxMeshLoader() {}

MeshSpec FbxMeshLoader::Load(std::string path) {
  // Read file
  FILE* fp = fopen(path.c_str(), "rb");
  if (!fp) {
    throw std::runtime_error("Unable to open file " + path);
  }
  fseek(fp, 0, SEEK_END);
  long file_size = ftell(fp);
  fseek(fp, 0, SEEK_SET);
  auto* content = new ofbx::u8[file_size];
  fread(content, 1, file_size, fp);
  auto scene = ofbx::load((ofbx::u8*)content, file_size,
                          (ofbx::u64)ofbx::LoadFlags::TRIANGULATE);

  int stride = (3 + 3 + 2);
  // position=3, normals=3, uvs=2
  int currentBufferSize = 0;

  float* verticesBuffer = nullptr;
  int* indicesBuffer = nullptr;

  int totalVerticesCount = 0;
  int totalIndicesCount = 0;

  // Feed buffer
  int meshCount = scene->getMeshCount();
  // TODO: uncomment last lines to allow multiple mesh
  for (int i = 0; i < meshCount; i++) {
    auto currentMesh = scene->getMesh(i);
    std::cout << currentMesh->name << std::endl;
    // Alloc space to copy vertices
    int verticesCount = currentMesh->getGeometry()->getVertexCount();
    float* subVerticesBuffer =
        new float[verticesCount * sizeof(float) * stride];

    // Extract vertices
    auto vertices = currentMesh->getGeometry()->getVertices();
    auto normals = currentMesh->getGeometry()->getNormals();
    auto uvs = currentMesh->getGeometry()->getUVs();

    // Extract indices
    int indicesCount = currentMesh->getGeometry()->getIndexCount();
    auto indices = currentMesh->getGeometry()->getFaceIndices();
    unsigned int* subIndicesBuffer =
        new unsigned int[indicesCount * sizeof(unsigned int)];

    totalVerticesCount += verticesCount * 8;
    totalIndicesCount += indicesCount;

    int t = 0;

    for (int w = 0; w < indicesCount; w += 3) {
      if (indices[w + 0] < 0) {
        subIndicesBuffer[w + 0] = (unsigned int)(indices[w + 0] * (-1) - 1);
      } else {
        subIndicesBuffer[w + 0] = (unsigned int)indices[w + 0];
      }
      if (indices[w + 1] < 0) {
        subIndicesBuffer[w + 1] = (unsigned int)(indices[w + 1] * (-1) - 1);
      } else {
        subIndicesBuffer[w + 1] = (unsigned int)indices[w + 1];
      }
      if (indices[w + 2] < 0) {
        subIndicesBuffer[w + 2] = (unsigned int)(indices[w + 2] * (-1) - 1);
      } else {
        subIndicesBuffer[w + 2] = (unsigned int)indices[w + 2];
      }
      if (t < 10) {
        printf("Face n°%d:\n{%d, %d, %d}\n", w, subIndicesBuffer[w + 0],
               subIndicesBuffer[w + 1], subIndicesBuffer[w + 2]);
        t++;
      }
    }

    t = 0;
    // Copy vertices to subBuffer
    for (int j = 0; j < verticesCount; j++) {
      // Copy current vertex to buffer
      // printf("Adding n° %d:\n{%f, %f, %f} {%f, %f, %f} {%f, %f}\n", j,
      //        (float)vertices[j].x, (float)vertices[j].y,
      //        (float)vertices[j].z, (float)normals[j].x, (float)normals[j].y,
      //        (float)normals[j].z, (float)uvs[j].x, (float)uvs[j].y);

      subVerticesBuffer[j * stride + 0] = (float)vertices[j].x;
      subVerticesBuffer[j * stride + 1] = (float)vertices[j].y;
      subVerticesBuffer[j * stride + 2] = (float)vertices[j].z;

      subVerticesBuffer[j * stride + 3] = (float)normals[j].x;
      subVerticesBuffer[j * stride + 4] = (float)normals[j].y;
      subVerticesBuffer[j * stride + 5] = (float)normals[j].z;

      subVerticesBuffer[j * stride + 6] = (float)uvs[j].x;
      subVerticesBuffer[j * stride + 7] = (float)uvs[j].y;

      if (t < 364) {
        printf("Adding n°%d:\n {%f, %f, %f}\n", j,
               subVerticesBuffer[j * stride], subVerticesBuffer[j * stride + 1],
               subVerticesBuffer[j * stride + 2]);
        t++;
      }
    }
    return {subVerticesBuffer, subIndicesBuffer, totalVerticesCount,
            totalIndicesCount};
    /*
        // Append sub buffer to big buffer
        if (verticesBuffer == nullptr) {
          // Copy indices for the first time
          indicesBuffer = (int*)malloc(indicesCount * sizeof(int));
          memcpy(indicesBuffer, indices, indicesCount * sizeof(int));
          delete indices;
          totalIndicesCount += indicesCount;
          // Copy vertices for the first time
          verticesBuffer = (float*)malloc(verticesCount * sizeof(float) *
      stride); memcpy(verticesBuffer, subBuffer, verticesCount * sizeof(float) *
      stride); currentBufferSize = verticesCount * sizeof(float) * stride; }
      else {
          // Copy indices
          indicesBuffer = (int*)realloc(
              indicesBuffer, sizeof(int) * (totalIndicesCount + indicesCount));
          memcpy(indicesBuffer + totalIndicesCount, indices,
                 indicesCount * sizeof(int));
          delete indices;
          totalIndicesCount += indicesCount;
          // Copy vertices
          verticesBuffer = (float*)realloc(
              verticesBuffer,
              currentBufferSize + verticesCount * sizeof(float) * stride);
          memcpy(verticesBuffer + currentBufferSize, subBuffer,
                 verticesCount * sizeof(float) * stride);
          currentBufferSize =
              currentBufferSize + verticesCount * sizeof(float) * stride;
        }
        delete subBuffer;
      }

      return {verticesBuffer, indicesBuffer, totalVerticesCount,
      totalIndicesCount};
      */
  }
}