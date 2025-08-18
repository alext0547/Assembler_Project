# Test 13: Combination of everything
    .data                  # DOT_DATA
    .align 8               # ALIGN
data_block:
    .ascii  "HELLO"        # ASCII
    .asciiz " world!\n"    # ASCIIZ
    .byte   0x11           # BYTE
    .half   0x3344         # HALF
    .word   0x55667788     # WORD
    .dword  0x1122334455667788  # DWORD
    .space  32             # SPACE

    .text
end_prog:
    jal x0, end_prog       # hang
    .text                  # DOT_TEXT
    .align 4               # ALIGN
_start:

# --- Pseudos / quick sanity ---
    nop                    # NOP
    li    x5,  123         # LI
    mv    x6,  x5          # MV
    neg   x7,  x6          # NEG
    seqz  x8,  x7          # SEQZ
    snez  x9,  x8          # SNEZ
    subi  x10, x10, 1      # SUBI  (addi x10,x10,-1)

# --- U/AUIPC/LUI/LA ---
    lui   x11, 0x12000     # LUI
    auipc x12, 0           # AUIPC (PC-rel)
    la    x13, data_block  # LA (address of data)

# --- Jumps / branches (J/JAL/JALR/JR/RET/ECALL/EBREAK) ---
    j    near_label        # J
    jal  x1, near_label    # JAL
    jalr x0, 0(x1)         # JALR (ret target form)
    jr   x1                # JR     (jalr x0,x1,0)
    ret                    # RET    (jalr x0,x1,0)
    ecall                  # ECALL
    ebreak                 # EBREAK

near_label:
# --- Base R ops (RV64) ---
    add   x14, x14, x15    # ADD
    sub   x16, x16, x17    # SUB
    sll   x18, x18, x19    # SLL
    slt   x20, x21, x22    # SLT
    sltu  x23, x24, x25    # SLTU
    xor   x26, x26, x27    # XOR
    srl   x28, x28, x29    # SRL
    sra   x30, x30, x31    # SRA
    or    x14, x14, x15    # OR
    and   x16, x16, x17    # AND

# --- M extension (RV64M) ---
    mul     x5,  x5,  x6   # MUL
    mulh    x6,  x6,  x7   # MULH
    mulhsu  x7,  x7,  x8   # MULHSU
    mulhu   x8,  x8,  x9   # MULHU
    div     x9,  x9,  x10  # DIV
    divu    x10, x10, x11  # DIVU
    rem     x11, x11, x12  # REM
    remu    x12, x12, x13  # REMU

# --- RV64 W-ops ---
    addw   x5,  x5,  x6    # ADDW
    subw   x6,  x6,  x7    # SUBW
    sllw   x7,  x7,  x8    # SLLW
    srlw   x8,  x8,  x9    # SRLW
    sraw   x9,  x9,  x10   # SRAW
    mulw   x10, x10, x11   # MULW
    divw   x11, x11, x12   # DIVW
    divuw  x12, x12, x13   # DIVUW
    remw   x13, x13, x14   # REMW
    remuw  x14, x14, x15   # REMUW

# --- I-ops (RV64) ---
    addi  x5,  x5,  -1     # ADDI
    slli  x6,  x6,   1     # SLLI
    slti  x7,  x7,   0     # SLTI
    sltiu x8,  x8,   1     # SLTIU
    xori  x9,  x9,  0x7f   # XORI
    ori   x10, x10, 0x55   # ORI
    andi  x11, x11, -32    # ANDI
    srli  x12, x12,  3     # SRLI
    srai  x13, x13,  5     # SRAI
    jalr  x0,  0(x13)     # JALR

# --- Loads (RV64) ---
    lb   x5,   0(x13)      # LB
    lh   x6,   2(x13)      # LH
    lbu  x7,   4(x13)      # LBU
    lhu  x8,   6(x13)      # LHU
    lw   x9,   8(x13)      # LW
    lwu  x10, 12(x13)      # LWU
    ld   x11, 16(x13)      # LD

# --- Stores (RV64) ---
    sb   x5,   0(x13)      # SB
    sh   x6,   2(x13)      # SH
    sw   x7,   4(x13)      # SW
    sd   x8,   8(x13)      # SD

# --- Branches (all forms) ---
    beq   x5,  x6,  b_eq   # BEQ
    bne   x5,  x6,  b_ne   # BNE
    blt   x5,  x6,  b_lt   # BLT
    bge   x5,  x6,  b_ge   # BGE
    bltu  x5,  x6,  b_ltu  # BLTU
    bgeu  x5,  x6,  b_geu  # BGEU
    beqz  x5,      b_eqz   # BEQZ
    bnez  x6,      b_nez   # BNEZ

b_eq:   nop
b_ne:   nop
b_lt:   nop
b_ge:   nop
b_ltu:  nop
b_geu:  nop
b_eqz:  nop
b_nez:  nop

# --- Tiny loop + if/else using CB/CJ where possible ---
loop_top:
    addi x15, x15, -1      # compressible CI
    bnez x15, loop_top     # CB (BNEZ)

    beqz x14, then_blk     # CB (BEQZ)
    add   x14, x14, x15    # else path
    j     if_end           # CJ (J)
then_blk:
    sub   x14, x14, x15
if_end:
    ebreak

# --- Explicit compressed ops (exercise explicit-C path) ---
    c.nop
    c.li   x5,  1
    c.lui  x17, 1
    c.addi x6,  -1
    c.addiw x8, 1          # RV64C
    c.slli x7,  1
    c.srli x8,  1
    c.srai x9,  1
    c.andi x10, -1
    c.mv   x12, x13
    c.add  x14, x15
    c.sub  x8,  x9
    c.xor  x10, x11
    c.or   x12, x13
    c.and  x14, x15
    c.addw x8,  x8         # RV64C CAW, rd’=rs1’
    c.subw x9,  x9         # RV64C CAW, rd’=rs1’
    c.jr   x1
    c.jalr x1
    c.ebreak

# --- Explicit compressed loads/stores ---
    # prime regs: x8..x15
    c.beqz  x8, near_label
    c.bnez  x9, near_label
    c.lw   x8,  0(x9)
    c.sw   x10, 0(x11)
    c.ld   x8,  0(x9)      # RV64C
    c.sd   x10, 0(x11)     # RV64C
    # stack forms
    c.lwsp  x5,   0
    c.swsp  x6,   4
    c.ldsp  x7,   8    # RV64C
    c.sdsp  x8,  16    # RV64C
    c.j     near_label

# --- End control ---
    j end_prog
