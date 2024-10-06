#include "gameplay.hpp"
#include "global.hpp"
#include "title.hpp"
#include <mapper.h>
#include <nesdoug.h>
#include <neslib.h>
#include <peekpoke.h>

int main() {
  set_prg_bank(0);

  pal_bright(0);
  ppu_off();

  bank_bg(0);
  bank_spr(1);

  set_vram_buffer();

  Global global_state;

  global_state.game_state = Global::GameState::Title;

  while (true) {
    switch (global_state.game_state) {
    case Global::GameState::Title: {
      Title title(global_state);
      title.run();
      break;
    }
    case Global::GameState::Gameplay: {
      global_state.misses = 0;
      global_state.plates_cleared = 0;
      while (global_state.misses < 3) {
        Gameplay gameplay(global_state, 12, 3, 2);
        gameplay.run();
      }
      global_state.game_state = Global::GameState::Title;
    }
    }
  }
}