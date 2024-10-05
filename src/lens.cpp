#include "lens.hpp"
#include "banked-metasprites.hpp"
#include "input.hpp"
#include "metasprites.hpp"
#include <nesdoug.h>
#include <neslib.h>

Lens::Lens(u8 x, u8 y, Input &input, u8 (&mirror)[])
    : mirror(mirror), x(x, 0), y(y, 0), x_speed(0), y_speed(0), input(input),
      horizontal_direction(HorizontalNeutral), vertical_direction(VerticalNeutral) {}

void Lens::draw_sprite() {
  banked_oam_meta_spr_horizontal(x.as_i(), y.as_i() - 1, UI_BANK, UI::lens);
}

void Lens::refill() {
  // lens size = 6x6 tiles
  u8 start_row = y.as_i() / 8 - 3;
  u8 start_col = x.as_i() / 8 - 3;
  for (u8 i = 0; i < 6; i++) {
    vram_adr((unsigned int)NTADR_A(start_col, start_row + i));
    vram_write(mirror + NTADR_A(start_col, start_row + i) - NTADR_A(0, 4), 6);
  }
}

void Lens::update() {
  if (input.held() & PAD_UP) {
    if (y_speed > 0) {
      y_speed = 0;
    }
    y_speed -= acceleration;
    if (y_speed < -max_speed) {
      y_speed = -max_speed;
    }
    vertical_direction = Up;
  }
  if (input.held() & PAD_DOWN) {
    if (y_speed < 0) {
      y_speed = 0;
    }
    y_speed += acceleration;
    if (y_speed > max_speed) {
      y_speed = max_speed;
    }
    vertical_direction = Down;
  }
  if (input.held() & PAD_LEFT) {
    if (x_speed > 0) {
      x_speed = 0;
    }
    x_speed -= acceleration;
    if (x_speed < -max_speed) {
      x_speed = -max_speed;
    }
    horizontal_direction = Left;
  }
  if (input.held() & PAD_RIGHT) {
    if (x_speed < 0) {
      x_speed = 0;
    }
    x_speed += acceleration;
    if (x_speed > max_speed) {
      x_speed = max_speed;
    }
    horizontal_direction = Right;
  }
  x += x_speed;
  if (x <= min_x || x >= max_x) {
    x -= x_speed;
    x_speed = 0;
  }
  y += y_speed;
  if (y <= min_y || y >= max_y) {
    y -= y_speed;
    y_speed = 0;
  }
  if (x_speed > 0) {
    x_speed -= drag;
    if (x_speed < 0) {
      x_speed = 0;
    }
  }
  if (x_speed < 0) {
    x_speed += drag;
    if (x_speed > 0) {
      x_speed = 0;
    }
  }
  if (y_speed > 0) {
    y_speed -= drag;
    if (y_speed < 0) {
      y_speed = 0;
    }
  }
  if (y_speed < 0) {
    y_speed += drag;
    if (y_speed > 0) {
      y_speed = 0;
    }
  }

  if (x_speed == 0) {
    horizontal_direction = HorizontalNeutral;
  }
  if (y_speed == 0) {
    vertical_direction = VerticalNeutral;
  }

  const u8 blank_strip[] = {3, 3, 3, 3, 3, 3};

  switch (vertical_direction) {
  case Up: {
    u8 column = x.as_i() / 8 - 3;
    if (x.as_i() & 0b111) { // round up to the next tile
      column++;
    }
    // delete row below
    u8 old_row = y.as_i() / 8 + 3;
    if (y.as_i() & 0b111) { // round up to the next tile
      old_row++;
    }
    multi_vram_buffer_horz(blank_strip, 6, NTADR_A(column, old_row));
    // draw row above
    u8 new_row = y.as_i() / 8 - 3;
    multi_vram_buffer_horz(mirror + NTADR_A(column, new_row) - NTADR_A(0, 4), 6,
                           NTADR_A(column, new_row));
  } break;
  case Down: {
    u8 column = x.as_i() / 8 - 3;
    if (x.as_i() & 0b111) { // round up to the next tile
      column++;
    }
    // delete row above
    u8 old_row = y.as_i() / 8 - 4;
    multi_vram_buffer_horz(blank_strip, 6, NTADR_A(column, old_row));
    // draw row below
    u8 new_row = y.as_i() / 8 + 2;
    if (y.as_i() & 0b111) { // round up to the next tile
      new_row++;
    }
    multi_vram_buffer_horz(mirror + NTADR_A(column, new_row) - NTADR_A(0, 4), 6,
                           NTADR_A(column, new_row));
  } break;
  }

  u8 new_strip[6];

  switch (horizontal_direction) {
  case Left: {
    u8 row = y.as_i() / 8 - 3;
    if (y.as_i() & 0b111) { // round up to the next tile
      row++;
    }
    // delete right column
    u8 old_column = x.as_i() / 8 + 3;
    if (x.as_i() & 0b111) { // round up to the next tile
      old_column++;
    }
    multi_vram_buffer_vert(blank_strip, 6, NTADR_A(old_column, row));
    // draw left column
    u8 new_column = x.as_i() / 8 - 3;
    for (u8 i = 0; i < 6; i++) {
      new_strip[i] = mirror[NTADR_A(new_column, row + i) - NTADR_A(0, 4)];
    }
    multi_vram_buffer_vert(new_strip, 6, NTADR_A(new_column, row));
  } break;
  case Right: {
    u8 row = y.as_i() / 8 - 3;
    if (y.as_i() & 0b111) { // round up to the next tile
      row++;
    }
    // delete left column
    u8 old_column = x.as_i() / 8 - 4;
    multi_vram_buffer_vert(blank_strip, 6, NTADR_A(old_column, row));
    // draw right column
    u8 new_column = x.as_i() / 8 + 2;
    if (x.as_i() & 0b111) { // round up to the next tile
      new_column++;
    }
    for (u8 i = 0; i < 6; i++) {
      new_strip[i] = mirror[NTADR_A(new_column, row + i) - NTADR_A(0, 4)];
    }
    multi_vram_buffer_vert(new_strip, 6, NTADR_A(new_column, row));
  } break;
  }
}