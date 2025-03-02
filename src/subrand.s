.section .zp
subrand_mask: .zero 1
subrand_bound: .zero 1

.section .prg_rom_last.text,"aR",@progbits
.global rand8
.global subrand8

subrand8:
  sta subrand_bound
  tax
  lda upper_bound_mask_lt, X
  beq return
  sta subrand_mask
loop:
  jsr rand8
  and subrand_mask
  cmp subrand_bound
  bcc return
  jmp loop
return:
  rts

.section .prg_rom_last.rodata,"aR",@progbits
upper_bound_mask_lt:
.byte 0 ; 0
.byte 0 ; 1
.byte 1 ; 2
.byte 3 ; 3
.byte 3 ; 4
.byte 7 ; 5
.byte 7 ; 6
.byte 7 ; 7
.byte 7 ; 8
.byte 15 ; 9
.byte 15 ; 10
.byte 15 ; 11
.byte 15 ; 12
.byte 15 ; 13
.byte 15 ; 14
.byte 15 ; 15
.byte 15 ; 16
.byte 31 ; 17
.byte 31 ; 18
.byte 31 ; 19
.byte 31 ; 20
.byte 31 ; 21
.byte 31 ; 22
.byte 31 ; 23
.byte 31 ; 24
.byte 31 ; 25
.byte 31 ; 26
.byte 31 ; 27
.byte 31 ; 28
.byte 31 ; 29
.byte 31 ; 30
.byte 31 ; 31
.byte 31 ; 32
.byte 63 ; 33
.byte 63 ; 34
.byte 63 ; 35
.byte 63 ; 36
.byte 63 ; 37
.byte 63 ; 38
.byte 63 ; 39
.byte 63 ; 40
.byte 63 ; 41
.byte 63 ; 42
.byte 63 ; 43
.byte 63 ; 44
.byte 63 ; 45
.byte 63 ; 46
.byte 63 ; 47
.byte 63 ; 48
.byte 63 ; 49
.byte 63 ; 50
.byte 63 ; 51
.byte 63 ; 52
.byte 63 ; 53
.byte 63 ; 54
.byte 63 ; 55
.byte 63 ; 56
.byte 63 ; 57
.byte 63 ; 58
.byte 63 ; 59
.byte 63 ; 60
.byte 63 ; 61
.byte 63 ; 62
.byte 63 ; 63
.byte 63 ; 64
.byte 127 ; 65
.byte 127 ; 66
.byte 127 ; 67
.byte 127 ; 68
.byte 127 ; 69
.byte 127 ; 70
.byte 127 ; 71
.byte 127 ; 72
.byte 127 ; 73
.byte 127 ; 74
.byte 127 ; 75
.byte 127 ; 76
.byte 127 ; 77
.byte 127 ; 78
.byte 127 ; 79
.byte 127 ; 80
.byte 127 ; 81
.byte 127 ; 82
.byte 127 ; 83
.byte 127 ; 84
.byte 127 ; 85
.byte 127 ; 86
.byte 127 ; 87
.byte 127 ; 88
.byte 127 ; 89
.byte 127 ; 90
.byte 127 ; 91
.byte 127 ; 92
.byte 127 ; 93
.byte 127 ; 94
.byte 127 ; 95
.byte 127 ; 96
.byte 127 ; 97
.byte 127 ; 98
.byte 127 ; 99
.byte 127 ; 100
.byte 127 ; 101
.byte 127 ; 102
.byte 127 ; 103
.byte 127 ; 104
.byte 127 ; 105
.byte 127 ; 106
.byte 127 ; 107
.byte 127 ; 108
.byte 127 ; 109
.byte 127 ; 110
.byte 127 ; 111
.byte 127 ; 112
.byte 127 ; 113
.byte 127 ; 114
.byte 127 ; 115
.byte 127 ; 116
.byte 127 ; 117
.byte 127 ; 118
.byte 127 ; 119
.byte 127 ; 120
.byte 127 ; 121
.byte 127 ; 122
.byte 127 ; 123
.byte 127 ; 124
.byte 127 ; 125
.byte 127 ; 126
.byte 127 ; 127
.byte 127 ; 128
.byte 255 ; 129
.byte 255 ; 130
.byte 255 ; 131
.byte 255 ; 132
.byte 255 ; 133
.byte 255 ; 134
.byte 255 ; 135
.byte 255 ; 136
.byte 255 ; 137
.byte 255 ; 138
.byte 255 ; 139
.byte 255 ; 140
.byte 255 ; 141
.byte 255 ; 142
.byte 255 ; 143
.byte 255 ; 144
.byte 255 ; 145
.byte 255 ; 146
.byte 255 ; 147
.byte 255 ; 148
.byte 255 ; 149
.byte 255 ; 150
.byte 255 ; 151
.byte 255 ; 152
.byte 255 ; 153
.byte 255 ; 154
.byte 255 ; 155
.byte 255 ; 156
.byte 255 ; 157
.byte 255 ; 158
.byte 255 ; 159
.byte 255 ; 160
.byte 255 ; 161
.byte 255 ; 162
.byte 255 ; 163
.byte 255 ; 164
.byte 255 ; 165
.byte 255 ; 166
.byte 255 ; 167
.byte 255 ; 168
.byte 255 ; 169
.byte 255 ; 170
.byte 255 ; 171
.byte 255 ; 172
.byte 255 ; 173
.byte 255 ; 174
.byte 255 ; 175
.byte 255 ; 176
.byte 255 ; 177
.byte 255 ; 178
.byte 255 ; 179
.byte 255 ; 180
.byte 255 ; 181
.byte 255 ; 182
.byte 255 ; 183
.byte 255 ; 184
.byte 255 ; 185
.byte 255 ; 186
.byte 255 ; 187
.byte 255 ; 188
.byte 255 ; 189
.byte 255 ; 190
.byte 255 ; 191
.byte 255 ; 192
.byte 255 ; 193
.byte 255 ; 194
.byte 255 ; 195
.byte 255 ; 196
.byte 255 ; 197
.byte 255 ; 198
.byte 255 ; 199
.byte 255 ; 200
.byte 255 ; 201
.byte 255 ; 202
.byte 255 ; 203
.byte 255 ; 204
.byte 255 ; 205
.byte 255 ; 206
.byte 255 ; 207
.byte 255 ; 208
.byte 255 ; 209
.byte 255 ; 210
.byte 255 ; 211
.byte 255 ; 212
.byte 255 ; 213
.byte 255 ; 214
.byte 255 ; 215
.byte 255 ; 216
.byte 255 ; 217
.byte 255 ; 218
.byte 255 ; 219
.byte 255 ; 220
.byte 255 ; 221
.byte 255 ; 222
.byte 255 ; 223
.byte 255 ; 224
.byte 255 ; 225
.byte 255 ; 226
.byte 255 ; 227
.byte 255 ; 228
.byte 255 ; 229
.byte 255 ; 230
.byte 255 ; 231
.byte 255 ; 232
.byte 255 ; 233
.byte 255 ; 234
.byte 255 ; 235
.byte 255 ; 236
.byte 255 ; 237
.byte 255 ; 238
.byte 255 ; 239
.byte 255 ; 240
.byte 255 ; 241
.byte 255 ; 242
.byte 255 ; 243
.byte 255 ; 244
.byte 255 ; 245
.byte 255 ; 246
.byte 255 ; 247
.byte 255 ; 248
.byte 255 ; 249
.byte 255 ; 250
.byte 255 ; 251
.byte 255 ; 252
.byte 255 ; 253
.byte 255 ; 254
.byte 255 ; 255
