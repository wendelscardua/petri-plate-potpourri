    .section .prg_rom_0,"aR",@progbits

    ; CHR data

    .global bg_chr
    bg_chr:
    .incbin "bg.chr"

    .global spr_chr
    spr_chr:
    .incbin "spr.chr"

    ; Palettes

    .global bg_pal
    bg_pal:
    .incbin "bg.pal"

    .global spr_pal
    spr_pal:
    .incbin "spr.pal"

    ; Nametables

    .global title_nam
    title_nam:
    .incbin "title.nam"

    .global screen_nam
    screen_nam:
    .incbin "screen.nam"
