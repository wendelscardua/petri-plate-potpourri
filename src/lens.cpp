#include "lens.hpp"
#include "banked-metasprites.hpp"
#include "input.hpp"
#include "metasprites.hpp"
#include <neslib.h>

Lens::Lens(u8 x, u8 y, Input& input, u8 (&mirror)[]) : mirror(mirror), x(x, 0), y(y, 0), x_speed(0), y_speed(0), input(input), horizontal_direction(Right), vertical_direction(Down) {}

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
}