//
// Created by vincent on 13.08.20.
//

#include "Game.h"

#include <string.h>

#include <glm/common.hpp>
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>

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
    this->width = this->device->Width();
    this->height = this->device->Height();
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

  /* Some MESH stuff */
  // Load SPHERE
  FbxMeshLoader *meshLoader = new FbxMeshLoader();
  MeshSpec sphere = meshLoader->Load("assets/meshes/sphere.fbx");

  uint32_t vbo_sphere =
      this->device->CreateVbo(sphere.vertices, sphere.verticesCount);
  uint32_t ibo_sphere =
      this->device->CreateIbo(sphere.indices, sphere.indicesCount);

  InputLayoutArgs inputLayout_sphere = {};
  Array<InputLayoutEntryArgs> entries_sphere(3);
  entries_sphere.Add(
      {0, 3, false, vertexStride, vbo_sphere, GLCType::GLFloat, 0});
  entries_sphere.Add({1, 3, false, vertexStride, vbo_sphere, GLCType::GLFloat,
                      (void *)(sizeof(float) * 3)});
  entries_sphere.Add({2, 2, false, vertexStride, vbo_sphere, GLCType::GLFloat,
                      (void *)(sizeof(float) * 6)});
  inputLayout_sphere.entries = entries_sphere;

  // Load SUZANNA
  MeshSpec suzanna = meshLoader->Load("assets/meshes/suzanna.fbx");

  uint32_t vbo_suzanna =
      this->device->CreateVbo(suzanna.vertices, suzanna.verticesCount);
  uint32_t ibo_suzanna =
      this->device->CreateIbo(suzanna.indices, suzanna.indicesCount);

  InputLayoutArgs inputLayout_suzanna = {};
  Array<InputLayoutEntryArgs> entries_suzanna(3);
  entries_suzanna.Add(
      {0, 3, false, vertexStride, vbo_suzanna, GLCType::GLFloat, 0});
  entries_suzanna.Add({1, 3, false, vertexStride, vbo_suzanna, GLCType::GLFloat,
                       (void *)(sizeof(float) * 3)});
  entries_suzanna.Add({2, 2, false, vertexStride, vbo_suzanna, GLCType::GLFloat,
                       (void *)(sizeof(float) * 6)});
  inputLayout_suzanna.entries = entries_suzanna;

  uint32_t vao_suzanna = this->device->CreateVao(inputLayout_suzanna);
  uint32_t vao_sphere = this->device->CreateVao(inputLayout_sphere);

  DefaultTextureLoader *loader = new DefaultTextureLoader();
  TextureSpec textureSpecPng = loader->Load("assets/textures/test.png");
  TextureSpec textureSpecJpg = loader->Load("assets/textures/test.jpg");

  uint32_t texture_test_png = this->device->CreateTexture(textureSpecPng);
  uint32_t texture_test_jpg = this->device->CreateTexture(textureSpecJpg);

  /* Some MATRICES stuff */
  glm::mat4 model = glm::mat4(1.0f);
  glm::mat4 view =
      glm::lookAt(glm::vec3(-2.0f, -2.0f, -2.0f), glm::vec3(0.0f, 0.0f, 0.0f),
                  glm::vec3(0.0f, 1.0f, 0.0f));
  glm::mat4 projection =
      glm::perspective((float)glm::radians(60.0),
                       (float)this->width / (float)this->height, 0.1f, 100.0f);

  struct SceneUniform {
    float projection[16];
    float view[16];
  };

  struct ObjectUniform {
    float model[16];
  };

  SceneUniform struct_ubo_scene;
  memcpy(&struct_ubo_scene.projection[0], &projection[0][0],
         16 * sizeof(float));
  memcpy(&struct_ubo_scene.view[0], &view[0][0], 16 * sizeof(float));

  ObjectUniform struct_ubo_object;
  memcpy(&struct_ubo_object.model[0], &model[0][0], 16 * sizeof(float));

  uint32_t ubo_scene =
      this->device->CreateUbo((void *)&struct_ubo_scene, sizeof(SceneUniform));

  uint32_t ubo_object = this->device->CreateUbo((void *)&struct_ubo_object,
                                                sizeof(ObjectUniform));

  Array<uint32_t> *ubos = new Array<uint32_t>();
  ubos->Add(ubo_scene);
  ubos->Add(ubo_object);

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
        {{vao_sphere, ibo_sphere, texture_test_png, sphere.indicesCount}, 4});*/
    /*currentFrame->AddDCSingle({vao_triangle, ibo_triangle, texture_test_jpg,
                               _countof(trianglesIndices)});*/
    currentFrame->AddDCSingle({vao_suzanna, ibo_suzanna, texture_test_png, ubos,
                               suzanna.indicesCount});
    currentFrame->SetProgramSingle(basicProgram);
    currentFrame->SetProgramInstanced(ibasicProgram);
    // Some work on the frame
    this->device->EatFrame();

    this->device->Swap();
    this->device->PollEvents();
  }
}
