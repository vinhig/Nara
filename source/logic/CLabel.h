//
// Created by vincent on 11.09.20.
//

#ifndef NARA_SOURCE_LOGIC_CLABEL_H_
#define NARA_SOURCE_LOGIC_CLABEL_H_

#include <string>

#include "Ecs.h"

class CLabel : public IComponent {
 private:
  std::string label;

 public:
  static const uint64_t uuid = 1;
  const uint64_t UUID() override { return this->uuid; };

  CLabel(IEntity* p_owner) : IComponent(p_owner){};
  ~CLabel(){};

  void Initialize() override {
    std::cout << "INITIATING CLabel" << std::endl;
    this->label = "Yo les bgs";
  };

  std::string Label() { return this->label; };
  void SetLabel(std::string newValue) { this->label = newValue; };
};

#endif  // NARA_SOURCE_LOGIC_CLABEL_H_