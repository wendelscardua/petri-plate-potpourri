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

  u8 row;
  u8 column;

  bool target;

  Creature(u8 row, u8 column, u8 genes);

  // draw on nametable
  void draw();

  // erase from nametable
  void erase();
};