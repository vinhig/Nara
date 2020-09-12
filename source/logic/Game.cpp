//
// Created by vincent on 13.08.20.
//

#include "Game.h"

#include <math.h>
#include <string.h>

#include <glm/common.hpp>
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <thread>

#include "../common/Macros.h"
#include "../loader/DefaultTextureLoader.h"
#include "../loader/FbxMeshLoader.h"
#include "CCamera.h"
#include "CMaterial.h"
#include "CMeshInstance.h"
#include "UpdateRegister.h"

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
    this->running = true;
    return 0;
  }

  return 1;
}

/**
 * Represents a game tick.
 * Actually feed workers with logical update jobs.
 */
template <class T>
void Game<T>::Update() {
  for (auto it = this->world->components.begin();
       it != this->world->components.end(); ++it) {
    if (UpdateRegister::ComponentType[it->first] == ShouldUpdate ||
        UpdateRegister::ComponentType[it->first] == ShouldUpdateAndDraw) {
      for (int i = 0; i < it->second.size(); i++) {
        this->findAJob.lock();
        Job job = {};
        job.component = it->second[i];
        job.subtype = JobType::ComponentUpdate;
        this->jobs.push_back(job);
        this->findAJob.unlock();
      }
    }
  }
}

/**
 * Find a logical job to do.
 */
template <class T>
void Game<T>::Work(int workerID) {
  // Choose a job to do
  // Tell everybody to let him choose
  this->findAJob.lock();
  if (this->jobs.size() != 0) {
    auto currentJob = this->jobs.front();
    this->jobs.erase(this->jobs.begin());  // I'll take that
    // Do the job
    if (currentJob.subtype == JobType::ComponentUpdate) {
      currentJob.component->Update();
    }
  }
  this->findAJob.unlock();  // Okay your turn dear other threads
}

