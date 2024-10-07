#include "gameplay.hpp"
#include "assets-loader.hpp"
#include "attributes.hpp"
#include "banked-metasprites.hpp"
#include "bindec.hpp"
#include "global.hpp"
#include "metasprites.hpp"
#include "subrand.hpp"
#include <nesdoug.h>
#include <neslib.h>

static const u8 starting_row = 4;

static const u8 start_column_per_row[] = {
    3, 2, 2, 2, 2, 2, 2, 2, 3,
};

static const u8 num_columns_per_row[] = {
    10, 12, 12, 12, 12, 12, 12, 12, 10,
};

Gameplay::Gameplay(Global &global_state, u8 num_creatures, u8 num_imposters,
                   u8 num_fixed_features)
    : global_state(global_state), num_creatures(num_creatures),
      num_imposters(num_imposters), num_fixed_features(num_fixed_features),
      lens(0x80, 0x80, global_state.p1_input, screen_mirror) {
  AssetsLoader::load_gameplay_assets();

  setup_creatures();

  vram_adr(NTADR_A(0, 4));
  vram_read(screen_mirror, sizeof(screen_mirror));
  vram_adr(NTADR_A(0, 4));
  for (u16 i = 0; i < sizeof(screen_mirror); i++) {
    vram_put(3);
  }
  lens.refill();

  oam_clear();
  lens.draw_sprite();
  oam_hide_rest();
  ppu_on_all();
  pal_fade_to(0, 4);
}

Gameplay::~Gameplay() {
  pal_fade_to(4, 0);
  ppu_off();
}

void Gameplay::run() {
  while (num_imposters > 0 && global_state.misses < 3 &&
         global_state.timer_seconds > 0) {
    ppu_wait_nmi();

    global_state.p1_input.poll();

    if (global_state.p1_input.pressed() & (PAD_A | PAD_B)) {
      // antibiotic shot
      inject_creature();
    }

    lens.update();

    global_state.timer_frames++;
    if (global_state.timer_frames == 60) {
      global_state.timer_frames = 0;
      global_state.timer_seconds--;
    }

    refresh_hud();

    lens.draw_sprite();
    oam_hide_rest();
  }

  oam_clear();

  if (num_imposters == 0) {
    global_state.plates_cleared++;
    if (global_state.timer_seconds < 180) {
      global_state.timer_seconds += 5;
    }
    banked_oam_meta_spr_horizontal(0x80, 0x80, UI_BANK, UI::Nice);
  } else {
    banked_oam_meta_spr_horizontal(0x80, 0x80, UI_BANK, UI::Fail);
  }

  oam_hide_rest();

  // reveal everything
  u16 index = 0;
  for (u8 row = 4; row < 30; row++, index += 0x20) {
    ppu_wait_nmi();
    multi_vram_buffer_horz(screen_mirror + index, 0x20, NTADR_A(0, row));
  }

  while (true) {
    rand16();
    ppu_wait_nmi();

    global_state.p1_input.poll();

    if (global_state.p1_input.pressed() & (PAD_START | PAD_A | PAD_B)) {
      break;
    }
  }
}

const u8 fixed_mask_lut[][6] = {
    {0b00000000, 0, 0, 0, 0, 0},                            // 0
    {0b00000011, 0b00001100, 0b00110000, 0b11000000, 0, 0}, // 1
    {0b00001111, 0b00110011, 0b00111100, 0b11000011, 0b11001100,
     0b11110000},                                           // 2
    {0b00111111, 0b11001111, 0b11110011, 0b11111100, 0, 0}, // 3
    {0b11111111, 0, 0, 0, 0, 0}                             // 4
};

const u8 fixed_mask_lut_sizes[] = {1, 4, 6, 4, 1};

void Gameplay::setup_creatures() {

  const u8 mask =
      fixed_mask_lut[num_fixed_features]
                    [subrand8(fixed_mask_lut_sizes[num_fixed_features] - 1)];

  for (u8 i = 0; i < num_creatures; i++) {
  retry:
    u8 row = subrand8(8);
    u8 column =
        subrand8(num_columns_per_row[row] - 1) + start_column_per_row[row];
    row += starting_row;

    for (u8 j = 0; j < i; j++) {
      if (row == creature[j].row && column == creature[j].column) {
        goto retry;
      }
    }

    creature[i].row = row;
    creature[i].column = column;
    creature[i].genes = rand8();
    creature[i].target = false;
    if (i > 0) {
      creature[i].genes =
          (creature[0].genes & mask) | (creature[i].genes & ~mask);
    }
  }

  for (u8 i = 0; i < num_imposters; i++) {
    creature[i].genes ^= mask;
    creature[i].target = true;
  }

  Attributes::init();
  for (u8 i = 0; i < num_creatures; i++) {
    creature[i].draw();
    flush_vram_update2();
  }
  Attributes::flush();
}

void Gameplay::inject_creature() {
  auto lens_column = lens.x.as_i() / 16;
  auto lens_row = lens.y.as_i() / 16;
  for (u8 i = 0; i < num_creatures; i++) {
    Creature &c = creature[i];
    if (c.row == lens_row && c.column == lens_column) {
      c.splat(screen_mirror);
      if (c.target) {
        num_imposters--;
        creature[i] = creature[--num_creatures];
      } else {
        global_state.misses++;
      }
      break;
    }
  }
}

void Gameplay::refresh_hud() {
  one_vram_buffer(0x10 + num_imposters, NTADR_A(12, 2));

  u8 text_buffer[3] = {0x0e, 0x0e, 0x0e}; // '...'

  for (u8 i = 0; i < global_state.misses; i++) {
    text_buffer[i] = 0x58; // 'x'
  }
  multi_vram_buffer_horz(text_buffer, 3, NTADR_A(27, 2));

  Bindec::convert(global_state.timer_seconds, text_buffer);
  multi_vram_buffer_horz(text_buffer, 3, NTADR_A(15, 2));
}