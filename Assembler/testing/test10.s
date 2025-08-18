# Test 10: All C extended instructions
    .text
_start:
    # --- CI (incl. c.nop) ---
    c.addi  x0, 0            # c.nop
    c.addi  x1, 1
    c.slli  x1, 3
    c.srli  x8, 4            # rd' must be x8..x15
    c.srai  x9, 5
    c.andi  x10, -1
    c.lui   x3, 1
    c.lwsp  x5, 16
    c.ldsp  x6, 24           # RV64C only

    # --- CR group ---
    c.mv    x4, x5
    c.add   x6, x7
    c.jr    x1
    c.jalr  x1
    c.ebreak

    # --- CA (rd', rs2' in x8..x15) ---
    c.sub   x8,  x9
    c.xor   x8,  x9
    c.or    x8,  x9
    c.and   x8,  x9
    c.subw  x8,  x9          # RV64C only
    c.addw  x8,  x9          # RV64C only

    # --- CL / CS ---
    c.lw    x8,  0(x9)
    c.ld    x8,  8(x9)       # RV64C only, uimm multiple of 8
    c.sw    x10, 4(x11)
    c.sd    x10, 8(x11)      # RV64C only

    # --- CSS ---
    c.swsp  x14, 8
    c.sdsp  x15, 16          # RV64C only

    # --- CB (rs1' in x8..x15) ---
    c.beqz  x12, Lbeqz
    c.bnez  x13, Lbnez

    # --- CJ (no c.jal in RV64C) ---
    c.j     Lj

Lbeqz:
    c.nop
Lbnez:
    c.nop
Lj:
    c.nop
