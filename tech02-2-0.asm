.global calculate
.global _calculate
.global R
.global _R

.text
calculate:
_calculate:
  ldr x0, A
  ldr x1, B
  ldr x2, C
  ldr x3, D

  mul x2, x2, x3
  madd x0, x0, x1, x2

  adr x1, R
  str x0, [x1]
  ret

.data
R:
_R: .zero 8