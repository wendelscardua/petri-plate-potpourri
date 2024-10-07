#include "gameplay.hpp"
#include "assets-loader.hpp"
#include "attributes.hpp"
#include "banked-metasprites.hpp"
#include "bindec.hpp"
#include "ggsound.hpp"
#include "global.hpp"
#include "metasprites.hpp"
#include "soundtrack.hpp"
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
  GGSound::play_song(Song::Pickem);
  pal_fade_to(0, 4);
}

Gameplay::~Gameplay() {
  pal_fade_to(4, 0);
  ppu_off();
}

void Gameplay::cleanup() {
  const u8 start_popup_y = 0xe0;
  const u8 target_popup_y = 0x58;
  oam_clear();

  const Sprite *popup;
  u8 popup_y = start_popup_y;

  if (num_imposters == 0) {
    global_state.plates_cleared++;
    if (global_state.timer_seconds < 180) {
      global_state.timer_seconds += 5;
    }
    popup = UI::Nice;
  } else {
    popup = UI::Fail;
  }

  // reveal everything
  u16 index = 0;
  for (u8 row = 4; row < 30; row++, index += 0x20) {
    ppu_wait_nmi();
    multi_vram_buffer_horz(screen_mirror + index, 0x20, NTADR_A(0, row));
  }

  GGSound::stop();
  if (num_imposters == 0) {
    GGSound::play_sfx(SFX::Nice, GGSound::SFXPriority::One);
  } else {
    GGSound::play_sfx(SFX::Fail, GGSound::SFXPriority::One);
  }

  for (u16 continue_delay = 0; continue_delay < 5 * 60; continue_delay++) {
    rand16();
    ppu_wait_nmi();
    banked_oam_meta_spr_horizontal(0x80, popup_y, UI_BANK, popup);
    popup_y = popup_y - (popup_y - target_popup_y) / 8;
    oam_hide_rest();

    global_state.p1_input.poll();

    if (global_state.p1_input.pressed() & (PAD_START | PAD_A | PAD_B)) {
      break;
    }
  }
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
      if (global_state.timer_seconds == 30) {
        GGSound::play_sfx(SFX::TimeAlmostOut, GGSound::SFXPriority::One);
      } else if (global_state.timer_seconds == 0) {
        GGSound::play_sfx(SFX::TimeOut, GGSound::SFXPriority::One);
      }
    }

    refresh_hud();

    lens.draw_sprite();
    oam_hide_rest();
  }

  // final steps before leaving gameplay

  cleanup();
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
  }

  u8 expected_genes = rand8() & mask;

  for (u8 i = 0; i < num_creatures; i++) {
    if (i < num_imposters) {
      creature[i].target = true;
      u8 temp;
      do {
        temp = rand8();
      } while ((temp & mask) == expected_genes);
      creature[i].genes = temp;
    } else {
      creature[i].target = false;
      creature[i].genes = expected_genes | (rand8() & ~mask);
    }
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
        GGSound::play_sfx(SFX::GoodHit, GGSound::SFXPriority::One);
      } else {
        global_state.misses++;
        GGSound::play_sfx(SFX::BadHit, GGSound::SFXPriority::One);
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