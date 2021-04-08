.syntax unified
.cpu cortex-m4
.global matxvec_madd_768x2720_asm
.type matxvec_madd_768x2720_asm, %function
.align 2
matxvec_madd_768x2720_asm:
push {r4-r12, r14}
push {r0}
mov r14, #0
push { r14 }
mov r3, #768
.HLOOP768x340:
ldm r2!, {r8-r10}
mov r0, #328
ldm r1!, {r11,r12,r14}
and r11, r8
and r12, r9
and r14, r10
eor r11, r12
eor r4, r11, r14
add r1, #328
ldm r1!, {r11,r12,r14}
and r11, r8
and r12, r9
and r14, r10
eor r11, r12
eor r5, r11, r14
add r1, #328
ldm r1!, {r11,r12,r14}
and r11, r8
and r12, r9
and r14, r10
eor r11, r12
eor r6, r11, r14
add r1, #328
ldm r1!, {r11,r12,r14}
and r11, r8
and r12, r9
and r14, r10
eor r11, r12
eor r7, r11, r14
sub r1, #1020
.WLOOP3768x340:
ldm r2!, {r8-r10}
sub r0, #12
ldm r1!, {r11,r12,r14}
and r11, r8
and r12, r9
and r14, r10
eor r4, r14
eor r11, r12
eor r4, r11
add r1, #328
ldm r1!, {r11,r12,r14}
and r11, r8
and r12, r9
and r14, r10
eor r5, r14
eor r11, r12
eor r5, r11
add r1, #328
ldm r1!, {r11,r12,r14}
and r11, r8
and r12, r9
and r14, r10
eor r6, r14
eor r11, r12
eor r6, r11
add r1, #328
ldm r1!, {r11,r12,r14}
and r11, r8
and r12, r9
and r14, r10
eor r7, r14
eor r11, r12
eor r7, r11
sub r1, #1020
cmp r0, #12
bge .WLOOP3768x340
cbz r0, .POSTWLOOP768x340
.WLOOP768x340:
ldr r8, [r2], #4
sub r0, #4
ldr r9, [r1], #4
ldr r10, [r1, #336]
ldr r11, [r1, #676]
ldr r12, [r1, #1016]
and r9, r8
and r10, r8
and r11, r8
and r12, r8
eor r4, r9
eor r5, r10
eor r6, r11
eor r7, r12
cmp  r0, #0
bgt .WLOOP768x340
.POSTWLOOP768x340:
add r1, #1020
sub r2, #340
.REDUCE768x340:
sub r3, #4
eor r8, r4, r4, lsr #2
eor r9, r5, r5, lsr #2
eor r10, r6, r6, lsr #2
eor r11, r7, r7, lsr #2
eor r4, r8, r8, lsr #1
eor r5, r9, r9, lsr #1
eor r6, r10, r10, lsr #1
eor r7, r11, r11, lsr #1
and r4, #0x11111111
and r5, #0x11111111
and r6, #0x11111111
and r7, #0x11111111
eor r8, r4, r5, lsl #1
eor r9, r6, r7, lsl #1
eor r4, r8, r9, lsl #2
eor r8, r4, r4, lsr #4
eor r4, r8, r8, lsr #8
eor r8, r4, r4, lsr #16
and r4, r8, #0xf
and r6, r3, #0x1f
rsb  r7, r6, #28
lsl r4, r7
pop { r14 }
eor r14, r14, r4
push { r14 }
cbz r6, .WB768x340
b  .HLOOP768x340
.WB768x340:
pop {r4,r5}
ldr  r6, [r5]
eor r4, r4, r6
str r4, [r5], #4
mov r4, #0
push { r4, r5 }
.TESTHEND768x340:
cmp r3, #0
bgt .HLOOP768x340
pop { r14 }
pop {r0}
pop {r4-r12, r14}
bx lr
.syntax unified
.cpu cortex-m4
.global matxvec_madd_1248x3360_asm
.type matxvec_madd_1248x3360_asm, %function
.align 2
matxvec_madd_1248x3360_asm:
push {r4-r12, r14}
push {r0}
mov r14, #0
push { r14 }
mov r3, #1248
.HLOOP1248x420:
ldm r2!, {r8-r10}
mov r0, #408
ldm r1!, {r11,r12,r14}
and r11, r8
and r12, r9
and r14, r10
eor r11, r12
eor r4, r11, r14
add r1, #408
ldm r1!, {r11,r12,r14}
and r11, r8
and r12, r9
and r14, r10
eor r11, r12
eor r5, r11, r14
add r1, #408
ldm r1!, {r11,r12,r14}
and r11, r8
and r12, r9
and r14, r10
eor r11, r12
eor r6, r11, r14
add r1, #408
ldm r1!, {r11,r12,r14}
and r11, r8
and r12, r9
and r14, r10
eor r11, r12
eor r7, r11, r14
sub r1, #1260
.WLOOP31248x420:
ldm r2!, {r8-r10}
sub r0, #12
ldm r1!, {r11,r12,r14}
and r11, r8
and r12, r9
and r14, r10
eor r4, r14
eor r11, r12
eor r4, r11
add r1, #408
ldm r1!, {r11,r12,r14}
and r11, r8
and r12, r9
and r14, r10
eor r5, r14
eor r11, r12
eor r5, r11
add r1, #408
ldm r1!, {r11,r12,r14}
and r11, r8
and r12, r9
and r14, r10
eor r6, r14
eor r11, r12
eor r6, r11
add r1, #408
ldm r1!, {r11,r12,r14}
and r11, r8
and r12, r9
and r14, r10
eor r7, r14
eor r11, r12
eor r7, r11
sub r1, #1260
cmp r0, #12
bge .WLOOP31248x420
cbz r0, .POSTWLOOP1248x420
.WLOOP1248x420:
ldr r8, [r2], #4
sub r0, #4
ldr r9, [r1], #4
ldr r10, [r1, #416]
ldr r11, [r1, #836]
ldr r12, [r1, #1256]
and r9, r8
and r10, r8
and r11, r8
and r12, r8
eor r4, r9
eor r5, r10
eor r6, r11
eor r7, r12
cmp  r0, #0
bgt .WLOOP1248x420
.POSTWLOOP1248x420:
add r1, #1260
sub r2, #420
.REDUCE1248x420:
sub r3, #4
eor r8, r4, r4, lsr #2
eor r9, r5, r5, lsr #2
eor r10, r6, r6, lsr #2
eor r11, r7, r7, lsr #2
eor r4, r8, r8, lsr #1
eor r5, r9, r9, lsr #1
eor r6, r10, r10, lsr #1
eor r7, r11, r11, lsr #1
and r4, #0x11111111
and r5, #0x11111111
and r6, #0x11111111
and r7, #0x11111111
eor r8, r4, r5, lsl #1
eor r9, r6, r7, lsl #1
eor r4, r8, r9, lsl #2
eor r8, r4, r4, lsr #4
eor r4, r8, r8, lsr #8
eor r8, r4, r4, lsr #16
and r4, r8, #0xf
and r6, r3, #0x1f
rsb  r7, r6, #28
lsl r4, r7
pop { r14 }
eor r14, r14, r4
push { r14 }
cbz r6, .WB1248x420
b  .HLOOP1248x420
.WB1248x420:
pop {r4,r5}
ldr  r6, [r5]
eor r4, r4, r6
str r4, [r5], #4
mov r4, #0
push { r4, r5 }
.TESTHEND1248x420:
cmp r3, #0
bgt .HLOOP1248x420
pop { r14 }
pop {r0}
pop {r4-r12, r14}
bx lr
