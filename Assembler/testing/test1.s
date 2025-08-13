# Test 01: RV32I coverage + data directives
#
# Uses: .data/.text/.align/.byte/.half/.word/.dword/.ascii/.asciiz/.space
# Instr: R (add..and), I (addi..andi, slli/srli/srai, loads, jalr),
#        S (sb/sh/sw), B (beq..bgeu), U (auipc/lui), J (jal), system (ecall/ebreak)

        .data
        .align 2              # align to 4 bytes
data_block:
        .byte   127
        .half   -1
        .word   0x12345678
        .dword  -42
        .ascii  "Hi"
        .asciiz "World"
        .space  8             # 8 zero bytes
        .align 3              # align to 8 bytes

        .text
main:
        # --- R-type ---
        add   x1,  x2,  x3
        sub   x4,  x5,  x6
        sll   x7,  x8,  x9
        slt   x10, x11, x12
        sltu  x13, x14, x15
        xor   x16, x17, x18
        srl   x19, x20, x21
        sra   x22, x23, x24
        or    x25, x26, x27
        and   x28, x29, x30

        # --- I-type (arith/logic/shift) ---
        addi  x1,  x1,  1
        slli  x2,  x2,  3
        srli  x3,  x3,  2
        srai  x4,  x4,  5
        slti  x5,  x6,  -1
        sltiu x6,  x7,  1
        xori  x7,  x7,  3
        ori   x8,  x8,  16
        andi  x9,  x9,  255

        # --- I-type (loads) ---
        lb    x10, 0(x0)
        lh    x11, 2(x0)
        lbu   x12, 4(x0)
        lhu   x13, 6(x0)
        lw    x14, 8(x0)

        # --- S-type (stores) ---
        sb    x1,  0(x0)
        sh    x2,  2(x0)
        sw    x3,  4(x0)

        # --- B-type (branches) ---
        beq   x1, x1, label_beq
        bne   x1, x0, label_bne
        blt   x0, x1, label_blt
        bge   x1, x0, label_bge
        bltu  x0, x1, label_bltu
        bgeu  x1, x0, label_bgeu

label_bne:
        # --- J/U/system ---
        jal   x1, label_jal           # link to x1
label_beq:
        auipc x2, 0                    # imm must be multiple of 4096; 0 is fine
        lui   x3,  0x10000             # also multiple of 4096; good for your checks
        jalr  x0, 0(x1)                # return via x1 (encodes as JALR rd=0, rs1=1, imm=0)

label_blt:
        ecall
label_bge:
        ebreak
label_bltu:
label_bgeu:
label_jal:
        jal   x0, done                 # unconditional jump (J) via JAL with rd=x0

done:
        addi  x0, x0, 0                # final NOP-ish (encodes, doesn’t change state)
