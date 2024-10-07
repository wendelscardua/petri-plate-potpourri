#include "score.hpp"
#include "assets-loader.hpp"
#include "bindec.hpp"
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

    u8 text_buffer[3];

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
      Bindec::convert(global_state.plates_cleared, text_buffer);
      multi_vram_buffer_horz(text_buffer, 3, NTADR_A(15, 14));
      break;
    case 192:
      multi_vram_buffer_horz(" Best:      "_ts, 12, NTADR_A(6, 16));
      break;
    case 240:
      Bindec::convert(global_state.plates_cleared_high_score, text_buffer);
      multi_vram_buffer_horz(text_buffer, 3, NTADR_A(15, 16));
      break;
    }
    delay_counter++;
  }
}