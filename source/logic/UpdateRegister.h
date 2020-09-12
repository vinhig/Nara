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
      ShouldNothing,        // IComponent
      ShouldUpdateAndDraw,  // CTransform
      ShouldDraw,           // CMeshInstance
      ShouldNothing,        // CMaterial
      ShouldUpdateAndDraw,  // CCamera
  };
};

#endif  // NARA_SOURCE_LOGIC_UPDATEREGISTER_H_