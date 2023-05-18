// int sum(int x0, size_t N, int *X);
//           r0      r1        r2

.global sum
.global _sum

.text

sum:
_sum:
    add x1, x2, x1, lsl #2
    while_start:
        cmp x2, x1
        beq while_end
            ldrsw x3, [x2]
            add x0, x0, x3
            add x2, x2, #4
        b while_start
    while_end:
    ret