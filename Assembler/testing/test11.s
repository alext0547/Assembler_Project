# Test 11: Auto-compress edge cases (RV32): CI/CR/CB/CJ, CL/CS/CSS/CI(sp)
    .text
_start:
    # --- CI: addi self (imm in [-32..31]) ---
    addi x5, x5, -32          # compress
    addi x6, x6, 31           # compress
    addi x7, x7, 32           # NOT (out of range)

    # --- CI: nop / li ---
    addi x0, x0, 0            # c.nop
    addi x0, x0, 1            # NOT (rd==x0, imm!=0)
    addi x10, x0, -32         # c.li via addi
    addi x11, x0, 31          # c.li via addi

    # --- CI: slli (rd!=x0, rd==rs1, sh in [0..31]) ---
    slli x12, x12, 0          # compress
    slli x13, x13, 31         # compress
    slli x0,  x0,  1          # NOT (rd==x0)
    slli x14, x15, 1          # NOT (rd!=rs1)

    # --- CI': srli/srai (rd in x8..x15, sh in [0..31]) ---
    srli x8,  x8, 31          # compress
    srai x9,  x9, 31          # compress
    srli x7,  x7, 1           # NOT (rd not prime range)
    srai x16, x16, 1          # NOT (rd not prime range)

    # --- CI': andi (rd in x8..x15, imm in [-32..31]) ---
    andi x10, x10, -32        # compress
    andi x11, x11, 31         # compress
    andi x7,  x7,  1          # NOT (rd not prime range)

    # --- CR: mv/add ---
    add  x12, x0,  x13        # c.mv
    add  x14, x14, x15        # c.add
    add  x0,  x1,  x2         # NOT (rd==x0)

    # --- JR/JALR ---
    addi x1, x0, 0
    jalr x0, 0(x1)            # c.jr
    jalr x1, 0(x2)            # c.jalr
    jalr x1, 4(x2)            # NOT (imm!=0)

    # --- EBREAK ---
    ebreak                    # c.ebreak

    # --- CA: sub/xor/or/and (rd',rs2' in x8..x15, rd==rs1) ---
    sub  x8,  x8,  x9         # compress
    xor  x10, x10, x11        # compress
    or   x12, x12, x13        # compress
    and  x14, x14, x15        # compress

    # --- CL loads (uimm multiples / max) ---
    lw   x8,  0(x9)           # compress
    lw   x10, 124(x11)        # compress (max)
    lw   x7,  0(x9)           # NOT (rd not prime)
    lw   x8,  2(x9)           # NOT (unaligned)

    # --- CS stores ---
    sw   x10, 0(x11)          # compress
    sw   x12, 124(x13)        # compress (max)
    sw   x0,  0(x11)          # NOT (rs2==x0)
    sw   x10, 2(x11)          # NOT (unaligned)

    # --- CI(sp) loads (rd!=x0; lwsp up to 252) ---
    lw   x5,  0(x2)           # compress
    lw   x6,  252(x2)         # compress (max)
    lw   x0,  0(x2)           # NOT (rd==x0)
    lw   x7,  2(x2)           # NOT (unaligned)

    # --- CSS(sp) stores (swsp up to 252) ---
    sw   x5,  0(x2)           # compress
    sw   x6,  252(x2)         # compress (max)
    sw   x0,  0(x2)           # NOT (rs2==x0)
    sw   x7,  2(x2)           # NOT (unaligned)

    # --- CB edge ranges: -256 and +254 (even) ---
cb_neg_label:
    .space 256
cb_from:
    beq  x8, x0, cb_neg_label   # disp = -256 (edge)
cb_pos_from:
    beq  x8, x0, cb_pos_label
    .space 252                  # if compressed (2) => 2+252 = 254 (edge)
cb_pos_label:

    # --- CJ edges (RV32): j and jal (even) ---
cj_neg_label:
    .space 2048
cj_from:
    jal x0, cj_neg_label        # disp = -2048 (edge)
cj_pos_from_j:
    jal x0, cj_pos_j_label
    .space 2044                 # if compressed (2) => 2046 (edge)
cj_pos_j_label:

    # c.jal edges (RV32 only)
cj_neg_label_jal:
    .space 2048
cj_from_jal:
    jal x1, cj_neg_label_jal    # disp = -2048 (edge)
cj_pos_from_jal:
    jal x1, cj_pos_jal_label
    .space 2044                 # if compressed (2) => 2046 (edge)
cj_pos_jal_label:
