.syntax unified
.cpu cortex-m4
.global idx_to_vec_64_3488_asm
.type idx_to_vec_64_3488_asm, %function
.align 2
idx_to_vec_64_3488_asm:
push {r4-r12, r14}
sub  r2, sp, #256
mov  r14, #1
.LOOPASIGNVAL643488:
ldm  r1!, { r9-r12 }
ubfx  r8, r9, #0, #5
lsl   r3, r14, r8
ubfx  r8, r9, #16, #5
lsl   r4, r14, r8
ubfx  r9, r10, #0, #5
lsl   r5, r14, r9
ubfx  r9, r10, #16, #5
lsl   r6, r14, r9
ubfx  r9, r11, #0, #5
lsl   r7, r14, r9
ubfx  r9, r11, #16, #5
lsl   r8, r14, r9
ubfx  r11, r12, #0, #5
lsl   r9,  r14, r11
ubfx  r11, r12, #16, #5
lsl   r10, r14, r11
stm   r2!,{ r3-r10 }
cmp   sp, r2
bne   .LOOPASIGNVAL643488
sub   r2, #256
sub   r1, #128
mov   r14, #0
.OUTERLOOP643488:
mov  r9, #0
.INNERLOOP643488:
ldm  r1!, { r11,r12 }
ldm  r2!, { r3-r6 }
ubfx r8, r11, #5, #11
eor  r7, r14, r8
sub  r7, #1
and  r3, r3, r7, asr #31
eor  r7, r14, r11, lsr #21
sub  r7, #1
and  r4, r4, r7, asr #31
ubfx r8, r12, #5, #11
eor  r7, r14, r8
sub  r7, #1
and  r5, r5, r7, asr #31
eor  r7, r14, r12, lsr #21
sub  r7, #1
and  r6, r6, r7, asr #31
orr  r3, r4
orr  r5, r6
orr  r9, r3
orr  r9, r5
ldm  r1!, { r11,r12 }
ldm  r2!, { r3-r6 }
ubfx r8, r11, #5, #11
eor  r7, r14, r8
sub  r7, #1
and  r3, r3, r7, asr #31
eor  r7, r14, r11, lsr #21
sub  r7, #1
and  r4, r4, r7, asr #31
ubfx r8, r12, #5, #11
eor  r7, r14, r8
sub  r7, #1
and  r5, r5, r7, asr #31
eor  r7, r14, r12, lsr #21
sub  r7, #1
and  r6, r6, r7, asr #31
orr  r3, r4
orr  r5, r6
orr  r9, r3
orr  r9, r5
ldm  r1!, { r11,r12 }
ldm  r2!, { r3-r6 }
ubfx r8, r11, #5, #11
eor  r7, r14, r8
sub  r7, #1
and  r3, r3, r7, asr #31
eor  r7, r14, r11, lsr #21
sub  r7, #1
and  r4, r4, r7, asr #31
ubfx r8, r12, #5, #11
eor  r7, r14, r8
sub  r7, #1
and  r5, r5, r7, asr #31
eor  r7, r14, r12, lsr #21
sub  r7, #1
and  r6, r6, r7, asr #31
orr  r3, r4
orr  r5, r6
orr  r9, r3
orr  r9, r5
ldm  r1!, { r11,r12 }
ldm  r2!, { r3-r6 }
ubfx r8, r11, #5, #11
eor  r7, r14, r8
sub  r7, #1
and  r3, r3, r7, asr #31
eor  r7, r14, r11, lsr #21
sub  r7, #1
and  r4, r4, r7, asr #31
ubfx r8, r12, #5, #11
eor  r7, r14, r8
sub  r7, #1
and  r5, r5, r7, asr #31
eor  r7, r14, r12, lsr #21
sub  r7, #1
and  r6, r6, r7, asr #31
orr  r3, r4
orr  r5, r6
orr  r9, r3
orr  r9, r5
cmp   sp, r2
bne   .INNERLOOP643488
sub   r2, #256
sub   r1, #128
add  r14, #1
str  r9, [r0], #4
cmp  r14, #109
bne  .OUTERLOOP643488
pop {r4-r12, r14}
bx lr
.syntax unified
.cpu cortex-m4
.global idx_to_vec_96_4608_asm
.type idx_to_vec_96_4608_asm, %function
.align 2
idx_to_vec_96_4608_asm:
push {r4-r12, r14}
sub  r2, sp, #384
mov  r14, #1
.LOOPASIGNVAL964608:
ldm  r1!, { r9-r12 }
ubfx  r8, r9, #0, #5
lsl   r3, r14, r8
ubfx  r8, r9, #16, #5
lsl   r4, r14, r8
ubfx  r9, r10, #0, #5
lsl   r5, r14, r9
ubfx  r9, r10, #16, #5
lsl   r6, r14, r9
ubfx  r9, r11, #0, #5
lsl   r7, r14, r9
ubfx  r9, r11, #16, #5
lsl   r8, r14, r9
ubfx  r11, r12, #0, #5
lsl   r9,  r14, r11
ubfx  r11, r12, #16, #5
lsl   r10, r14, r11
stm   r2!,{ r3-r10 }
cmp   sp, r2
bne   .LOOPASIGNVAL964608
sub   r2, #384
sub   r1, #192
mov   r14, #0
.OUTERLOOP964608:
mov  r9, #0
.INNERLOOP964608:
ldm  r1!, { r11,r12 }
ldm  r2!, { r3-r6 }
ubfx r8, r11, #5, #11
eor  r7, r14, r8
sub  r7, #1
and  r3, r3, r7, asr #31
eor  r7, r14, r11, lsr #21
sub  r7, #1
and  r4, r4, r7, asr #31
ubfx r8, r12, #5, #11
eor  r7, r14, r8
sub  r7, #1
and  r5, r5, r7, asr #31
eor  r7, r14, r12, lsr #21
sub  r7, #1
and  r6, r6, r7, asr #31
orr  r3, r4
orr  r5, r6
orr  r9, r3
orr  r9, r5
ldm  r1!, { r11,r12 }
ldm  r2!, { r3-r6 }
ubfx r8, r11, #5, #11
eor  r7, r14, r8
sub  r7, #1
and  r3, r3, r7, asr #31
eor  r7, r14, r11, lsr #21
sub  r7, #1
and  r4, r4, r7, asr #31
ubfx r8, r12, #5, #11
eor  r7, r14, r8
sub  r7, #1
and  r5, r5, r7, asr #31
eor  r7, r14, r12, lsr #21
sub  r7, #1
and  r6, r6, r7, asr #31
orr  r3, r4
orr  r5, r6
orr  r9, r3
orr  r9, r5
ldm  r1!, { r11,r12 }
ldm  r2!, { r3-r6 }
ubfx r8, r11, #5, #11
eor  r7, r14, r8
sub  r7, #1
and  r3, r3, r7, asr #31
eor  r7, r14, r11, lsr #21
sub  r7, #1
and  r4, r4, r7, asr #31
ubfx r8, r12, #5, #11
eor  r7, r14, r8
sub  r7, #1
and  r5, r5, r7, asr #31
eor  r7, r14, r12, lsr #21
sub  r7, #1
and  r6, r6, r7, asr #31
orr  r3, r4
orr  r5, r6
orr  r9, r3
orr  r9, r5
ldm  r1!, { r11,r12 }
ldm  r2!, { r3-r6 }
ubfx r8, r11, #5, #11
eor  r7, r14, r8
sub  r7, #1
and  r3, r3, r7, asr #31
eor  r7, r14, r11, lsr #21
sub  r7, #1
and  r4, r4, r7, asr #31
ubfx r8, r12, #5, #11
eor  r7, r14, r8
sub  r7, #1
and  r5, r5, r7, asr #31
eor  r7, r14, r12, lsr #21
sub  r7, #1
and  r6, r6, r7, asr #31
orr  r3, r4
orr  r5, r6
orr  r9, r3
orr  r9, r5
cmp   sp, r2
bne   .INNERLOOP964608
sub   r2, #384
sub   r1, #192
add  r14, #1
str  r9, [r0], #4
cmp  r14, #144
bne  .OUTERLOOP964608
pop {r4-r12, r14}
bx lr
