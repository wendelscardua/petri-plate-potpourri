#pragma once

#include "creature.hpp"
#include "global.hpp"
class Gameplay {
  Global &global_state;
  static constexpr u8 MAX_CREATURES = 12;
  u8 num_creatures;
  Creature creature[MAX_CREATURES];
public:
  Gameplay(Global&);
  ~Gameplay();

  void run();
};