#pragma once

#include "creature.hpp"
#include "global.hpp"
class Gameplay {
  Global &global_state;
  static constexpr u8 MAX_CREATURES = 12;
  u8 num_creatures;
  Creature creature[MAX_CREATURES];

  u8 screen_mirror[13 * 16 * 4];
public:
  Gameplay(Global&);
  ~Gameplay();

  void run();
};