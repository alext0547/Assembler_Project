# Test 06: Pseudo-instruction coverage (no base mnemonics)
# Pseudos used: nop, li, mv, neg, seqz, snez, subi, beqz, bnez, j, jr, la, ret

        .data
        .align 2
tbl:
        .word 0x11111111 
        .word 0x22222222
msg:
        .asciiz "ok"

        .text
main:
        nop                     # 1) nop

        li    x5, 0             # 2) li (small)
        beqz  x5, is_zero       # 3) beqz

        j     fail               # 4) j

is_zero:
        li    x6, 42            # li again (still small, safe)
        mv    x7, x6            # 5) mv
        neg   x8, x7            # 6) neg (x8 = -42)
        snez  x10, x8           # 7) snez (x10 = 1 since x8 != 0)
        seqz  x9, x0            # 8) seqz (x9 = 1 since x0 == 0)
        subi  x7, x7, 42        # 9) subi (x7 = 0)

        bnez  x7, fail          # 10) bnez (won't branch, x7==0)

        la    x11, tbl          # 11) la (AUIPC/ADDI pcrel reloc pair)
        jr    x11               # 12) jr (jump via register — assembles fine)

        ret                     # 13) ret (jalr x0,x1,0)

fail:
        j     fail              # j (again): infinite loop to keep “only pseudos”
