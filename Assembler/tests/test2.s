# if (x < y) t = 1;
# else if (x == y) t = 2;
# else t = 3;

.text
  addi x5, x0, 5        # x = 5
  addi x6, x0, 7        # y = 7
  addi x8, x0, 0        # t = 0

  slt  x7, x5, x6       # x7 = (x<y)
  beq  x7, x0, check_eq
  addi x8, x0, 1        # t = 1
  jal  x0, end

check_eq:
  sub  x9, x5, x6
  beq  x9, x0, set_two
  addi x8, x0, 3        # t = 3
  jal  x0, end

set_two:
  addi x8, x0, 2        # t = 2

end:
  ecall