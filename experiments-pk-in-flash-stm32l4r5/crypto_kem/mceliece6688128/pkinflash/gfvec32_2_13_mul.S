.syntax unified
.cpu cortex-m4
.fpu fpv4-sp-d16
.global vec32_2_13_mul
.type vec32_2_13_mul, %function
.align 2
vec32_2_13_mul:
push { r4-r12,r14 }
vpush { d8-d11 }
vmov  s20,r0
add  r1,#20
add  r2,#20
vmov  s21,r1
vmov  s22,r2
add  r2, #16
ldm  r2, { r0,r3,r4,r5 }
ldmdb  r1,{ r6,r7,r8,r9,r10 }
@ c9
and  r11, r6, r0
vmov  s4,r11
@ c10
and  r11, r6, r3
and  r2, r7, r0
eor  r11, r2
vmov  s5,r11
@ c11
and  r11, r6, r4
and  r2, r7, r3
eor  r11, r2
and  r2, r8, r0
eor  r11, r2
vmov  s6,r11
@ c12
and  r11, r6, r5
and  r2, r7, r4
eor  r11, r2
and  r2, r8, r3
eor  r11, r2
and  r2, r9, r0
eor  r11, r2
vmov  s7,r11
@ c13
and  r11, r7, r5
and  r2, r8, r4
eor  r11, r2
and  r2, r9, r3
eor  r11, r2
and  r2, r10, r0
eor  r11, r2
vmov  s8,r11
@ c14
and  r6, r8, r5
and  r2, r9, r4
eor  r6, r2
and  r2, r10, r3
eor  r6, r2
@ c15
and  r11, r9, r5
and  r2, r10, r4
eor  r11, r2
@ c16
and  r12, r10, r5
@ a5-a8
ldm  r1!,{ r7,r8,r9,r10 }
@ c14
and  r2, r7, r0
eor  r6, r2
vmov  s9,r6
@ c15
and  r2, r7, r3
eor  r11, r2
and  r2, r8, r0
eor  r11, r2
vmov  s10,r11
@ c16
and  r2, r7, r4
eor  r12, r2
and  r2, r8, r3
eor  r12, r2
and  r2, r9, r0
eor  r12, r2
vmov  s11,r12
@ c17
and  r6, r7, r5
and  r2, r8, r4
eor  r6, r2
and  r2, r9, r3
eor  r6, r2
and  r2, r10, r0
eor  r6, r2
@ c18
and  r11, r8, r5
and  r2, r9, r4
eor  r11, r2
and  r2, r10, r3
eor  r11, r2
@ c19
and  r12, r9, r5
and  r2, r10, r4
eor  r12, r2
@ c20
and  r14, r10, r5
@ a9-a12
ldm  r1!,{ r7,r8,r9,r10 }
@ c24
and  r1, r10, r5
vmov  s19,r1
@ c23
and  r1, r10, r4
and  r2, r9, r5
eor  r1, r2
vmov  s18,r1
@ c22
and  r1, r10, r3
and  r2, r9, r4
eor  r1, r2
and  r2, r8, r5
eor  r1, r2
vmov  s17,r1
@ c21
and  r1, r10, r0
and  r2, r9, r3
eor  r1, r2
and  r2, r8, r4
eor  r1, r2
and  r2, r7, r5
eor  r1, r2
vmov  s16,r1
@ c20
and  r2, r9, r0
eor  r14, r2
and  r2, r8, r3
eor  r14, r2
and  r2, r7, r4
eor  r14, r2
@ c19
and  r2, r8, r0
eor  r12, r2
and  r2, r7, r3
eor  r12, r2
@ c18
and  r2, r7, r0
eor  r11, r2
vmov r0,s22
ldm  r0,{ r1,r3,r4,r5 }
@ c20
and  r2, r10, r5
eor  r14, r2
vmov  s15,r14
@ c19
and  r2, r9, r5
eor  r12, r2
and  r2, r10, r4
eor  r12, r2
vmov  s14,r12
@ c18
and  r2, r8, r5
eor  r11, r2
and  r2, r9, r4
eor  r11, r2
and  r2, r10, r3
eor  r11, r2
vmov  s13,r11
@ c17
and  r2, r7, r5
eor  r6, r2
and  r2, r8, r4
eor  r6, r2
and  r2, r9, r3
eor  r6, r2
and  r2, r10, r1
eor  r6, r2
vmov  s12,r6
vmov  r6, s11
vmov  r11, s10
vmov  r12, s9
@ c16
and  r2, r7, r4
eor  r6, r2
and  r2, r8, r3
eor  r6, r2
and  r2, r9, r1
eor  r6, r2
@ c15
and  r2, r7, r3
eor  r11, r2
and  r2, r8, r1
eor  r11, r2
@ c14
and  r2, r7, r1
eor  r12, r2
ldmdb  r0,{ r1,r3,r4,r5,r14 }
@ c16
and  r2, r10, r14
eor  r6, r2
vmov  s11,r6
@ c15
and  r2, r9, r14
eor  r11, r2
and  r2, r10, r5
eor  r11, r2
vmov  s10,r11
@ c14
and  r2, r8, r14
eor  r12, r2
and  r2, r9, r5
eor  r12, r2
and  r2, r10, r4
eor  r12, r2
vmov  s9,r12
vmov  r0,s8
vmov  r6,s7
vmov  r11,s6
vmov  r12,s5
@ c13
and  r2, r7, r14
eor  r0, r2
and  r2, r8, r5
eor  r0, r2
and  r2, r9, r4
eor  r0, r2
and  r2, r10, r3
eor  r0, r2
vmov  s8,r0
@ c12
and  r2, r7, r5
eor  r6, r2
and  r2, r8, r4
eor  r6, r2
and  r2, r9, r3
eor  r6, r2
and  r2, r10, r1
eor  r6, r2
@ c11
and  r2, r7, r4
eor  r11, r2
and  r2, r8, r3
eor  r11, r2
and  r2, r9, r1
eor  r11, r2
@ c10
and  r2, r7, r3
eor  r12, r2
and  r2, r8, r1
eor  r12, r2
@ c9
vmov  r0,s4
and  r2, r7, r1
eor  r0, r2
vmov  r2,s21
ldm  r2,{ r7,r8,r9,r10 }
@ c11
and  r2, r10, r5
eor  r11, r2
@ c10
and  r2, r9, r5
eor  r12, r2
and  r2, r10, r4
eor  r12, r2
@ c9
and  r2, r8, r5
eor  r0, r2
and  r2, r9, r4
eor  r0, r2
and  r2, r10, r3
eor  r0, r2
@ c8
and  r14, r7, r5
and  r2, r8, r4
eor  r14, r2
and  r2, r9, r3
eor  r14, r2
and  r2, r10, r1
eor  r14, r2
vmov  s3,r14
@ c7
and  r14, r7, r4
and  r2, r8, r3
eor  r14, r2
and  r2, r9, r1
eor  r14, r2
vmov  s2,r14
@ c6
and  r14, r7, r3
and  r2, r8, r1
eor  r14, r2
vmov  s1,r14
@ c5
and  r14, r7, r1
vmov  s0,r14
vmov  r2,s22
sub  r2,#4
ldm  r2,{ r1,r3,r4,r5,r14 }
@ c9
and  r2, r7, r1
eor  r0, r2
@ c10
and  r2, r7, r3
eor  r12, r2
and  r2, r8, r1
eor  r12, r2
@ c11
and  r2, r7, r4
eor  r11, r2
and  r2, r8, r3
eor  r11, r2
and  r2, r9, r1
eor  r11, r2
@ c12
and  r2, r7, r5
eor  r6, r2
and  r2, r8, r4
eor  r6, r2
and  r2, r9, r3
eor  r6, r2
and  r2, r10, r1
eor  r6, r2
@ c13
vmov  r1,s8
and  r2, r7, r14
eor  r1, r2
and  r2, r8, r5
eor  r1, r2
and  r2, r9, r4
eor  r1, r2
and  r2, r10, r3
eor  r1, r2
vmov  s8,r1
@ c14
vmov r1,s9
and  r2, r8, r14
eor  r1, r2
and  r2, r9, r5
eor  r1, r2
and  r2, r10, r4
eor  r1, r2
vmov  s9,r1
@ c15
vmov r1, s10
and  r2, r9, r14
eor  r1, r2
and  r2, r10, r5
eor  r1, r2
vmov  s10,r1
@ c16
vmov r1,s11
and  r2, r10, r14
eor  r1, r2
vmov  s11,r1
vmov  r2,s21
ldmdb  r2,{ r1,r7,r8,r9,r10 }
@ c12
and  r2, r10, r14
eor  r6, r2
vmov  s7,r6
@ c11
and  r2, r9, r14
eor  r11, r2
and  r2, r10, r5
eor  r11, r2
vmov  s6,r11
@ c10
and  r2, r8, r14
eor  r12, r2
and  r2, r9, r5
eor  r12, r2
and  r2, r10, r4
eor  r12, r2
vmov  s5,r12
@ c9
and  r2, r7, r14
eor  r0, r2
and  r2, r8, r5
eor  r0, r2
and  r2, r9, r4
eor  r0, r2
and  r2, r10, r3
eor  r0, r2
vmov  s4,r0
vmov  r0,s0
vmov  r6,s1
vmov  r11,s2
vmov  r12,s3
@ c8
and  r2, r1, r14
eor  r12, r2
and  r2, r7, r5
eor  r12, r2
and  r2, r8, r4
eor  r12, r2
and  r2, r9, r3
eor  r12, r2
@ c7
and  r2, r1, r5
eor  r11, r2
and  r2, r7, r4
eor  r11, r2
and  r2, r8, r3
eor  r11, r2
@ c6
and  r2, r1, r4
eor  r6, r2
and  r2, r7, r3
eor  r6, r2
@ c5
and  r2, r1, r3
eor  r0, r2
vmov  r5,s22
ldmdb  r5!,{ r3,r4 }
@ c8
and  r2, r10, r4
eor  r12, r2
vmov  s3,r12
@ c7
and  r2, r9, r4
eor  r11, r2
and  r2, r10, r3
eor  r11, r2
vmov  s2,r11
@ c6
and  r2, r8, r4
eor  r6, r2
and  r2, r9, r3
eor  r6, r2
@ c5
and  r2, r7, r4
eor  r0, r2
and  r2, r8, r3
eor  r0, r2
@ c4
and  r12,r1,r4
and  r2, r7, r3
eor  r12, r2
@ c3
and  r11,r1,r3
ldmdb  r5!,{ r3,r4,r14 }
@ c6
and  r2, r10, r14
eor  r6, r2
vmov  s1,r6
@ c5
and  r2, r10, r4
eor  r0, r2
and  r2, r9, r14
eor  r0, r2
vmov  s0,r0
@ c4
and  r2, r10, r3
eor  r12, r2
and  r2, r9, r4
eor  r12, r2
and  r2, r8, r14
eor  r12, r2
@ c3
and  r2, r9, r3
eor  r11, r2
and  r2, r8, r4
eor  r11, r2
and  r2, r7, r14
eor  r11, r2
@ c2
and  r6,r1,r14
and  r2, r7, r4
eor  r6, r2
and  r2, r8, r3
eor  r6, r2
@ c1
and  r5,r1,r4
and  r2, r7, r3
eor  r5, r2
@ c0
and  r0,r1,r3
@ reduction
vmov  r1,s8
vmov  r2,s9
vmov  r3,s10
vmov  r4,s3
vmov  r7,s4
vmov  r8,s5
vmov  r9,s6
vmov  r10,s7
vmov  r14, s19
eor  r9, r14
eor  r10, r14
eor  r2, r14
eor  r3, r14
vmov  r14, s18
eor  r8, r14
eor  r9, r14
eor  r1, r14
eor  r2, r14
vmov  r14, s17
eor  r7, r14
eor  r8, r14
eor  r10, r14
eor  r1, r14
vmov  r14, s16
eor  r4, r14
eor  r7, r14
eor  r9, r14
eor  r10, r14
eor  r0, r1
eor  r5, r1
eor  r11, r1
eor  r12, r1
vmov  r1,s0
eor  r5, r2
eor  r6, r2
eor  r12, r2
eor  r1, r2
vmov  r2,s1
eor  r6, r3
eor  r11, r3
eor  r1, r3
eor  r2, r3
vmov  r3,s2
vmov  r14, s15
eor  r3, r14
eor  r4, r14
eor  r8, r14
eor  r9, r14
vmov  r14, s14
eor  r2, r14
eor  r3, r14
eor  r7, r14
eor  r8, r14
vmov  r14, s13
eor  r1, r14
eor  r2, r14
eor  r4, r14
eor  r7, r14
vmov  r14, s12
eor  r12, r14
eor  r1, r14
eor  r3, r14
eor  r4, r14
vmov  r14, s11
eor  r11, r14
eor  r12, r14
eor  r2, r14
eor  r3, r14
vmov  r14,s20
stm  r14!,{ r0,r5,r6 }
stm  r14!,{ r11,r12 }
stm  r14,{ r1,r2,r3,r4,r7,r8,r9,r10 }
vpop { d8-d11 }
pop { r4-r12,r14 }
bx lr
.size   vec32_2_13_mul, .-vec32_2_13_mul
.global vec32_2_13_accu_mul
.type vec32_2_13_accu_mul, %function
.align 2
vec32_2_13_accu_mul:
cmp  r3, #1
blt  .Lexitdot13
bgt  .Lstart2muls
push  { lr }
bl    vec32_2_13_mul
pop   { pc }
.Lstart2muls:
push { r4-r12,r14 }
vpush { d8-d15 }
vmov  s25,r0
add  r1,#20
add  r2,#20
vmov  s26,r1
vmov  s27,r2
subs  r3,r3,#1
vmov  s28,r3
vmov  r0,s27
add  r0, #16
ldm r0, { r1,r2,r3,r4 }
vmov  r5,s26
ldmdb  r5,{ r6,r7,r8,r9,r10 }
@ c9
and  r11, r6, r1
vmov  s9,r11
@ c10
and  r11, r6, r2
and  r0, r7, r1
eor  r11, r0
vmov  s10,r11
@ c11
and  r11, r6, r3
and  r0, r7, r2
eor  r11, r0
and  r0, r8, r1
eor  r11, r0
vmov  s11,r11
@ c12
and  r11, r6, r4
and  r0, r7, r3
eor  r11, r0
and  r0, r8, r2
eor  r11, r0
and  r0, r9, r1
eor  r11, r0
vmov  s12,r11
@ c13
and  r11, r7, r4
and  r0, r8, r3
eor  r11, r0
and  r0, r9, r2
eor  r11, r0
and  r0, r10, r1
eor  r11, r0
vmov  s13,r11
@ c14
and  r6, r8, r4
and  r0, r9, r3
eor  r6, r0
and  r0, r10, r2
eor  r6, r0
@ c15
and  r11, r9, r4
and  r0, r10, r3
eor  r11, r0
@ c16
and  r12, r10, r4
@ a5-a8
ldm  r5!,{ r7,r8,r9,r10 }
@ c14
and  r0, r7, r1
eor  r6, r0
vmov  s14,r6
@ c15
and  r0, r7, r2
eor  r11, r0
and  r0, r8, r1
eor  r11, r0
vmov  s15,r11
@ c16
and  r0, r7, r3
eor  r12, r0
and  r0, r8, r2
eor  r12, r0
and  r0, r9, r1
eor  r12, r0
vmov  s16,r12
@ c17
and  r6, r7, r4
and  r0, r8, r3
eor  r6, r0
and  r0, r9, r2
eor  r6, r0
and  r0, r10, r1
eor  r6, r0
@ c18
and  r11, r8, r4
and  r0, r9, r3
eor  r11, r0
and  r0, r10, r2
eor  r11, r0
@ c19
and  r12, r9, r4
and  r0, r10, r3
eor  r12, r0
@ c20
and  r14, r10, r4
@ a9-a12
ldm  r5!,{ r7,r8,r9,r10 }
@ c24
and  r5, r10, r4
vmov  s24,r5
@ c23
and  r5, r10, r3
and  r0, r9, r4
eor  r5, r0
vmov  s23,r5
@ c22
and  r5, r10, r2
and  r0, r9, r3
eor  r5, r0
and  r0, r8, r4
eor  r5, r0
vmov  s22,r5
@ c21
and  r5, r10, r1
and  r0, r9, r2
eor  r5, r0
and  r0, r8, r3
eor  r5, r0
and  r0, r7, r4
eor  r5, r0
vmov  s21,r5
@ c20
and  r0, r9, r1
eor  r14, r0
and  r0, r8, r2
eor  r14, r0
and  r0, r7, r3
eor  r14, r0
@ c19
and  r0, r8, r1
eor  r12, r0
and  r0, r7, r2
eor  r12, r0
@ c18
and  r0, r7, r1
eor  r11, r0
vmov r1,s27
ldm  r1,{ r2,r3,r4,r5 }
@ c20
and  r0, r10, r5
eor  r14, r0
vmov  s20,r14
@ c19
and  r0, r9, r5
eor  r12, r0
and  r0, r10, r4
eor  r12, r0
vmov  s19,r12
@ c18
and  r0, r8, r5
eor  r11, r0
and  r0, r9, r4
eor  r11, r0
and  r0, r10, r3
eor  r11, r0
vmov  s18,r11
@ c17
and  r0, r7, r5
eor  r6, r0
and  r0, r8, r4
eor  r6, r0
and  r0, r9, r3
eor  r6, r0
and  r0, r10, r2
eor  r6, r0
vmov  s17,r6
vmov  r6, s16
vmov  r11, s15
vmov  r12, s14
@ c16
and  r0, r7, r4
eor  r6, r0
and  r0, r8, r3
eor  r6, r0
and  r0, r9, r2
eor  r6, r0
@ c15
and  r0, r7, r3
eor  r11, r0
and  r0, r8, r2
eor  r11, r0
@ c14
and  r0, r7, r2
eor  r12, r0
ldmdb  r1,{ r2,r3,r4,r5,r14 }
@ c16
and  r0, r10, r14
eor  r6, r0
vmov  s16,r6
@ c15
and  r0, r9, r14
eor  r11, r0
and  r0, r10, r5
eor  r11, r0
vmov  s15,r11
@ c14
and  r0, r8, r14
eor  r12, r0
and  r0, r9, r5
eor  r12, r0
and  r0, r10, r4
eor  r12, r0
vmov  s14,r12
vmov  r1,s13
vmov  r6,s12
vmov  r11,s11
vmov  r12,s10
@ c13
and  r0, r7, r14
eor  r1, r0
and  r0, r8, r5
eor  r1, r0
and  r0, r9, r4
eor  r1, r0
and  r0, r10, r3
eor  r1, r0
vmov  s13,r1
@ c12
and  r0, r7, r5
eor  r6, r0
and  r0, r8, r4
eor  r6, r0
and  r0, r9, r3
eor  r6, r0
and  r0, r10, r2
eor  r6, r0
@ c11
and  r0, r7, r4
eor  r11, r0
and  r0, r8, r3
eor  r11, r0
and  r0, r9, r2
eor  r11, r0
@ c10
and  r0, r7, r3
eor  r12, r0
and  r0, r8, r2
eor  r12, r0
@ c9
vmov  r1,s9
and  r0, r7, r2
eor  r1, r0
vmov  r0,s26
ldm  r0,{ r7,r8,r9,r10 }
@ c11
and  r0, r10, r5
eor  r11, r0
@ c10
and  r0, r9, r5
eor  r12, r0
and  r0, r10, r4
eor  r12, r0
@ c9
and  r0, r8, r5
eor  r1, r0
and  r0, r9, r4
eor  r1, r0
and  r0, r10, r3
eor  r1, r0
@ c8
and  r14, r7, r5
and  r0, r8, r4
eor  r14, r0
and  r0, r9, r3
eor  r14, r0
and  r0, r10, r2
eor  r14, r0
vmov  s8,r14
@ c7
and  r14, r7, r4
and  r0, r8, r3
eor  r14, r0
and  r0, r9, r2
eor  r14, r0
vmov  s7,r14
@ c6
and  r14, r7, r3
and  r0, r8, r2
eor  r14, r0
vmov  s6,r14
@ c5
and  r14, r7, r2
vmov  s5,r14
vmov  r0,s27
sub  r0,#4
ldm  r0,{ r2,r3,r4,r5,r14 }
@ c9
and  r0, r7, r2
eor  r1, r0
@ c10
and  r0, r7, r3
eor  r12, r0
and  r0, r8, r2
eor  r12, r0
@ c11
and  r0, r7, r4
eor  r11, r0
and  r0, r8, r3
eor  r11, r0
and  r0, r9, r2
eor  r11, r0
@ c12
and  r0, r7, r5
eor  r6, r0
and  r0, r8, r4
eor  r6, r0
and  r0, r9, r3
eor  r6, r0
and  r0, r10, r2
eor  r6, r0
@ c13
vmov  r2,s13
and  r0, r7, r14
eor  r2, r0
and  r0, r8, r5
eor  r2, r0
and  r0, r9, r4
eor  r2, r0
and  r0, r10, r3
eor  r2, r0
vmov  s13,r2
@ c14
vmov r2,s14
and  r0, r8, r14
eor  r2, r0
and  r0, r9, r5
eor  r2, r0
and  r0, r10, r4
eor  r2, r0
vmov  s14,r2
@ c15
vmov r2, s15
and  r0, r9, r14
eor  r2, r0
and  r0, r10, r5
eor  r2, r0
vmov  s15,r2
@ c16
vmov r2,s16
and  r0, r10, r14
eor  r2, r0
vmov  s16,r2
vmov  r0,s26
ldmdb  r0,{ r2,r7,r8,r9,r10 }
@ c12
and  r0, r10, r14
eor  r6, r0
vmov  s12,r6
@ c11
and  r0, r9, r14
eor  r11, r0
and  r0, r10, r5
eor  r11, r0
vmov  s11,r11
@ c10
and  r0, r8, r14
eor  r12, r0
and  r0, r9, r5
eor  r12, r0
and  r0, r10, r4
eor  r12, r0
vmov  s10,r12
@ c9
and  r0, r7, r14
eor  r1, r0
and  r0, r8, r5
eor  r1, r0
and  r0, r9, r4
eor  r1, r0
and  r0, r10, r3
eor  r1, r0
vmov  s9,r1
vmov  r1,s5
vmov  r6,s6
vmov  r11,s7
vmov  r12,s8
@ c8
and  r0, r2, r14
eor  r12, r0
and  r0, r7, r5
eor  r12, r0
and  r0, r8, r4
eor  r12, r0
and  r0, r9, r3
eor  r12, r0
@ c7
and  r0, r2, r5
eor  r11, r0
and  r0, r7, r4
eor  r11, r0
and  r0, r8, r3
eor  r11, r0
@ c6
and  r0, r2, r4
eor  r6, r0
and  r0, r7, r3
eor  r6, r0
@ c5
and  r0, r2, r3
eor  r1, r0
vmov  r5,s27
ldmdb  r5!,{ r3,r4 }
@ c8
and  r0, r10, r4
eor  r12, r0
vmov  s8,r12
@ c7
and  r0, r9, r4
eor  r11, r0
and  r0, r10, r3
eor  r11, r0
vmov  s7,r11
@ c6
and  r0, r8, r4
eor  r6, r0
and  r0, r9, r3
eor  r6, r0
@ c5
and  r0, r7, r4
eor  r1, r0
and  r0, r8, r3
eor  r1, r0
@ c4
and  r11, r2, r4
and  r0, r7, r3
eor  r11, r0
@ c3
and  r12, r2, r3
ldmdb  r5!,{ r3,r4,r14 }
@ c6
and  r0, r10, r14
eor  r6, r0
vmov  s6,r6
@ c5
and  r0, r10, r4
eor  r1, r0
and  r0, r9, r14
eor  r1, r0
vmov  s5,r1
@ c4
and  r0, r10, r3
eor  r11, r0
and  r0, r9, r4
eor  r11, r0
and  r0, r8, r14
eor  r11, r0
vmov  s4,r11
@ c3
and  r0, r9, r3
eor  r12, r0
and  r0, r8, r4
eor  r12, r0
and  r0, r7, r14
eor  r12, r0
vmov  s3,r12
@ c2
and  r1, r2, r14
and  r0, r7, r4
eor  r1, r0
and  r0, r8, r3
eor  r1, r0
vmov  s2,r1
@ c1
and  r1, r2, r4
and  r0, r7, r3
eor  r1, r0
vmov  s1,r1
@ c0
and  r1, r2, r3
vmov  s0,r1
.Lmul13:
vmov  r1,s26
vmov  r2,s27
add   r1,#52
add   r2,#52
vmov  s26,r1
vmov  s27,r2
vmov  r0,s27
add  r0, #16
ldm r0, { r1,r2,r3,r4 }
vmov  r5,s26
ldmdb  r5,{ r6,r7,r8,r9,r10 }
@ c9
vmov  r11,s9
and  r0, r6, r1
eor  r11, r0
vmov  s9,r11
@ c10
vmov  r11,s10
and  r0, r6, r2
eor  r11, r0
and  r0, r7, r1
eor  r11, r0
vmov  s10,r11
@ c11
vmov  r11,s11
and  r0, r6, r3
eor  r11, r0
and  r0, r7, r2
eor  r11, r0
and  r0, r8, r1
eor  r11, r0
vmov  s11,r11
@ c12
vmov  r11,s12
and  r0, r6, r4
eor  r11, r0
and  r0, r7, r3
eor  r11, r0
and  r0, r8, r2
eor  r11, r0
and  r0, r9, r1
eor  r11, r0
vmov  s12,r11
@ c13
vmov  r11,s13
and  r0, r7, r4
eor  r11, r0
and  r0, r8, r3
eor  r11, r0
and  r0, r9, r2
eor  r11, r0
and  r0, r10, r1
eor  r11, r0
vmov  s13,r11
@ c14
vmov  r6,s14
and  r0, r8, r4
eor  r6, r0
and  r0, r9, r3
eor  r6, r0
and  r0, r10, r2
eor  r6, r0
@ c15
vmov  r11,s15
and  r0, r9, r4
eor  r11, r0
and  r0, r10, r3
eor  r11, r0
@ c16
vmov  r12,s16
and  r0, r10, r4
eor  r12, r0
@ a5-a8
ldm  r5!,{ r7,r8,r9,r10 }
@ c14
and  r0, r7, r1
eor  r6, r0
vmov  s14,r6
@ c15
and  r0, r7, r2
eor  r11, r0
and  r0, r8, r1
eor  r11, r0
vmov  s15,r11
@ c16
and  r0, r7, r3
eor  r12, r0
and  r0, r8, r2
eor  r12, r0
and  r0, r9, r1
eor  r12, r0
vmov  s16,r12
@ c17
vmov  r6,s17
and  r0, r7, r4
eor  r6, r0
and  r0, r8, r3
eor  r6, r0
and  r0, r9, r2
eor  r6, r0
and  r0, r10, r1
eor  r6, r0
@ c18
vmov  r11,s18
and  r0, r8, r4
eor  r11, r0
and  r0, r9, r3
eor  r11, r0
and  r0, r10, r2
eor  r11, r0
@ c19
vmov  r12,s19
and  r0, r9, r4
eor  r12, r0
and  r0, r10, r3
eor  r12, r0
@ c20
vmov  r14,s20
and  r0, r10, r4
eor  r14, r0
@ a9-a12
ldm  r5!,{ r7,r8,r9,r10 }
@ c24
vmov  r5,s24
and  r0, r10, r4
eor  r5, r0
vmov  s24,r5
@ c23
vmov  r5,s23
and  r0, r10, r3
eor  r5, r0
and  r0, r9, r4
eor  r5, r0
vmov  s23,r5
@ c22
vmov  r5,s22
and  r0, r10, r2
eor  r5, r0
and  r0, r9, r3
eor  r5, r0
and  r0, r8, r4
eor  r5, r0
vmov  s22,r5
@ c21
vmov  r5,s21
and  r0, r10, r1
eor  r5, r0
and  r0, r9, r2
eor  r5, r0
and  r0, r8, r3
eor  r5, r0
and  r0, r7, r4
eor  r5, r0
vmov  s21,r5
@ c20
and  r0, r9, r1
eor  r14, r0
and  r0, r8, r2
eor  r14, r0
and  r0, r7, r3
eor  r14, r0
@ c19
and  r0, r8, r1
eor  r12, r0
and  r0, r7, r2
eor  r12, r0
@ c18
and  r0, r7, r1
eor  r11, r0
vmov r1,s27
ldm  r1,{ r2,r3,r4,r5 }
@ c20
and  r0, r10, r5
eor  r14, r0
vmov  s20,r14
@ c19
and  r0, r9, r5
eor  r12, r0
and  r0, r10, r4
eor  r12, r0
vmov  s19,r12
@ c18
and  r0, r8, r5
eor  r11, r0
and  r0, r9, r4
eor  r11, r0
and  r0, r10, r3
eor  r11, r0
vmov  s18,r11
@ c17
and  r0, r7, r5
eor  r6, r0
and  r0, r8, r4
eor  r6, r0
and  r0, r9, r3
eor  r6, r0
and  r0, r10, r2
eor  r6, r0
vmov  s17,r6
vmov  r6, s16
vmov  r11, s15
vmov  r12, s14
@ c16
and  r0, r7, r4
eor  r6, r0
and  r0, r8, r3
eor  r6, r0
and  r0, r9, r2
eor  r6, r0
@ c15
and  r0, r7, r3
eor  r11, r0
and  r0, r8, r2
eor  r11, r0
@ c14
and  r0, r7, r2
eor  r12, r0
ldmdb  r1,{ r2,r3,r4,r5,r14 }
@ c16
and  r0, r10, r14
eor  r6, r0
vmov  s16,r6
@ c15
and  r0, r9, r14
eor  r11, r0
and  r0, r10, r5
eor  r11, r0
vmov  s15,r11
@ c14
and  r0, r8, r14
eor  r12, r0
and  r0, r9, r5
eor  r12, r0
and  r0, r10, r4
eor  r12, r0
vmov  s14,r12
vmov  r1,s13
vmov  r6,s12
vmov  r11,s11
vmov  r12,s10
@ c13
and  r0, r7, r14
eor  r1, r0
and  r0, r8, r5
eor  r1, r0
and  r0, r9, r4
eor  r1, r0
and  r0, r10, r3
eor  r1, r0
vmov  s13,r1
@ c12
and  r0, r7, r5
eor  r6, r0
and  r0, r8, r4
eor  r6, r0
and  r0, r9, r3
eor  r6, r0
and  r0, r10, r2
eor  r6, r0
@ c11
and  r0, r7, r4
eor  r11, r0
and  r0, r8, r3
eor  r11, r0
and  r0, r9, r2
eor  r11, r0
@ c10
and  r0, r7, r3
eor  r12, r0
and  r0, r8, r2
eor  r12, r0
@ c9
vmov  r1,s9
and  r0, r7, r2
eor  r1, r0
vmov  r0,s26
ldm  r0,{ r7,r8,r9,r10 }
@ c11
and  r0, r10, r5
eor  r11, r0
@ c10
and  r0, r9, r5
eor  r12, r0
and  r0, r10, r4
eor  r12, r0
@ c9
and  r0, r8, r5
eor  r1, r0
and  r0, r9, r4
eor  r1, r0
and  r0, r10, r3
eor  r1, r0
@ c8
vmov  r14,s8
and  r0, r7, r5
eor  r14, r0
and  r0, r8, r4
eor  r14, r0
and  r0, r9, r3
eor  r14, r0
and  r0, r10, r2
eor  r14, r0
vmov  s8,r14
@ c7
vmov  r14,s7
and  r0, r7, r4
eor  r14, r0
and  r0, r8, r3
eor  r14, r0
and  r0, r9, r2
eor  r14, r0
vmov  s7,r14
@ c6
vmov  r14,s6
and  r0, r7, r3
eor  r14, r0
and  r0, r8, r2
eor  r14, r0
vmov  s6,r14
@ c5
vmov  r14,s5
and  r0, r7, r2
eor  r14, r0
vmov  s5,r14
vmov  r0,s27
sub  r0,#4
ldm  r0,{ r2,r3,r4,r5,r14 }
@ c9
and  r0, r7, r2
eor  r1, r0
@ c10
and  r0, r7, r3
eor  r12, r0
and  r0, r8, r2
eor  r12, r0
@ c11
and  r0, r7, r4
eor  r11, r0
and  r0, r8, r3
eor  r11, r0
and  r0, r9, r2
eor  r11, r0
@ c12
and  r0, r7, r5
eor  r6, r0
and  r0, r8, r4
eor  r6, r0
and  r0, r9, r3
eor  r6, r0
and  r0, r10, r2
eor  r6, r0
@ c13
vmov  r2,s13
and  r0, r7, r14
eor  r2, r0
and  r0, r8, r5
eor  r2, r0
and  r0, r9, r4
eor  r2, r0
and  r0, r10, r3
eor  r2, r0
vmov  s13,r2
@ c14
vmov r2,s14
and  r0, r8, r14
eor  r2, r0
and  r0, r9, r5
eor  r2, r0
and  r0, r10, r4
eor  r2, r0
vmov  s14,r2
@ c15
vmov r2, s15
and  r0, r9, r14
eor  r2, r0
and  r0, r10, r5
eor  r2, r0
vmov  s15,r2
@ c16
vmov r2,s16
and  r0, r10, r14
eor  r2, r0
vmov  s16,r2
vmov  r0,s26
ldmdb  r0,{ r2,r7,r8,r9,r10 }
@ c12
and  r0, r10, r14
eor  r6, r0
vmov  s12,r6
@ c11
and  r0, r9, r14
eor  r11, r0
and  r0, r10, r5
eor  r11, r0
vmov  s11,r11
@ c10
and  r0, r8, r14
eor  r12, r0
and  r0, r9, r5
eor  r12, r0
and  r0, r10, r4
eor  r12, r0
vmov  s10,r12
@ c9
and  r0, r7, r14
eor  r1, r0
and  r0, r8, r5
eor  r1, r0
and  r0, r9, r4
eor  r1, r0
and  r0, r10, r3
eor  r1, r0
vmov  s9,r1
vmov  r1,s5
vmov  r6,s6
vmov  r11,s7
vmov  r12,s8
@ c8
and  r0, r2, r14
eor  r12, r0
and  r0, r7, r5
eor  r12, r0
and  r0, r8, r4
eor  r12, r0
and  r0, r9, r3
eor  r12, r0
@ c7
and  r0, r2, r5
eor  r11, r0
and  r0, r7, r4
eor  r11, r0
and  r0, r8, r3
eor  r11, r0
@ c6
and  r0, r2, r4
eor  r6, r0
and  r0, r7, r3
eor  r6, r0
@ c5
and  r0, r2, r3
eor  r1, r0
vmov  r5,s27
ldmdb  r5!,{ r3,r4 }
@ c8
and  r0, r10, r4
eor  r12, r0
vmov  s8,r12
@ c7
and  r0, r9, r4
eor  r11, r0
and  r0, r10, r3
eor  r11, r0
vmov  s7,r11
@ c6
and  r0, r8, r4
eor  r6, r0
and  r0, r9, r3
eor  r6, r0
@ c5
and  r0, r7, r4
eor  r1, r0
and  r0, r8, r3
eor  r1, r0
@ c4
vmov  r11,s4
and  r0, r2, r4
eor  r11, r0
and  r0, r7, r3
eor  r11, r0
@ c3
vmov  r12,s3
and  r0, r2, r3
eor  r12, r0
ldmdb  r5!,{ r3,r4,r14 }
@ c6
and  r0, r10, r14
eor  r6, r0
vmov  s6,r6
@ c5
and  r0, r10, r4
eor  r1, r0
and  r0, r9, r14
eor  r1, r0
vmov  s5,r1
@ c4
and  r0, r10, r3
eor  r11, r0
and  r0, r9, r4
eor  r11, r0
and  r0, r8, r14
eor  r11, r0
vmov  s4,r11
@ c3
and  r0, r9, r3
eor  r12, r0
and  r0, r8, r4
eor  r12, r0
and  r0, r7, r14
eor  r12, r0
vmov  s3,r12
@ c2
vmov  r1,s2
and  r0, r2, r14
eor  r1, r0
and  r0, r7, r4
eor  r1, r0
and  r0, r8, r3
eor  r1, r0
vmov  s2,r1
@ c1
vmov  r1,s1
and  r0, r2, r4
eor  r1, r0
and  r0, r7, r3
eor  r1, r0
vmov  s1,r1
@ c0
vmov  r1,s0
and  r0, r2, r3
eor  r1, r0
vmov  s0,r1
vmov r3,s28
subs r3,r3,#1
vmov s28,r3
bne  .Lmul13
.Lreduce13:
@ reduction
vmov  r0,s0
vmov  r1,s1
vmov  r2,s2
vmov  r3,s3
vmov  r4,s4
vmov  r5,s13
vmov  r6,s14
vmov  r7,s15
vmov  r8,s8
vmov  r9,s9
vmov  r10,s10
vmov  r11,s11
vmov  r12,s12
vmov  r14, s24
eor  r11, r14
eor  r12, r14
eor  r6, r14
eor  r7, r14
vmov  r14, s23
eor  r10, r14
eor  r11, r14
eor  r5, r14
eor  r6, r14
vmov  r14, s22
eor  r9, r14
eor  r10, r14
eor  r12, r14
eor  r5, r14
vmov  r14, s21
eor  r8, r14
eor  r9, r14
eor  r11, r14
eor  r12, r14
eor  r0, r5
eor  r1, r5
eor  r3, r5
eor  r4, r5
vmov  r5,s5
eor  r1, r6
eor  r2, r6
eor  r4, r6
eor  r5, r6
vmov  r6,s6
eor  r2, r7
eor  r3, r7
eor  r5, r7
eor  r6, r7
vmov  r7,s7
vmov  r14, s20
eor  r7, r14
eor  r8, r14
eor  r10, r14
eor  r11, r14
vmov  r14, s19
eor  r6, r14
eor  r7, r14
eor  r9, r14
eor  r10, r14
vmov  r14, s18
eor  r5, r14
eor  r6, r14
eor  r8, r14
eor  r9, r14
vmov  r14, s17
eor  r4, r14
eor  r5, r14
eor  r7, r14
eor  r8, r14
vmov  r14, s16
eor  r3, r14
eor  r4, r14
eor  r6, r14
eor  r7, r14
vmov  r14,s25
stm  r14,{ r0,r1,r2,r3,r4,r5,r6,r7,r8,r9,r10,r11,r12 }
vpop { d8-d15 }
pop { r4-r12,r14 }
.Lexitdot13:
bx lr
.size   vec32_2_13_accu_mul, .-vec32_2_13_accu_mul
