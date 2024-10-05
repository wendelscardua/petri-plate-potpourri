#include "gameplay.hpp"
#include "assets-loader.hpp"
#include "global.hpp"
#include "subrand.hpp"
#include <nesdoug.h>
#include <neslib.h>

static const u8 starting_row = 3;

static const u8 start_column_per_row[] = {
    4, 3, 2, 1, 1, 1, 1, 1, 2, 3, 4,
};

static const u8 num_columns_per_row[] = {
    8, 10, 12, 14, 14, 14, 14, 14, 12, 10, 8,
};

Gameplay::Gameplay(Global &global_state) : global_state(global_state) {
  num_creatures = MAX_CREATURES; // TODO: change this
  for (u8 i = 0; i < num_creatures; i++) {
    u8 row = subrand8(10);
    u8 column = subrand8(num_columns_per_row[row] - 1) + start_column_per_row[row];
    row += starting_row;

    creature[i].row = row;
    creature[i].column = column;
    creature[i].genes = rand8();
  }

  AssetsLoader::load_gameplay_assets();

  ppu_on_all();
  pal_fade_to(0, 4);
}

Gameplay::~Gameplay() { pal_fade_to(4, 0); }

void Gameplay::run() {
  while (true) {
    ppu_wait_nmi();

    global_state.p1_input.poll();

    if (global_state.p1_input.pressed() & PAD_START) {
      break;
    }
  }
}