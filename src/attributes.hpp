#pragma once

#include "common.hpp"
namespace Attributes {
  extern u8 cache[64];

  void init();
  void set(u8 metatile_x, u8 metatile_y, u8 attribute);
  void flush();
}; // namespace Attributes