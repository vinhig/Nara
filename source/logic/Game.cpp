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
#include "CTerrain.h"
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

  Entity *terrain = new Entity(this->world);

  // CTerrain *terrainCTerrain = terrain->GetOrCreate<CTerrain>();
  // terrainCTerrain->width = 10;
  // terrainCTerrain->height = 10;
  // terrainCTerrain->precision = 2;

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
  sphereCMaterial->diffusePath = "assets/textures/test.png";
  monkeyCMaterial->diffusePath = "assets/textures/doc.png";
  sphereCMaterial->normalPath = "assets/textures/test.png";
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
  // terrainCTerrain->Initialize<Device<T>>(this->device);

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

  float moove = 0.0f;
  while (this->device->IsOpen()) {
    this->device->SetClearArgs({0, {0.9f, 0.9f, 0.9f, 1.0f}, true, true});
    this->device->Clear();

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
