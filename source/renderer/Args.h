//
// Created by vincent on 17.08.20.
//

#ifndef NARA_SOURCE_RENDERER_ARGS_H_
#define NARA_SOURCE_RENDERER_ARGS_H_

struct ClearArgs {
  unsigned int framebuffer;
  float color[4];
  bool color_buffer;
  bool depth_buffer;
};

#endif  // NARA_SOURCE_RENDERER_ARGS_H_
