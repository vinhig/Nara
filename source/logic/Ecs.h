//
// Created by vincent on 11.09.20.
//

#ifndef NARA_SOURCE_LOGIC_ECS_H_
#define NARA_SOURCE_LOGIC_ECS_H_

#include <unordered_map>
#include <vector>

#include "../common/Array.h"

class System;  // cpp bad

/**
 * Entity is an object managed by a system.
 * It doesn't own anything but some components
 * targets it as their owner.
 */
class Entity {
 private:
  /**
   * System managing this entity.
   */
  System* system = nullptr;

 public:
  /**
   * Create an entity and reference its corresponding system.
   */
  Entity(System* p_system) { this->system = p_system; };
  ~Entity() = default;

  /**
   * Get the component targeting this entity.
   * Or create a new one if it doesn't exist.
   */
  template <typename T>
  T* GetOrCreate();
};

/**
 * Base class for all component.
 * Characterized by a unique identifier (uuid).
 * It targets an entity describing it as its owner.
 */
class IComponent {
 private:
 public:
  /**
   * Owner of this component
   */
  Entity* entity = nullptr;

  /**
   * Create a component and reference its corresponding entity.
   */
  IComponent(Entity* p_entity) { this->entity = p_entity; };
  ~IComponent(){};

  /**
   * Unique identitifer.
   * Used as a reflection method (to compare types).
   */
  // static const uint64_t uuid = 0;
  virtual uint64_t m_UUID() { return 0; };

  /**
   * Initialize ("logically") this component.
   * The actual first action applied on all components.
   */
  virtual void Initialize() = 0;
  /**
   * Update ("logically") this component.
   * Action applied on all components each frame.
   */
  virtual void Update(){};
};

/**
 * System can be compared as a scene grouping many entities represented by
 * components.
 */
class System {
 private:
  /**
   * All components of this scene.
   * No sorted by type.
   */
  // std::vector<IComponent*> components;

 public:
  System(){};
  ~System(){};

  /**
   * All components of this scene.
   * Sorted by type.
   */
  std::unordered_map<uint64_t, std::vector<IComponent*>> components;

  /**
   * Append a new component.
   */
  template <typename T>
  void Append(T* component);

  /**
   * Get the component owned by 'parent' with a 'T' type.
   * The component returned is unique.
   */
  template <typename T>
  T* Get(Entity* parent);

  /**
   * Initialize every components.
   * Has to be called before every other action methods.
   */
  void Initialize() {
    for (auto it = this->components.begin(); it != this->components.end();
         ++it) {
      for (int i = 0; i < it->second.size(); i++) {
        it->second[i]->Initialize();
      }
    }
  }
};

#ifndef ECS_IMPLEMENTATION
#define ECS_IMPLEMENTATION

template <typename T>
T* Entity::GetOrCreate() {
  // Check if there isn't any entity
  auto component = this->system->Get<T>(this);
  if (!component) {
    // Create it
    this->system->Append(new T(this));
    return this->system->Get<T>(this);
  }
  return component;
};

template <typename T>
void System::Append(T* component) {
  if (this->components.find(T::UUID()) == this->components.end()) {
    this->components[T::UUID()];
  }
  this->components[T::UUID()].push_back(component);
};

template <typename T>
T* System::Get(Entity* parent) {
  auto subComponents = this->components[T::UUID()];
  for (int i = 0; i < subComponents.size(); i++) {
    // Compare owner of the component and the actual type of the component
    // (represented by a uuid).
    if (subComponents[i]->entity == parent &&
        ((T*)subComponents[i])->UUID() == T::UUID()) {
      return (T*)subComponents[i];
    }
  }
  return nullptr;
};

#endif  // ECS_IMPLEMENTATION

#endif  // NARA_SOURCE_LOGIC_ECS_H_