template <class T>
void Game<T>::Run() {
  uint32_t basicProgram = this->device->CreateProgram("assets/shaders/basic");
  uint32_t ibasicProgram = this->device->CreateProgram("assets/shaders/ibasic");

  size_t vertexStride = sizeof(float) * 8;

  this->world = new System();

  // Create a testing sphere
  Entity *sphere = new Entity(this->world);

  // Create a testing monkey
  Entity *monkey = new Entity(this->world);

  Entity *camera = new Entity(this->world);

  CCamera *cameraCCamera = camera->GetOrCreate<CCamera>();

  // Prepare something to load our mesh and textures
  FbxMeshLoader *meshLoader = new FbxMeshLoader();
  DefaultTextureLoader *textureLoader = new DefaultTextureLoader();

  // Populate our entity with a visual component
  CMeshInstance *sphereCMesh = sphere->GetOrCreate<CMeshInstance>();
  CMeshInstance *monkeyCMesh = monkey->GetOrCreate<CMeshInstance>();
  sphereCMesh->path = "assets/meshes/sphere.fbx";
  monkeyCMesh->path = "assets/meshes/suzanna.fbx";
  sphereCMesh->loader = meshLoader;
  monkeyCMesh->loader = meshLoader;

  // Populate our entity with a visual component
  CMaterial *sphereCMaterial = sphere->GetOrCreate<CMaterial>();
  CMaterial *monkeyCMaterial = monkey->GetOrCreate<CMaterial>();
  sphereCMaterial->diffusePath = "assets/textures/doc.png";
  monkeyCMaterial->diffusePath = "assets/textures/doc.png";
  sphereCMaterial->normalPath = "assets/textures/doc.png";
  monkeyCMaterial->normalPath = "assets/textures/doc.png";
  sphereCMaterial->loader = textureLoader;
  monkeyCMaterial->loader = textureLoader;

  // Launch init step
  this->world->Initialize();

  // Manually launch loading step
  cameraCCamera->Initialize<Device<T>>(this->device);
  sphereCMesh->Initialize<Device<T>>(this->device);
  monkeyCMesh->Initialize<Device<T>>(this->device);
  sphereCMaterial->Initialize<Device<T>>(this->device);
  monkeyCMaterial->Initialize<Device<T>>(this->device);

  // Spawn std::thread::hardware_concurrency() - 2 threads
  // Keep a master thread and a drawing thread
  // TODO: make exception when there are only two or less threads.
  std::cout << std::thread::hardware_concurrency() << std::endl;
  // Master thread
  workers.push_back(std::thread([this]() {
    while (this->running) {
      // TODO: Keep a 144 Hz tick frequency
      this->Update();
    };
  }));

  // Workers threads
  for (int i = 0; i < std::thread::hardware_concurrency() - 2; i++) {
    workers.push_back(std::thread([this, i]() {
      while (this->running) {
        this->Work(i);
      }
    }));
  }

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
  /*
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

  */

  /* Some MATRICES stuff */

  /*glm::mat4 model = glm::mat4(1.0f);
  glm::mat4 view =
      glm::lookAt(glm::vec3(-4.0f, -4.0f, -4.0f), glm::vec3(0.0f, 0.0f, 0.0f),
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

  Array<uint32_t> *ubos = new Array<uint32_t>(2);
  ubos->Add(ubo_scene);
  ubos->Add(ubo_object);

  Array<uint32_t> *textures = new Array<uint32_t>(2);
  textures->Add(texture_test_png);
  textures->Add(texture_test_jpg);
  */
  float moove = 0.0f;
  while (this->device->IsOpen()) {
    /*ClearArgs args = {.framebuffer = 0,
                      .color = {1.0f, 0.0f, 1.0f, 1.0f},
                      .color_buffer = true,
                      .depth_buffer = true};*/
    this->device->SetClearArgs({0, {1.0f, 0.0f, 1.0f, 1.0f}, true, true});
    this->device->Clear();

    // Spawn a new frame
    // Populate it with draw calls
    /*
      THREAD 1
    */
    // delete frames[currentFrame % 2];
    // frames[currentFrame % 2] = this->device->SpawnFrame();
    // this->device->EatFrame(frames[currentFrame % 2]);
    // currentFrame++;

    Frame *currentFrame = this->device->SpawnFrame();

    for (auto it = this->world->components.begin();
         it != this->world->components.end(); ++it) {
      if (UpdateRegister::ComponentType[it->first] == ShouldDraw ||
          UpdateRegister::ComponentType[it->first] == ShouldUpdateAndDraw) {
        for (int i = 0; i < it->second.size(); i++) {
          DrawCall call;
          if (it->second[i]->m_UUID() == 2) {
            ((CMeshInstance *)it->second[i])->Upload(this->device);
            call = ((CMeshInstance *)it->second[i])->Draw();
          } else if (it->second[i]->m_UUID() == 1) {
            ((CTransform *)it->second[i])->Upload(this->device);
            call = ((CTransform *)it->second[i])->Draw();
          } else if (it->second[i]->m_UUID() == 4) {
            ((CCamera *)it->second[i])->Upload(this->device);
            call = ((CCamera *)it->second[i])->Draw();
          }

          switch (call.subtype) {
            case DrawCallType::NoneDrawCall:
              /* code */
              break;
            case DrawCallType::SingleDrawCall: {
              currentFrame->AddDCSingle(call.single);
              break;
            }
            case DrawCallType::InstancedDrawCall: {
              currentFrame->AddDCInstanced(call.instanced);
              break;
            }

            default:
              break;
          }
        }
      }
    }

    monkey->GetOrCreate<CTransform>()->SetRotation(
        glm::vec3(moove, 0.0f, moove));

    sphere->GetOrCreate<CTransform>()->SetPosition(
        glm::vec3((float)cos(moove / 30.0f) * 2.0f, 0.0f,
                  (float)sin(moove / 30.0f) * 2.0f));

    moove += 2.0f;

    /*currentFrame->AddDCInstanced(
        {{vao_sphere, ibo_sphere, textures, ubos, sphere.indicesCount}, 4});
    /*currentFrame->AddDCSingle({vao_triangle, ibo_triangle, texture_test_jpg,
                               _countof(trianglesIndices)});*/
    // currentFrame->AddDCSingle(
    //     {vao_suzanna, ibo_suzanna, textures, ubos, suzanna.indicesCount});
    currentFrame->SetProgramSingle(basicProgram);
    currentFrame->SetProgramInstanced(ibasicProgram);
    // Some work on the frame
    this->device->EatFrame(currentFrame);

    this->device->Swap();
    this->device->PollEvents();
  }
  this->running = false;
  for (int i = 0; i < this->workers.size(); i++) {
    this->workers[i].join();
  }
}
