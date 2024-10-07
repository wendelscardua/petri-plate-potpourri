#pragma once

#include "common.hpp"

#define ASSETS_LOADER_TEXT                                                     \
  __attribute__((noinline, section(".prg_rom_fixed.assets_loader.text")))

class AssetsLoader {
  u8 bank;

public:
  enum class Palette {
    BG,
    SPR,
  };

  ASSETS_LOADER_TEXT AssetsLoader(u8 bank);

  ASSETS_LOADER_TEXT void load_chr(const void *data, u16 size);
  ASSETS_LOADER_TEXT void load_nam(const void *data, u16 size);
  ASSETS_LOADER_TEXT void load_pal(Palette palette, const void *data);

  ASSETS_LOADER_TEXT static void load_title_assets();
  ASSETS_LOADER_TEXT static void load_gameplay_assets();
  ASSETS_LOADER_TEXT static void load_score_assets();
};