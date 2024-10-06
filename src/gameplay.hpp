#pragma once

#include "creature.hpp"
#include "global.hpp"
#include "lens.hpp"
class Gameplay {
  Global &global_state;
  static constexpr u8 MAX_CREATURES = 16;
  u8 num_creatures;
  u8 num_imposters;
  u8 num_fixed_features;
  Creature creature[MAX_CREATURES];
  Lens lens;

  u8 screen_mirror[13 * 16 * 4];
public:
  Gameplay(Global&, u8 num_creatures, u8 num_imposters, u8 num_fixed_features);
  ~Gameplay();

  void setup_creatures();
  void run();
  void refresh_hud();
  void inject_creature();
};