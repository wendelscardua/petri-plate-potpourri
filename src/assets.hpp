#pragma once

#include <soa.h>

const int PPU_PATTERN_TABLE_0 = 0x0000;
const int PPU_PATTERN_TABLE_1 = 0x1000;

extern "C" const char bg_chr[];
extern "C" const char spr_chr[];

extern "C" const char screen_nam[];
extern "C" const char title_nam[];

extern "C" const char bg_pal[];
extern "C" const char spr_pal[];
