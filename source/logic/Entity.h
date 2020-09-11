//
// Created by vincent on 11.09.20.
//

#ifndef NARA_SOURCE_LOGIC_ENTITY_H_
#define NARA_SOURCE_LOGIC_ENTITY_H_

#include "System.h"

class System; // cpp bad

class IEntity {
 private:
  System *system;

 public:
  IEntity() = default;
  ~IEntity() = default;

  /*template <typename T>
  T GetOrCreate() {
    this->system->Append(T());
  };*/
};

#endif  // NARA_SOURCE_LOGIC_ENTITY_H_