#include "creature.hpp"
#include "attributes.hpp"
#include <nesdoug.h>
#include <neslib.h>

Creature::Creature() {}

void Creature::draw() {
  u8 top_tiles[2];
  u8 bottom_tiles[2];

  top_tiles[0] = 0x60 + 8 * shape + 2 * eyes;
  top_tiles[1] = top_tiles[0] + 1;
  bottom_tiles[0] = 0x80 + 8 * shape + 2 * mouth;
  bottom_tiles[1] = bottom_tiles[0] + 1;

  multi_vram_buffer_horz(top_tiles, 2, NTADR_A(2 * column, 2 * row));
  multi_vram_buffer_horz(bottom_tiles, 2, NTADR_A(2 * column, 2 * row + 1));
  Attributes::set(column, row, color);
}

void Creature::erase() {
  const u8 zero_tiles[2] = {0, 0};

  multi_vram_buffer_horz(zero_tiles, 2, NTADR_A(2 * column, 2 * row));
  multi_vram_buffer_vert(zero_tiles, 2, NTADR_B(2 * column, 2 * row + 1));
  Attributes::set(column, row, 0);
}

void Creature::splat(u8 (&mirror)[]) {
  const u8 splat_top[] = {
      0xb0,
      0xb1,
  };
  const u8 splat_bottom[] = {
      0xc0,
      0xc1,
  };

  multi_vram_buffer_horz(splat_top, 2, NTADR_A(2 * column, 2 * row));
  multi_vram_buffer_horz(splat_bottom, 2, NTADR_A(2 * column, 2 * row + 1));

  u16 index = (u16) NTADR_A(2 * column, 2 * row) - NTADR_A(0, 4);

  mirror[index] = 0xb0;
  mirror[index + 1] = 0xb1;
  mirror[index + 0x20] = 0xc0;
  mirror[index + 0x21] = 0xc1;
}