#include "input.hpp"
#include <nesdoug.h>
#include <neslib.h>

Input::Input(char player) : player(player) {}

void Input::poll() {
  pad_poll(player);
  held_state = pad_state(player);
  pressed_state = get_pad_new(player);
}
