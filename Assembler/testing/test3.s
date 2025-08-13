# Test 03: While loop coverage
# while (i < n) { if (i & 1) sum += i; i++; }

.text
  addi x5, x0, 0        # i = 0
  addi x6, x0, 8        # n = 8
  addi x7, x0, 0        # sum = 0

loop_cond:
  blt  x5, x6, loop_body
  jal  x0, loop_end

loop_body:
  andi x10, x5, 1
  beq  x10, x0, skip_add
  add  x7, x7, x5       # sum += i
skip_add:
  addi x5, x5, 1        # i++
  jal  x0, loop_cond

loop_end:
  ecall
