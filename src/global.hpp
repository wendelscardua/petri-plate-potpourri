#pragma once

#include "common.hpp"
#include "input.hpp"

/* represents global state:
   - settings
   - high scores
   - etc
*/
class Global {
public:
  enum class GameState : u8 { Title, Gameplay };

  GameState game_state;

  Input p1_input;
  Input p2_input;

  u8 misses;
  u8 plates_cleared;
  u8 plates_cleared_high_score;
  u16 timer_seconds;
  u8 timer_frames;

  Global();
};