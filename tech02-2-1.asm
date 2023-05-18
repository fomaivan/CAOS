.global main

.text
main:
   // Save stack frame and ret address
   stp x29, x30, [sp, -16]! // sp <- sp - 16
   mov x29, sp
   // Allocate local variables
   sub sp, sp, 24

   // Scanf
   adr x0, SCANF_STR
   sub x1, x29, 4
   sub x2, x29, 8
   bl scanf

   // Calculate
   ldr w1, [x29, -4]
   ldr w2, [x29, -8]
   add w1, w2, w1

   // Printf
   adr x0, PRINTF_STR
   bl printf

   mov w0, 0
   mov sp, x29
   ldp x29, x30, [sp], 16
   ret


.data
PRINTF_STR:
   .string "%d\n"
SCANF_STR:
   .string "%d%d"