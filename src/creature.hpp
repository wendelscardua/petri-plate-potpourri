#pragma once

#include "common.hpp"

class Creature {
public:
  union {
    u8 genes;
    struct {
      u8 shape : 2;
      u8 color : 2;
      u8 eyes : 2;
      u8 mouth : 2;
    };
  };

  u8 row : 4;
  u8 column : 4;

  bool target;

  Creature();

  // draw on nametable
  void draw();

  // erase from nametable
  void erase();

  // splat on both nametable and mirror
  void splat(u8 (&mirror)[]);
};