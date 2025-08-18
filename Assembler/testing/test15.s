# Test 15: Ensuring auto compressable instructions don't auto compress when flag isn't set
    .text
_start:
    # CI: addi rd, rd, imm (in range)
    addi x5,  x5,  -32
    addi x6,  x6,   31

    # CI via LI: addi rd, x0, imm (in range)
    addi x10, x0,    1

    # C.NOP pattern
    addi x0,  x0,    0

    # CI: slli rd, rd, sh
    slli x11, x11,   1

    # CI (prime regs): srli/srai rd, rd, sh
    srli x8,  x8,    3
    srai x9,  x9,    5

    # CI (prime regs): andi rd, rd, imm
    andi x10, x10,  -1

    # CR: mv/add patterns
    add  x12, x0,  x13      # mv rd,rs2
    add  x14, x14, x15      # add rd,rd,rs2

    # CR: jr/jalr patterns
    jalr x0,  0(x1)        # jr x1
    jalr x1,  0(x2)        # jalr x1, x2, 0

    # CR: ebreak pattern
    ebreak

    # CA: sub/xor/or/and on prime regs
    sub  x8,  x8,  x9
    xor  x10, x10, x11
    or   x12, x12, x13
    and  x14, x14, x15

    # CL/CS (prime regs): loads/stores with encodable offsets
    lw   x8,   0(x9)
    lw   x10, 124(x11)
    sw   x10,   0(x11)
    sw   x12, 124(x13)

    # CI/CSS SP forms
    lw   x5,    0(x2)       # lwsp
    sw   x6,  252(x2)       # swsp

    # CB/CJ: short branches/jumps (encodable if compression were enabled)
    beq  x8,  x0,  b1
    bne  x8,  x0,  b2
    jal  x0,  j1

    nop
b1: nop
b2: nop
j1: nop
