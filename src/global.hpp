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
  enum class GameState : u8 { Gameplay };

  GameState game_state;

  Input p1_input;
  Input p2_input;

  Global();
};