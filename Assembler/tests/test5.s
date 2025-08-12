# Test 05: Stress the data directives, alignment, and spacing.

.data
  .align 3           # align to 8
d0:
  .byte  127
  .byte  -1
  .half  1234
  .half  -1234
  .word  2047
  .word  -2048
  .dword 42
  .dword -42
  .ascii  "RAW"
  .asciiz "Z-TERM"
  .space 16

  .align 2
d1:
  .word 1
  .word 2
  .word 3

.text
  la   x10, d0
  lw   x11, 8(x10)     # read one of the words
  ecall
