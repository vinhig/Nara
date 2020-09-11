//
// Created by vincent on 11.09.20.
//

#ifndef NARA_SOURCE_LOGIC_COMPONENT_H_
#define NARA_SOURCE_LOGIC_COMPONENT_H_

#include "Entity.h"

class IEntity; // cpp bad

class IComponent {
 private:
  IEntity* owner;
 public:
  IComponent(IEntity* p_owner);
  ~IComponent();

  virtual void Initialize() = 0;
};

#endif  // NARA_SOURCE_LOGIC_COMPONENT_H_