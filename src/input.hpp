#pragma once

class Input {
private:
  char player;
  char held_state;
  char pressed_state;

public:
  Input(char player);

  void poll();

  char pressed() { return pressed_state; }
  char held() { return held_state; }
};