//
// Created by vincent on 11.09.20.
//

#ifndef NARA_SOURCE_LOGIC_SYSTEM_H_
#define NARA_SOURCE_LOGIC_SYSTEM_H_

#include "../common/Array.h"
#include "Component.h"
#include "CTransform.h"

class System {
 private:
  Array<CTransform>* transforms;

  Array<IComponent>* components;

 public:
  System(/* args */) {
    this->transforms = new Array<CTransform>(15);
    this->components = new Array<IComponent>(15);
  };
  ~System();

  /*template <typename T>
  void Append(T component) {
    this->components->Add(component);
  };*/
};

#endif  // NARA_SOURCE_LOGIC_SYSTEM_H_