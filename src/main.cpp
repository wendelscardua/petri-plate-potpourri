#include "global.hpp"
#include <mapper.h>
#include <nesdoug.h>
#include <neslib.h>
#include <peekpoke.h>

int main() {
  set_prg_bank(0);

  ppu_off();

  bank_bg(0);
  bank_spr(1);

  set_vram_buffer();

  Global global_state;

  // global_state.game_state = Global::GameState::TitleScreen;

  while (true) {
    switch (global_state.game_state) {
    }
  }
}