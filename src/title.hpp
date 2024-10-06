#pragma once

#include "global.hpp"
class Title {
public:
  Global &global_state;

  Title(Global& global_state);
  ~Title();

  void run();
};