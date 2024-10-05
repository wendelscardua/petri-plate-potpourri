#include "lens.hpp"
#include "banked-metasprites.hpp"
#include "metasprites.hpp"
#include <neslib.h>

Lens::Lens(u8 x, u8 y, u8 (&mirror)[]) : mirror(mirror), x(x), y(y) {}

void Lens::draw_sprite() {
  banked_oam_meta_spr_horizontal(x, y - 1, UI_BANK, UI::lens);
}

void Lens::refill() {
  // lens size = 6x6 tiles
  u8 start_row = y / 8 - 3;
  u8 start_col = x / 8 - 3;
  for (u8 i = 0; i < 6; i++) {
    vram_adr((unsigned int)NTADR_A(start_col, start_row + i));
    vram_write(mirror + NTADR_A(start_col, start_row + i) - NTADR_A(0, 4), 6);
  }
}