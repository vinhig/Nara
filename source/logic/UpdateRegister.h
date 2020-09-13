//
// Created by vincent on 12.09.20.
//

#ifndef NARA_SOURCE_LOGIC_UPDATEREGISTER_H_
#define NARA_SOURCE_LOGIC_UPDATEREGISTER_H_

enum UpdateFlags {
  ShouldNothing,
  ShouldUpdate,         // Care about me on tick
  ShouldDraw,           // Care about me on draw
  ShouldUpdateAndDraw,  // Care about me on draw AND tick
};

class UpdateRegister {
 public:
  static constexpr int ComponentType[] = {
      ShouldNothing,        // IComponent = 0
      ShouldUpdateAndDraw,  // CTransform = 1
      ShouldDraw,           // CMeshInstance = 2
      ShouldNothing,        // CMaterial = 3
      ShouldUpdateAndDraw,  // CCamera = 4
      ShouldNothing,        // CTerrain = 5
  };
};

#endif  // NARA_SOURCE_LOGIC_UPDATEREGISTER_H_