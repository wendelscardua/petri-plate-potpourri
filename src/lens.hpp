#pragma once

#include "common.hpp"
#include "input.hpp"
#include "fixed-point.hpp"
class Lens {
public:
  u8 (&mirror)[];

  FixedPoint<8, 8, false> x;
  FixedPoint<8, 8, false> y;
  FixedPoint<8, 8, true> x_speed;
  FixedPoint<8, 8, true> y_speed;

  static constexpr FixedPoint<8, 8, false> min_x = {32, 0}, max_x = {256 - 32, 0};
  static constexpr FixedPoint<8, 8, false> min_y = {64, 0}, max_y = {240 - 32, 0};
  static constexpr FixedPoint<8, 8, true> max_speed = {1, 128};

  static constexpr FixedPoint<8, 8, true> acceleration = {0, 8};

  static constexpr FixedPoint<8, 8, true> drag = { 0, 4};

  Input& input;

  enum HorizontalDirection {
    Left,
    HorizontalNeutral,
    Right,
  } horizontal_direction;

  enum VerticalDirection {
    Up,
    VerticalNeutral,
    Down,
  } vertical_direction;

  Lens(u8 x, u8 y, Input& input, u8 (&mirror)[]);

  // redraws all the tiles visible inside of it - assume rendering is off
  void refill();

  void update();

  void draw_sprite();
};