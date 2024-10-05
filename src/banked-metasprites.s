.section .prg_rom_fixed,"axR",@progbits

.global banked_oam_meta_spr_horizontal

; void banked_oam_meta_spr_horizontal(int x, char y, char bank, const void *data);
; A/rc6: x.l
; X/rc7: x.h
; rc2: y
; rc3: bank
; rc4: data.l
; rc5: data.h
; rc8: extend signal for sprite delta x

banked_oam_meta_spr_horizontal:
  sta __rc6
  stx __rc7

  jsr get_prg_bank
  pha
  lda __rc3
  jsr set_prg_bank

  ldx SPRID
  ldy #0
1:
  lda (__rc4),y  ;x offset
  cmp #$80
  beq 2f
  
  bcs 5f ; go to extend negative

  lda #0
  sta __rc8 ; extend positive
  jmp 6f
5: 
  lda #$ff
  sta __rc8
6: ; proceed with x offset
  lda (__rc4),y ; restore x offset
  iny
  clc
  adc __rc6
  sta OAM_BUF+3,x

  lda __rc7
  adc __rc8

  ; high byte zero = on screen (?)
  beq 4f

  ; not on screen, go to next sprite
  iny
  iny
  iny
  jmp 1b

4:
  
  lda (__rc4),y  ; y offset
  iny
  clc
  adc __rc2
  sta OAM_BUF+0,x
    
  lda (__rc4),y  ;tile
  iny
  sta OAM_BUF+1,x
  lda (__rc4),y  ;attribute
  iny
  sta OAM_BUF+2,x
  inx
  inx
  inx
  inx
  jmp 1b
2:
  stx SPRID
  pla
  jsr set_prg_bank
  rts

.global banked_oam_meta_spr_horizontal_recolor

; void banked_oam_meta_spr_horizontal_recolor(int x, char y, char attr, char bank, const void *data);
; A/rc8: x.l
; X/rc9: x.h
; rc2: y
; rc3: attr
; rc4: bank
; rc6: data.l
; rc7: data.h
; rc10: extend signal for sprite delta x

banked_oam_meta_spr_horizontal_recolor:
  sta __rc8
  stx __rc9

  jsr get_prg_bank
  pha
  lda __rc4
  jsr set_prg_bank

  ldx SPRID
  ldy #0
1:
  lda (__rc6),y  ;x offset
  cmp #$80
  beq 2f
  
  bcs 5f ; go to extend negative

  lda #0
  sta __rc10 ; extend positive
  jmp 6f
5: 
  lda #$ff
  sta __rc10
6: ; proceed with x offset
  lda (__rc6),y ; restore x offset
  iny
  clc
  adc __rc8
  sta OAM_BUF+3,x

  lda __rc9
  adc __rc10

  ; high byte zero = on screen (?)
  beq 4f

  ; not on screen, go to next sprite
  iny
  iny
  iny
  jmp 1b

4:
  
  lda (__rc6),y  ; y offset
  iny
  clc
  adc __rc2
  sta OAM_BUF+0,x
    
  lda (__rc6),y  ;tile
  iny
  sta OAM_BUF+1,x
  lda __rc3  ;attribute
  iny
  sta OAM_BUF+2,x
  inx
  inx
  inx
  inx
  jmp 1b
2:
  stx SPRID
  pla
  jsr set_prg_bank
  rts