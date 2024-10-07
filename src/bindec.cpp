#include "bindec.hpp"

void Bindec::convert(u8 value, u8 (&buffer)[]) {
  u8 digit = 0;
  if (value >= 200) {
    value -= 200;
    digit += 2;
  }
  if (value >= 100) {
    value -= 100;
    digit += 1;
  }
  buffer[0] = 0x10 + digit;
  digit = 0;
  if (value >= 80) {
    value -= 80;
    digit += 8;
  }
  if (value >= 40) {
    value -= 40;
    digit += 4;
  }
  if (value >= 20) {
    value -= 20;
    digit += 2;
  }
  if (value >= 10) {
    value -= 10;
    digit += 1;
  }
  buffer[1] = 0x10 + digit;
  buffer[2] = 0x10 + value;

  // replace left zeros with spaces
  for (u8 i = 0; i < 2; i++) {
    if (buffer[i] == 0x10) {
      buffer[i] = 0x02;
    } else {
      break;
    }
  }
}
