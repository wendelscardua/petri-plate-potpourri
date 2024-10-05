#include "assets-loader.hpp"
#include "assets.hpp"
#include "bank-helper.hpp"
#include <neslib.h>

ASSETS_LOADER_TEXT AssetsLoader::AssetsLoader(u8 bank) : bank(bank) {}

ASSETS_LOADER_TEXT void
AssetsLoader::load_gameplay_assets() {
  AssetsLoader loader(0);
  vram_adr(NAMETABLE_A);

  loader.load_nam(screen_nam, 1024);
  vram_adr(PPU_PATTERN_TABLE_0);
  loader.load_chr(bg_chr, 4096);
  vram_adr(PPU_PATTERN_TABLE_1);
  loader.load_chr(spr_chr, 4096);
}

ASSETS_LOADER_TEXT void AssetsLoader::load_chr(const void *data, u16 size) {
  ScopedBank scope(bank);
  // TODO: maybe use donut compression
  vram_write(data, size);
}

ASSETS_LOADER_TEXT void AssetsLoader::load_nam(const void *data, u16 size) {
  ScopedBank scope(bank);
  // TODO: maybe use zx compression
  vram_write(data, size);
}

ASSETS_LOADER_TEXT void AssetsLoader::load_pal(Palette palette,
                                               const void *data) {
  ScopedBank scope(bank);

  switch (palette) {
  case Palette::BG:
    pal_bg(data);
    break;
  case Palette::SPR:
    pal_spr(data);
    break;
  }
}