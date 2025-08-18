# Test 08: RV32 with M-extension
.text
  mul    x5,  x6,  x7
  mulh   x8,  x9,  x10
  mulhsu x11, x12, x13
  mulhu  x14, x15, x16

  div    x17, x18, x19
  divu   x20, x21, x22
  rem    x23, x24, x25
  remu   x26, x27, x28

  ecall
