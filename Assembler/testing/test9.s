# Test 09: RV64 with M-extension (includes standard M ops + W variants)
.text
  # RV64-only W variants
  mulw   x5,  x6,  x7
  divw   x8,  x9,  x10
  divuw  x11, x12, x13
  remw   x14, x15, x16
  remuw  x17, x18, x19

  ecall
