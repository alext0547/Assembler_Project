# Test 12: Auto-compress edge cases (RV64): includes LD/SD and ADDW/SUBW, no c.jal
    .text
_start:
    # --- CI addi self ---
    addi x5, x5, -32
    addi x6, x6, 31

    # --- CI nop / li ---
    addi x0, x0, 0
    addi x10, x0, -32
    addi x11, x0, 31

    # --- CI slli (sh in [0..63]) ---
    slli x12, x12, 0
    slli x13, x13, 63

    # --- CI' srli/srai (prime regs, sh in [0..63]) ---
    srli x8,  x8, 63
    srai x9,  x9, 63

    # --- CI' andi ---
    andi x10, x10, -32
    andi x11, x11, 31

    # --- CR mv/add ---
    add  x12, x0,  x13
    add  x14, x14, x15

    # --- JR/JALR (c.jr/c.jalr exist on RV64) ---
    addi x1, x0, 0
    jalr x0, 0(x1)
    jalr x1, 0(x2)

    # --- EBREAK ---
    ebreak

    # --- CA ALU ---
    sub  x8,  x8,  x9
    xor  x10, x10, x11
    or   x12, x12, x13
    and  x14, x14, x15

    # --- CAW ALU (RV64 only) ---
    addw x8,  x8,  x9
    subw x10, x10, x11

    # --- CL loads (LD allowed on RV64) ---
    lw   x8,  0(x9)            # compress
    ld   x10, 0(x11)           # compress
    ld   x12, 248(x13)         # compress (max)

    # --- CS stores ---
    sw   x8,  0(x9)            # compress
    sd   x10, 0(x11)           # compress
    sd   x12, 248(x13)         # compress (max)

    # --- CI(sp) loads (lwsp/ldsp bounds) ---
    lw   x5,  252(x2)          # compress (max)
    ld   x6,  504(x2)          # compress (max)

    # --- CSS(sp) stores (swsp/sdsp bounds) ---
    sw   x5,  252(x2)          # compress (max)
    sd   x6,  504(x2)          # compress (max)

    # --- CB edges ---
cb64_neg_label:
    .space 256
cb64_from:
    beq  x8, x0, cb64_neg_label
cb64_pos_from:
    bne  x8, x0, cb64_pos_label
    .space 252
cb64_pos_label:

    # --- CJ edges (no c.jal on RV64, but c.j works) ---
cj64_neg_label:
    .space 2048
cj64_from:
    jal x0, cj64_neg_label
cj64_pos_from:
    jal x0, cj64_pos_label
    .space 2044
cj64_pos_label:
