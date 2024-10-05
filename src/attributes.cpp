#include "attributes.hpp"
#include <nesdoug.h>

namespace Attributes {
  u8 cache[64];
  u8 last_index;

  void init() {
    last_index = 0xff;
    for (auto &item : cache) {
      item = 0;
    }
  }

  void set(u8 metatile_x, u8 metatile_y, u8 attribute) {
    u8 attr_index = (metatile_y / 2) * 32 / 4 + (metatile_x / 2);
    u8 attr_shift = 2 * ((metatile_y % 2) * 2 + metatile_x % 2);

    if (attr_index != last_index) {
      flush();
    }

    cache[attr_index] =
        (cache[attr_index] & (0xff ^ ((u8)(0b11 << attr_shift)))) |
        ((u8)(attribute << attr_shift));

    last_index = attr_index;
  }

  void flush() {
    if (last_index != 0xff) {
      one_vram_buffer(cache[last_index], 0x23c0 + last_index);
      last_index = 0xff;
    }
  }
}; // namespace Attributes