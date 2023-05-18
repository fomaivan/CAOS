.text
.global main
.global _main

main:
_main:
    stp x29, x30, [sp, -16]!
    mov x29, sp
    sub sp, sp, #16

    _while_start:
        bl getchar_unlocked
        cmp x0, -1
        beq _while_end

        cmp x0, 59
        bhi _while_start

        cmp x0, 47
        bls _while_start

        bl putchar_unlocked
        bl _while_start

        _while_end:
            mov x0, 0
            mov sp, x29
            ldp x29, x30, [sp], 16
    ret