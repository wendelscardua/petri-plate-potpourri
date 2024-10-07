#include "title.hpp"
#include "assets-loader.hpp"
#include "ggsound.hpp"
#include "global.hpp"
#include "soundtrack.hpp"
#include <nesdoug.h>
#include <neslib.h>

Title::Title(Global &global_state) : global_state(global_state) {
  AssetsLoader::load_title_assets();
  oam_clear();
  ppu_on_all();
  GGSound::play_song(Song::Prologue);
  pal_fade_to(0, 4);
}

Title::~Title() {
  pal_fade_to(4, 0);
  ppu_off();
}

void Title::run() {
  while (true) {
    rand16();
    ppu_wait_nmi();
    global_state.p1_input.poll();

    if (global_state.p1_input.pressed() & (PAD_START | PAD_A | PAD_B)) {
      global_state.game_state = Global::GameState::Gameplay;
      break;
    }
  }
}