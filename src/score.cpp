#include "score.hpp"
#include "assets-loader.hpp"
#include <nesdoug.h>
#include <neslib.h>

Score::Score(Global &global_state) : global_state(global_state) {
  AssetsLoader::load_score_assets();
  oam_clear();
  ppu_on_all();
  pal_fade_to(0, 4);
}

Score::~Score() {
  pal_fade_to(4, 0);
  ppu_off();
}

void Score::run() {
  while (true) {
    rand16();
    ppu_wait_nmi();
  }
}