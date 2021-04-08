.syntax unified
.cpu cortex-m4
.fpu fpv4-sp-d16
.global vec32_2_12_mul
.type vec32_2_12_mul, %function
.align 2
vec32_2_12_mul:
push { r4-r12,r14 }
vpush { d8-d10 }
vmov  s0,r0
vmov  s1,r1
vmov  s2,r2
add  r2, #32
ldm r2, { r0,r3,r4,r5 }
ldm  r1!,{ r2,r6,r7,r8 }
@ c8
and  r9, r2, r0
vmov  s7,r9
@ c9
and  r10, r2, r3
and  r9, r6, r0
eor  r10, r9
vmov s8,r10
@ c10
and  r10, r2, r4
and  r9, r6, r3
eor  r10, r9
and  r9, r7, r0
eor  r10, r9
vmov s9,r10
@ c11
and  r10, r2, r5
and  r9, r6, r4
eor  r10, r9
and  r9, r7, r3
eor  r10, r9
and  r9, r8, r0
eor  r10, r9
vmov s10, r10
@ c12
and  r10, r6, r5
and  r9, r7, r4
eor  r10, r9
and  r9, r8, r3
eor  r10, r9
@ c13
and  r11, r7, r5
and  r9, r8, r4
eor  r11, r9
@ c14
and  r12, r8, r5
@ a4-a7
ldm  r1!,{ r2,r6,r7,r8 }
@ c12
and  r9, r2, r0
eor  r10, r9
vmov s11, r10
@ c13
and  r9, r2, r3
eor  r11, r9
and  r9, r6, r0
eor  r11, r9
vmov s12, r11
@ c14
and  r9, r2, r4
eor  r12, r9
and  r9, r6, r3
eor  r12, r9
and  r9, r7, r0
eor  r12, r9
vmov s13, r12
@ c15
and  r10, r2, r5
and  r9, r6, r4
eor  r10, r9
and  r9, r7, r3
eor  r10, r9
and  r9, r8, r0
eor  r10, r9
@ c16
and  r11, r6, r5
and  r9, r7, r4
eor  r11, r9
and  r9, r8, r3
eor  r11, r9
@ c17
and  r12, r7, r5
and  r9, r8, r4
eor  r12, r9
@ c18
and  r14, r8, r5
@ a8-a11
ldm  r1!,{ r2,r6,r7,r8 }
@ c22
and  r1, r8, r5
vmov s21, r1
@ c21
and  r1, r8, r4
and  r9, r7, r5
eor  r1, r9
vmov s20, r1
@ c20
and  r1, r8, r3
and  r9, r7, r4
eor  r1, r9
and  r9, r6, r5
eor  r1, r9
vmov s19, r1
@ c19
and  r1, r8, r0
and  r9, r7, r3
eor  r1, r9
and  r9, r6, r4
eor  r1, r9
and  r9, r2, r5
eor  r1, r9
vmov s18, r1
@ c18
and  r9, r7, r0
eor  r14, r9
and  r9, r6, r3
eor  r14, r9
and  r9, r2, r4
eor  r14, r9
@ c17
and  r9, r6, r0
eor  r12, r9
and  r9, r2, r3
eor  r12, r9
@ c16
and  r9, r2, r0
eor  r11, r9
vmov r0,s2
add  r0, #32
ldmdb  r0!,{ r1,r3,r4,r5 }
@ c18
and  r9, r8, r5
eor  r14, r9
vmov s17, r14
@ c17
and  r9, r7, r5
eor  r12, r9
and  r9, r8, r4
eor  r12, r9
vmov s16, r12
@ c16
and  r9, r6, r5
eor  r11, r9
and  r9, r7, r4
eor  r11, r9
and  r9, r8, r3
eor  r11, r9
vmov s15, r11
@ c15
and  r9, r2, r5
eor  r10, r9
and  r9, r6, r4
eor  r10, r9
and  r9, r7, r3
eor  r10, r9
and  r9, r8, r1
eor  r10, r9
vmov s14, r10
vmov  r12, s11
vmov  r11, s12
vmov  r10, s13
@ c14
and  r9, r2, r4
eor  r10, r9
and  r9, r6, r3
eor  r10, r9
and  r9, r7, r1
eor  r10, r9
@ c13
and  r9, r2, r3
eor  r11, r9
and  r9, r6, r1
eor  r11, r9
@ c12
and  r9, r2, r1
eor  r12, r9
ldmdb  r0,{ r1,r3,r4,r5 }
@ c14
and  r9, r8, r5
eor  r10, r9
vmov s13, r10
@ c13
and  r9, r7, r5
eor  r11, r9
and  r9, r8, r4
eor  r11, r9
vmov s12, r11
@ c12
and  r9, r6, r5
eor  r12, r9
and  r9, r7, r4
eor  r12, r9
and  r9, r8, r3
eor  r12, r9
vmov s11, r12
vmov  r0,s10
vmov  r10,s9
vmov  r11,s8
vmov  r12,s7
@ c11
and  r9, r2, r5
eor  r0, r9
and  r9, r6, r4
eor  r0, r9
and  r9, r7, r3
eor  r0, r9
and  r9, r8, r1
eor  r0, r9
@ c10
and  r9, r2, r4
eor  r10, r9
and  r9, r6, r3
eor  r10, r9
and  r9, r7, r1
eor  r10, r9
@ c9
and  r9, r2, r3
eor  r11, r9
and  r9, r6, r1
eor  r11, r9
@ c8
and  r9, r2, r1
eor  r12, r9
vmov  r14,s1
add  r14,#16
ldm  r14,{ r2,r6,r7,r8 }
@ c10
and  r9, r8, r5
eor  r10, r9
@ c9
and  r9, r7, r5
eor  r11, r9
and  r9, r8, r4
eor  r11, r9
@ c8
and  r9, r6, r5
eor  r12, r9
and  r9, r7, r4
eor  r12, r9
and  r9, r8, r3
eor  r12, r9
@ c7
and  r14, r2, r5
and  r9, r6, r4
eor  r14, r9
and  r9, r7, r3
eor  r14, r9
and  r9, r8, r1
eor  r14, r9
vmov s6, r14
@ c6
and  r14, r2, r4
and  r9, r6, r3
eor  r14, r9
and  r9, r7, r1
eor  r14, r9
vmov s5, r14
@ c5
and  r14, r2, r3
and  r9, r6, r1
eor  r14, r9
vmov s4, r14
@ c4
and  r14, r2, r1
vmov s3, r14
vmov  r14,s2
add  r14,#16
ldm  r14,{ r1,r3,r4,r5 }
@ c8
and  r9, r2, r1
eor  r12, r9
@ c9
and  r9, r2, r3
eor  r11, r9
and  r9, r6, r1
eor  r11, r9
@ c10
and  r9, r2, r4
eor  r10, r9
and  r9, r6, r3
eor  r10, r9
and  r9, r7, r1
eor  r10, r9
@ c11
and  r9, r2, r5
eor  r0, r9
and  r9, r6, r4
eor  r0, r9
and  r9, r7, r3
eor  r0, r9
and  r9, r8, r1
eor  r0, r9
vmov s10, r0
@ c12
vmov r0,s11
and  r9, r6, r5
eor  r0, r9
and  r9, r7, r4
eor  r0, r9
and  r9, r8, r3
eor  r0, r9
vmov s11, r0
@ c13
vmov r0, s12
and  r9, r7, r5
eor  r0, r9
and  r9, r8, r4
eor  r0, r9
vmov s12, r0
@ c14
vmov r0,s13
and  r9, r8, r5
eor  r0, r9
vmov s13, r0
vmov  r0,s1
ldm  r0,{ r2,r6,r7,r8 }
@ c10
and  r9, r8, r5
eor  r10, r9
vmov s9, r10
@ c9
and  r9, r7, r5
eor  r11, r9
and  r9, r8, r4
eor  r11, r9
vmov s8, r11
@ c8
and  r9, r6, r5
eor  r12, r9
and  r9, r7, r4
eor  r12, r9
and  r9, r8, r3
eor  r12, r9
vmov s7, r12
vmov  r0,s3
vmov  r10,s4
vmov  r11,s5
vmov  r12,s6
@ c7
and  r9, r2, r5
eor  r12, r9
and  r9, r6, r4
eor  r12, r9
and  r9, r7, r3
eor  r12, r9
and  r9, r8, r1
eor  r12, r9
@ c6
and  r9, r2, r4
eor  r11, r9
and  r9, r6, r3
eor  r11, r9
and  r9, r7, r1
eor  r11, r9
@ c5
and  r9, r2, r3
eor  r10, r9
and  r9, r6, r1
eor  r10, r9
@ c4
and  r9, r2, r1
eor  r0, r9
vmov  r14,s2
ldm  r14,{ r1,r3,r4,r5 }
@ c6
and  r9, r8, r5
eor  r11, r9
@ c5
and  r9, r7, r5
eor  r10, r9
and  r9, r8, r4
eor  r10, r9
@ c4
and  r9, r6, r5
eor  r0, r9
and  r9, r7, r4
eor  r0, r9
and  r9, r8, r3
eor  r0, r9
@ c3
and  r14, r2, r5
and  r9, r6, r4
eor  r14, r9
and  r9, r7, r3
eor  r14, r9
and  r9, r8, r1
eor  r14, r9
@ c2
and  r5, r2, r4
and  r9, r6, r3
eor  r5, r9
and  r9, r7, r1
eor  r5, r9
@ c1
and  r4, r2, r3
and  r9, r6, r1
eor  r4, r9
@ c0
and  r3, r2, r1
@  c0: r3
@  c1: r4
@  c2: r5
@  c3: r14
@  c4: r0
@  c5: r10
@  c6: r11
@  c7: r12
@ reduction
vmov  r1,s7
vmov  r2,s8
vmov  r6,s9
vmov  r7,s10
vmov  r9,s12
vmov  r8, s21
eor  r6, r8
eor  r9, r8
eor  r0, r9
eor  r4, r9
vmov  r9,s11
vmov  r8, s20
eor  r2, r8
eor  r9, r8
eor  r14, r9
eor  r3, r9
vmov  r8, s19
eor  r1, r8
eor  r7, r8
vmov  r8, s18
eor  r12, r8
eor  r6, r8
vmov  r8, s17
eor  r11, r8
eor  r2, r8
vmov  r8, s16
eor  r10, r8
eor  r1, r8
vmov  r8, s15
eor  r0, r8
eor  r12, r8
vmov  r8, s14
eor  r14, r8
eor  r11, r8
vmov  r8, s13
eor  r5, r8
eor  r10, r8
vmov  r8,s0
stm  r8!,{ r3,r4,r5,r14 }
stm  r8!,{ r0,r10,r11,r12 }
stm  r8!,{ r1,r2,r6,r7 }
vpop { d8-d10 }
pop { r4-r12,r14 }
bx lr
.size   vec32_2_12_mul, .-vec32_2_12_mul
.global vec32_2_12_accu_mul
.type vec32_2_12_accu_mul, %function
.align 2
vec32_2_12_accu_mul:
cmp  r3, #1
blt  .Lexitdot12
bgt  .Lstartdot122muls
push  { lr }
bl    vec32_2_12_mul
pop   { pc }
.Lstartdot122muls:
push { r4-r12,r14 }
vpush { d8-d12 }
vmov  s23,r0
add  r1,#16
add  r2,#16
vmov  s24,r1
vmov  s25,r2
subs r3,r3,#1
vmov s26,r3
vmov  r0,s25
add  r0, #16
ldm r0, { r1,r2,r3,r4 }
vmov  r5,s24
ldmdb  r5,{ r6,r7,r8,r9 }
@ c8
and  r10, r6, r1
vmov  s8,r10
@ c9
and  r10, r6, r2
and  r0, r7, r1
eor  r10, r0
vmov  s9,r10
@ c10
and  r10, r6, r3
and  r0, r7, r2
eor  r10, r0
and  r0, r8, r1
eor  r10, r0
vmov  s10,r10
@ c11
and  r10, r6, r4
and  r0, r7, r3
eor  r10, r0
and  r0, r8, r2
eor  r10, r0
and  r0, r9, r1
eor  r10, r0
vmov  s11,r10
@ c12
and  r10, r7, r4
and  r0, r8, r3
eor  r10, r0
and  r0, r9, r2
eor  r10, r0
@ c13
and  r11, r8, r4
and  r0, r9, r3
eor  r11, r0
@ c14
and  r12, r9, r4
@ a4-a7
ldm  r5!,{ r6,r7,r8,r9 }
@ c12
and  r0, r6, r1
eor  r10, r0
vmov  s12,r10
@ c13
and  r0, r6, r2
eor  r11, r0
and  r0, r7, r1
eor  r11, r0
vmov  s13,r11
@ c14
and  r0, r6, r3
eor  r12, r0
and  r0, r7, r2
eor  r12, r0
and  r0, r8, r1
eor  r12, r0
vmov  s14,r12
@ c15
and  r10, r6, r4
and  r0, r7, r3
eor  r10, r0
and  r0, r8, r2
eor  r10, r0
and  r0, r9, r1
eor  r10, r0
@ c16
and  r11, r7, r4
and  r0, r8, r3
eor  r11, r0
and  r0, r9, r2
eor  r11, r0
@ c17
and  r12, r8, r4
and  r0, r9, r3
eor  r12, r0
@ c18
and  r14, r9, r4
@ a8-a11
ldm  r5!,{ r6,r7,r8,r9 }
@ c22
and  r5, r9, r4
vmov  s22,r5
@ c21
and  r5, r9, r3
and  r0, r8, r4
eor  r5, r0
vmov  s21,r5
@ c20
and  r5, r9, r2
and  r0, r8, r3
eor  r5, r0
and  r0, r7, r4
eor  r5, r0
vmov  s20,r5
@ c19
and  r5, r9, r1
and  r0, r8, r2
eor  r5, r0
and  r0, r7, r3
eor  r5, r0
and  r0, r6, r4
eor  r5, r0
vmov  s19,r5
@ c18
and  r0, r8, r1
eor  r14, r0
and  r0, r7, r2
eor  r14, r0
and  r0, r6, r3
eor  r14, r0
@ c17
and  r0, r7, r1
eor  r12, r0
and  r0, r6, r2
eor  r12, r0
@ c16
and  r0, r6, r1
eor  r11, r0
vmov r1,s25
ldm  r1,{ r2,r3,r4,r5 }
@ c18
and  r0, r9, r5
eor  r14, r0
vmov  s18,r14
@ c17
and  r0, r8, r5
eor  r12, r0
and  r0, r9, r4
eor  r12, r0
vmov  s17,r12
@ c16
and  r0, r7, r5
eor  r11, r0
and  r0, r8, r4
eor  r11, r0
and  r0, r9, r3
eor  r11, r0
vmov  s16,r11
@ c15
and  r0, r6, r5
eor  r10, r0
and  r0, r7, r4
eor  r10, r0
and  r0, r8, r3
eor  r10, r0
and  r0, r9, r2
eor  r10, r0
vmov  s15,r10
vmov  r12, s12
vmov  r11, s13
vmov  r10, s14
@ c14
and  r0, r6, r4
eor  r10, r0
and  r0, r7, r3
eor  r10, r0
and  r0, r8, r2
eor  r10, r0
@ c13
and  r0, r6, r3
eor  r11, r0
and  r0, r7, r2
eor  r11, r0
@ c12
and  r0, r6, r2
eor  r12, r0
ldmdb  r1,{ r2,r3,r4,r5 }
@ c14
and  r0, r9, r5
eor  r10, r0
vmov  s14,r10
@ c13
and  r0, r8, r5
eor  r11, r0
and  r0, r9, r4
eor  r11, r0
vmov  s13,r11
@ c12
and  r0, r7, r5
eor  r12, r0
and  r0, r8, r4
eor  r12, r0
and  r0, r9, r3
eor  r12, r0
vmov  s12,r12
vmov  r1,s11
vmov  r10,s10
vmov  r11,s9
vmov  r12,s8
@ c11
and  r0, r6, r5
eor  r1, r0
and  r0, r7, r4
eor  r1, r0
and  r0, r8, r3
eor  r1, r0
and  r0, r9, r2
eor  r1, r0
@ c10
and  r0, r6, r4
eor  r10, r0
and  r0, r7, r3
eor  r10, r0
and  r0, r8, r2
eor  r10, r0
@ c9
and  r0, r6, r3
eor  r11, r0
and  r0, r7, r2
eor  r11, r0
@ c8
and  r0, r6, r2
eor  r12, r0
vmov  r0,s24
ldm  r0,{ r6,r7,r8,r9 }
@ c10
and  r0, r9, r5
eor  r10, r0
@ c9
and  r0, r8, r5
eor  r11, r0
and  r0, r9, r4
eor  r11, r0
@ c8
and  r0, r7, r5
eor  r12, r0
and  r0, r8, r4
eor  r12, r0
and  r0, r9, r3
eor  r12, r0
@ c7
and  r14, r6, r5
and  r0, r7, r4
eor  r14, r0
and  r0, r8, r3
eor  r14, r0
and  r0, r9, r2
eor  r14, r0
vmov  s7,r14
@ c6
and  r14, r6, r4
and  r0, r7, r3
eor  r14, r0
and  r0, r8, r2
eor  r14, r0
vmov  s6,r14
@ c5
and  r14, r6, r3
and  r0, r7, r2
eor  r14, r0
vmov  s5,r14
@ c4
and  r14, r6, r2
vmov  s4,r14
vmov  r0,s25
ldm  r0,{ r2,r3,r4,r5 }
@ c8
and  r0, r6, r2
eor  r12, r0
@ c9
and  r0, r6, r3
eor  r11, r0
and  r0, r7, r2
eor  r11, r0
@ c10
and  r0, r6, r4
eor  r10, r0
and  r0, r7, r3
eor  r10, r0
and  r0, r8, r2
eor  r10, r0
@ c11
and  r0, r6, r5
eor  r1, r0
and  r0, r7, r4
eor  r1, r0
and  r0, r8, r3
eor  r1, r0
and  r0, r9, r2
eor  r1, r0
vmov  s11,r1
@ c12
vmov r1,s12
and  r0, r7, r5
eor  r1, r0
and  r0, r8, r4
eor  r1, r0
and  r0, r9, r3
eor  r1, r0
vmov  s12,r1
@ c13
vmov r1, s13
and  r0, r8, r5
eor  r1, r0
and  r0, r9, r4
eor  r1, r0
vmov  s13,r1
@ c14
vmov r1,s14
and  r0, r9, r5
eor  r1, r0
vmov  s14,r1
vmov  r0,s24
ldmdb  r0,{ r1,r6,r7,r8 }
@ c10
and  r0, r8, r5
eor  r10, r0
vmov  s10,r10
@ c9
and  r0, r7, r5
eor  r11, r0
and  r0, r8, r4
eor  r11, r0
vmov  s9,r11
@ c8
and  r0, r6, r5
eor  r12, r0
and  r0, r7, r4
eor  r12, r0
and  r0, r8, r3
eor  r12, r0
vmov  s8,r12
vmov  r9,s4
vmov  r10,s5
vmov  r11,s6
vmov  r12,s7
@ c7
and  r0, r1, r5
eor  r12, r0
and  r0, r6, r4
eor  r12, r0
and  r0, r7, r3
eor  r12, r0
and  r0, r8, r2
eor  r12, r0
vmov  s7,r12
@ c6
and  r0, r1, r4
eor  r11, r0
and  r0, r6, r3
eor  r11, r0
and  r0, r7, r2
eor  r11, r0
@ c5
and  r0, r1, r3
eor  r10, r0
and  r0, r6, r2
eor  r10, r0
@ c4
and  r0, r1, r2
eor  r9, r0
vmov  r0,s25
ldmdb  r0,{ r2,r3,r4,r5 }
@ c6
and  r0, r8, r5
eor  r11, r0
vmov  s6,r11
@ c5
and  r0, r7, r5
eor  r10, r0
and  r0, r8, r4
eor  r10, r0
vmov  s5,r10
@ c4
and  r0, r6, r5
eor  r9, r0
and  r0, r7, r4
eor  r9, r0
and  r0, r8, r3
eor  r9, r0
vmov  s4,r9
@ c3
and  r9, r1, r5
and  r0, r6, r4
eor  r9, r0
and  r0, r7, r3
eor  r9, r0
and  r0, r8, r2
eor  r9, r0
vmov  s3,r9
@ c2
and  r9, r1, r4
and  r0, r6, r3
eor  r9, r0
and  r0, r7, r2
eor  r9, r0
vmov  s2,r9
@ c1
and  r9, r1, r3
and  r0, r6, r2
eor  r9, r0
vmov  s1,r9
@ c0
and  r9, r1, r2
vmov  s0,r9
.Lmul12:
vmov  r1,s24
vmov  r2,s25
add   r1,r1,#48
add   r2,r2,#48
vmov  s24,r1
vmov  s25,r2
vmov  r0,s25
add  r0, #16
ldm r0, { r1,r2,r3,r4 }
vmov  r5,s24
ldmdb  r5,{ r6,r7,r8,r9 }
@ c8
vmov  r10,s8
and  r0, r6, r1
eor  r10, r0
vmov  s8,r10
@ c9
vmov  r10,s9
and  r0, r6, r2
eor  r10, r0
and  r0, r7, r1
eor  r10, r0
vmov  s9,r10
@ c10
vmov  r10,s10
and  r0, r6, r3
eor  r10, r0
and  r0, r7, r2
eor  r10, r0
and  r0, r8, r1
eor  r10, r0
vmov  s10,r10
@ c11
vmov  r10,s11
and  r0, r6, r4
eor  r10, r0
and  r0, r7, r3
eor  r10, r0
and  r0, r8, r2
eor  r10, r0
and  r0, r9, r1
eor  r10, r0
vmov  s11,r10
@ c12
vmov  r10,s12
and  r0, r7, r4
eor  r10, r0
and  r0, r8, r3
eor  r10, r0
and  r0, r9, r2
eor  r10, r0
@ c13
vmov  r11,s13
and  r0, r8, r4
eor  r11, r0
and  r0, r9, r3
eor  r11, r0
@ c14
vmov  r12,s14
and  r0, r9, r4
eor  r12, r0
@ a4-a7
ldm  r5!,{ r6,r7,r8,r9 }
@ c12
and  r0, r6, r1
eor  r10, r0
vmov  s12,r10
@ c13
and  r0, r6, r2
eor  r11, r0
and  r0, r7, r1
eor  r11, r0
vmov  s13,r11
@ c14
and  r0, r6, r3
eor  r12, r0
and  r0, r7, r2
eor  r12, r0
and  r0, r8, r1
eor  r12, r0
vmov  s14,r12
@ c15
vmov  r10,s15
and  r0, r6, r4
eor  r10, r0
and  r0, r7, r3
eor  r10, r0
and  r0, r8, r2
eor  r10, r0
and  r0, r9, r1
eor  r10, r0
@ c16
vmov  r11,s16
and  r0, r7, r4
eor  r11, r0
and  r0, r8, r3
eor  r11, r0
and  r0, r9, r2
eor  r11, r0
@ c17
vmov  r12,s17
and  r0, r8, r4
eor  r12, r0
and  r0, r9, r3
eor  r12, r0
@ c18
vmov  r14,s18
and  r0, r9, r4
eor  r14, r0
@ a8-a11
ldm  r5!,{ r6,r7,r8,r9 }
@ c22
vmov  r5,s22
and  r0, r9, r4
eor  r5, r0
vmov  s22,r5
@ c21
vmov  r5,s21
and  r0, r9, r3
eor  r5, r0
and  r0, r8, r4
eor  r5, r0
vmov  s21,r5
@ c20
vmov  r5,s20
and  r0, r9, r2
eor  r5, r0
and  r0, r8, r3
eor  r5, r0
and  r0, r7, r4
eor  r5, r0
vmov  s20,r5
@ c19
vmov  r5,s19
and  r0, r9, r1
eor  r5, r0
and  r0, r8, r2
eor  r5, r0
and  r0, r7, r3
eor  r5, r0
and  r0, r6, r4
eor  r5, r0
vmov  s19,r5
@ c18
and  r0, r8, r1
eor  r14, r0
and  r0, r7, r2
eor  r14, r0
and  r0, r6, r3
eor  r14, r0
@ c17
and  r0, r7, r1
eor  r12, r0
and  r0, r6, r2
eor  r12, r0
@ c16
and  r0, r6, r1
eor  r11, r0
vmov r1,s25
ldm  r1,{ r2,r3,r4,r5 }
@ c18
and  r0, r9, r5
eor  r14, r0
vmov  s18,r14
@ c17
and  r0, r8, r5
eor  r12, r0
and  r0, r9, r4
eor  r12, r0
vmov  s17,r12
@ c16
and  r0, r7, r5
eor  r11, r0
and  r0, r8, r4
eor  r11, r0
and  r0, r9, r3
eor  r11, r0
vmov  s16,r11
@ c15
and  r0, r6, r5
eor  r10, r0
and  r0, r7, r4
eor  r10, r0
and  r0, r8, r3
eor  r10, r0
and  r0, r9, r2
eor  r10, r0
vmov  s15,r10
vmov  r12, s12
vmov  r11, s13
vmov  r10, s14
@ c14
and  r0, r6, r4
eor  r10, r0
and  r0, r7, r3
eor  r10, r0
and  r0, r8, r2
eor  r10, r0
@ c13
and  r0, r6, r3
eor  r11, r0
and  r0, r7, r2
eor  r11, r0
@ c12
and  r0, r6, r2
eor  r12, r0
ldmdb  r1,{ r2,r3,r4,r5 }
@ c14
and  r0, r9, r5
eor  r10, r0
vmov  s14,r10
@ c13
and  r0, r8, r5
eor  r11, r0
and  r0, r9, r4
eor  r11, r0
vmov  s13,r11
@ c12
and  r0, r7, r5
eor  r12, r0
and  r0, r8, r4
eor  r12, r0
and  r0, r9, r3
eor  r12, r0
vmov  s12,r12
vmov  r1,s11
vmov  r10,s10
vmov  r11,s9
vmov  r12,s8
@ c11
and  r0, r6, r5
eor  r1, r0
and  r0, r7, r4
eor  r1, r0
and  r0, r8, r3
eor  r1, r0
and  r0, r9, r2
eor  r1, r0
@ c10
and  r0, r6, r4
eor  r10, r0
and  r0, r7, r3
eor  r10, r0
and  r0, r8, r2
eor  r10, r0
@ c9
and  r0, r6, r3
eor  r11, r0
and  r0, r7, r2
eor  r11, r0
@ c8
and  r0, r6, r2
eor  r12, r0
vmov  r0,s24
ldm  r0,{ r6,r7,r8,r9 }
@ c10
and  r0, r9, r5
eor  r10, r0
@ c9
and  r0, r8, r5
eor  r11, r0
and  r0, r9, r4
eor  r11, r0
@ c8
and  r0, r7, r5
eor  r12, r0
and  r0, r8, r4
eor  r12, r0
and  r0, r9, r3
eor  r12, r0
@ c7
vmov  r14,s7
and  r0, r6, r5
eor  r14, r0
and  r0, r7, r4
eor  r14, r0
and  r0, r8, r3
eor  r14, r0
and  r0, r9, r2
eor  r14, r0
vmov  s7,r14
@ c6
vmov  r14,s6
and  r0, r6, r4
eor  r14, r0
and  r0, r7, r3
eor  r14, r0
and  r0, r8, r2
eor  r14, r0
vmov  s6,r14
@ c5
vmov  r14,s5
and  r0, r6, r3
eor  r14, r0
and  r0, r7, r2
eor  r14, r0
vmov  s5,r14
@ c4
vmov  r14,s4
and  r0, r6, r2
eor  r14, r0
vmov  s4,r14
vmov  r0,s25
ldm  r0,{ r2,r3,r4,r5 }
@ c8
and  r0, r6, r2
eor  r12, r0
@ c9
and  r0, r6, r3
eor  r11, r0
and  r0, r7, r2
eor  r11, r0
@ c10
and  r0, r6, r4
eor  r10, r0
and  r0, r7, r3
eor  r10, r0
and  r0, r8, r2
eor  r10, r0
@ c11
and  r0, r6, r5
eor  r1, r0
and  r0, r7, r4
eor  r1, r0
and  r0, r8, r3
eor  r1, r0
and  r0, r9, r2
eor  r1, r0
vmov  s11,r1
@ c12
vmov r1,s12
and  r0, r7, r5
eor  r1, r0
and  r0, r8, r4
eor  r1, r0
and  r0, r9, r3
eor  r1, r0
vmov  s12,r1
@ c13
vmov r1, s13
and  r0, r8, r5
eor  r1, r0
and  r0, r9, r4
eor  r1, r0
vmov  s13,r1
@ c14
vmov r1,s14
and  r0, r9, r5
eor  r1, r0
vmov  s14,r1
vmov  r0,s24
ldmdb  r0,{ r1,r6,r7,r8 }
@ c10
and  r0, r8, r5
eor  r10, r0
vmov  s10,r10
@ c9
and  r0, r7, r5
eor  r11, r0
and  r0, r8, r4
eor  r11, r0
vmov  s9,r11
@ c8
and  r0, r6, r5
eor  r12, r0
and  r0, r7, r4
eor  r12, r0
and  r0, r8, r3
eor  r12, r0
vmov  s8,r12
vmov  r9,s4
vmov  r10,s5
vmov  r11,s6
vmov  r12,s7
@ c7
and  r0, r1, r5
eor  r12, r0
and  r0, r6, r4
eor  r12, r0
and  r0, r7, r3
eor  r12, r0
and  r0, r8, r2
eor  r12, r0
vmov  s7,r12
@ c6
and  r0, r1, r4
eor  r11, r0
and  r0, r6, r3
eor  r11, r0
and  r0, r7, r2
eor  r11, r0
@ c5
and  r0, r1, r3
eor  r10, r0
and  r0, r6, r2
eor  r10, r0
@ c4
and  r0, r1, r2
eor  r9, r0
vmov  r0,s25
ldmdb  r0,{ r2,r3,r4,r5 }
@ c6
and  r0, r8, r5
eor  r11, r0
vmov  s6,r11
@ c5
and  r0, r7, r5
eor  r10, r0
and  r0, r8, r4
eor  r10, r0
vmov  s5,r10
@ c4
and  r0, r6, r5
eor  r9, r0
and  r0, r7, r4
eor  r9, r0
and  r0, r8, r3
eor  r9, r0
vmov  s4,r9
@ c3
vmov  r9,s3
and  r0, r1, r5
eor  r9, r0
and  r0, r6, r4
eor  r9, r0
and  r0, r7, r3
eor  r9, r0
and  r0, r8, r2
eor  r9, r0
vmov  s3,r9
@ c2
vmov  r9,s2
and  r0, r1, r4
eor  r9, r0
and  r0, r6, r3
eor  r9, r0
and  r0, r7, r2
eor  r9, r0
vmov  s2,r9
@ c1
vmov  r9,s1
and  r0, r1, r3
eor  r9, r0
and  r0, r6, r2
eor  r9, r0
vmov  s1,r9
@ c0
vmov  r9,s0
and  r0, r1, r2
eor  r9, r0
vmov  s0,r9
vmov r3,s26
subs r3,r3,#1
vmov s26,r3
bne  .Lmul12
.Lreduce12:
@ reduction
vmov  r0,s0
vmov  r1,s1
vmov  r2,s2
vmov  r3,s3
vmov  r4,s4
vmov  r5,s5
vmov  r6,s6
vmov  r7,s7
vmov  r8,s8
vmov  r9,s9
vmov  r10,s10
vmov  r11,s11
vmov  r14,s13
vmov  r12, s22
eor  r10, r12
eor  r14, r12
eor  r4, r14
eor  r1, r14
vmov  r14,s12
vmov  r12, s21
eor  r9, r12
eor  r14, r12
eor  r3, r14
eor  r0, r14
vmov  r12, s20
eor  r8, r12
eor  r11, r12
vmov  r12, s19
eor  r7, r12
eor  r10, r12
vmov  r12, s18
eor  r6, r12
eor  r9, r12
vmov  r12, s17
eor  r5, r12
eor  r8, r12
vmov  r12, s16
eor  r4, r12
eor  r7, r12
vmov  r12, s15
eor  r3, r12
eor  r6, r12
vmov  r12, s14
eor  r2, r12
eor  r5, r12
vmov  r12,s23
stm  r12,{ r0,r1,r2,r3,r4,r5,r6,r7,r8,r9,r10,r11 }
vpop { d8-d12 }
pop { r4-r12,r14 }
.Lexitdot12:
bx lr
.size   vec32_2_12_accu_mul, .-vec32_2_12_accu_mul
