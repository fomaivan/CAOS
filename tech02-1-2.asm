.global solve
.global _solve

// int solve(int64_t A, int B, int C, int D);
// x0 = A, x1 = B, x2 = C, x3 = D;
// x5      x6      x7      x8

.text

solve:
_solve:
    mov x4, #0
    mov x5, #256
    while_start:
        cmp x5, #0
        beq while_end

            mov x5, x0
            sxtw x6, w1
            sxtw x7, w2
            sxtw x8, w3

            madd x5, x5, x4, x6
            madd x5, x5, x4, x7
            madd x5, x5, x4, x8

            add x4, x4, #1
        b while_start
    while_end:
    sub x4, x4, #1
    sxtw x0, w4
    ret