# Test 07: RV64I/RV64I+Zifencei subset used here (no M-extension)
.text
  addiw x5,  x6, -1
  slliw x7,  x7, 31
  srliw x8,  x8, 1
  sraiw x9,  x9, 5

  lwu   x10, 0(x0)
  ld    x11, 8(x0)
  sd    x11, 16(x0)

  addw  x12, x13, x14
  subw  x15, x16, x17
  sllw  x18, x19, x20
  srlw  x21, x22, x23
  sraw  x24, x25, x26

  ecall
