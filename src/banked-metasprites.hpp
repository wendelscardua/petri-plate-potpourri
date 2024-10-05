#pragma once

// draws metasprite from the metasprite bank w/ horizontal scroll culling
extern "C" void banked_oam_meta_spr_horizontal(int x, char y, char bank,
                                               const void *data);

// draws metasprite from the metasprite bank w/ horizontal scroll culling
// while also recoloring tiles with a given attribute
extern "C" void banked_oam_meta_spr_horizontal_recolor(int x, char y, char attr,
                                                       char bank,
                                                       const void *data);