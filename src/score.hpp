#pragma once

#include "global.hpp"
class Score {
  public:
  Global &global_state;

  Score(Global& global_state);
  ~Score();

  void run();
};