//
// Created by vincent on 13.08.20.
//

#include "Game.h"

#include "../common/Macros.h"
#include "../loader/DefaultTextureLoader.h"
#include "../loader/FbxMeshLoader.h"

template <class T>
Game<T>::Game() = default;

template <class T>
Game<T>::~Game() = default;

template <class T>
int Game<T>::SetDevice(Device<T> *p_device) {
  if (p_device && p_device->IsSuitable()) {
    this->device = p_device;
    return 0;
  } else {
    return 1;
  }
}

template <class T>
void Game<T>::Update() {}

template <class T>
void Game<T>::Run() {
  uint32_t basicProgram = this->device->CreateProgram("assets/shaders/basic");
  uint32_t ibasicProgram = this->device->CreateProgram("assets/shaders/ibasic");
  size_t vertexStride = sizeof(float) * 8;

  // Our first triangle
  // float triangleVertices[] = {
  //     -1.0f, -1.0f, 0.0f, 1.0, 0.0, 0.0, 0.0, 0.0,  // TOP LEFT
  //     1.0f,  -1.0f, 0.0f, 0.0, 1.0, 0.0, 1.0, 0.0,  // TOP RIGHT
  //     0.0f,  1.0f,  0.0f, 0.0, 0.0, 1.0, 0.5, 1.0,  // BOTTOM
  // };
  // unsigned int trianglesIndices[] = {0, 1, 2};
  // uint32_t vbo_triangle =
  //     this->device->CreateVbo(triangleVertices, _countof(triangleVertices));
  // uint32_t ibo_triangle =
  //     this->device->CreateIbo(trianglesIndices, _countof(trianglesIndices));

  // InputLayoutArgs inputLayout_triangle = {};
  // Array<InputLayoutEntryArgs> entries_triangle(3);
  // entries_triangle.Add(
  //     {0, 3, false, vertexStride, vbo_triangle, GLCType::GLFLoat, 0});
  // entries_triangle.Add({1, 3, false, vertexStride, vbo_triangle,
  //                       GLCType::GLFLoat, (void *)(sizeof(float) * 3)});
  // entries_triangle.Add({2, 2, false, vertexStride, vbo_triangle,
  //                       GLCType::GLFLoat, (void *)(sizeof(float) * 6)});
  // inputLayout_triangle.entries = entries_triangle;

  // uint32_t vao_triangle = this->device->CreateVao(inputLayout_triangle);

  // Our first square
  // float squareVertices[] = {
  //     0.5f,  0.5f,  0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f,  // TOP RIGHT
  //     0.5f,  -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,  // BOTTOM RIGHT
  //     -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,  // BOTTOM LEFT
  //     -0.5f, 0.5f,  0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f   // TOP LEFT
  // };
  // int squareIndices[] = {0, 1, 3, 1, 2, 3};
  FbxMeshLoader *meshLoader = new FbxMeshLoader();
  MeshSpec suzanne = meshLoader->Load("assets/meshes/suzanna.fbx");

  uint32_t vbo_square =
      this->device->CreateVbo(suzanne.vertices, suzanne.verticesCount);
  uint32_t ibo_square =
      this->device->CreateIbo(suzanne.indices, suzanne.indicesCount);

  InputLayoutArgs inputLayout_square = {};
  Array<InputLayoutEntryArgs> entries_square(3);
  entries_square.Add(
      {0, 3, false, vertexStride, vbo_square, GLCType::GLFLoat, 0});
  entries_square.Add({1, 3, false, vertexStride, vbo_square, GLCType::GLFLoat,
                      (void *)(sizeof(float) * 3)});
  entries_square.Add({2, 2, false, vertexStride, vbo_square, GLCType::GLFLoat,
                      (void *)(sizeof(float) * 6)});
  inputLayout_square.entries = entries_square;

  uint32_t vao_square = this->device->CreateVao(inputLayout_square);

  DefaultTextureLoader *loader = new DefaultTextureLoader();
  TextureSpec textureSpecPng = loader->Load("assets/textures/test.png");
  TextureSpec textureSpecJpg = loader->Load("assets/textures/test.jpg");

  uint32_t texture_test_png = this->device->CreateTexture(textureSpecPng);
  uint32_t texture_test_jpg = this->device->CreateTexture(textureSpecJpg);

  while (this->device->IsOpen()) {
    this->Update();
    /*ClearArgs args = {.framebuffer = 0,
                      .color = {1.0f, 0.0f, 1.0f, 1.0f},
                      .color_buffer = true,
                      .depth_buffer = true};*/
    this->device->SetClearArgs({0, {1.0f, 0.0f, 1.0f, 1.0f}, true, true});
    this->device->Clear();

    Frame *currentFrame = this->device->SpawnFrame();
    /*currentFrame->AddDCInstanced(
        {{vao_square, ibo_square, texture_test_png, _countof(squareIndices)},
         4});*/
    /*currentFrame->AddDCSingle({vao_triangle, ibo_triangle, texture_test_jpg,
                               _countof(trianglesIndices)});*/
    currentFrame->AddDCSingle(
        {vao_square, ibo_square, texture_test_jpg, suzanne.indicesCount});
    currentFrame->SetProgramSingle(basicProgram);
    currentFrame->SetProgramInstanced(ibasicProgram);
    // Some work on the frame
    this->device->EatFrame();

    this->device->Swap();
    this->device->PollEvents();
  }
}
