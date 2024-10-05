#pragma once

#include "common.hpp"
class Lens {
public:
  u8 (&mirror)[];
  u8 x;
  u8 y;

  Lens(u8 x, u8 y, u8 (&mirror)[]);

  // redraws all the tiles visible inside of it - assume rendering is off
  void refill();

  void draw_sprite();
};