# for (i = 0; i < 10; i++) {
#   if (i == 3) continue;
#   sum += i;
# }

.text
  addi x5, x0, 0        # i = 0
  addi x7, x0, 0        # sum = 0
  addi x6, x0, 10       # limit = 10

for_cond:
  blt  x5, x6, for_body
  jal  x0, for_end

for_body:
  addi x10, x0, 3
  sub  x11, x5, x10
  beq  x11, x0, for_inc   # if (i==3) continue;
  add  x7, x7, x5         # sum += i
for_inc:
  addi x5, x5, 1
  jal  x0, for_cond

for_end:
  ecall