#include "score.hpp"
#include "assets-loader.hpp"
#include "charset.hpp"
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
  u16 delay_counter = 0;
  while (true) {
    rand16();
    ppu_wait_nmi();

    switch (delay_counter) {
    case 48:
      multi_vram_buffer_horz("Performance "_ts, 12, NTADR_A(6, 10));
      multi_vram_buffer_horz(" report:    "_ts, 12, NTADR_A(6, 11));
      break;
    case 96:
      multi_vram_buffer_horz(" Plates     "_ts, 12, NTADR_A(6, 13));
      multi_vram_buffer_horz(" cleared:   "_ts, 12, NTADR_A(6, 14));
      break;
    case 144:
      // TODO display current score
      break;
    case 192:
      multi_vram_buffer_horz(" Best:      "_ts, 12, NTADR_A(6, 16));
      break;
    case 240:
      // TODO display best score
      break;
    }
    delay_counter++;
  }
}