.syntax unified
.cpu cortex-m4
.fpu fpv4-sp-d16
.global gfu32_2_13_inv
.type gfu32_2_13_inv, %function
.align 2
gfu32_2_13_inv:
stmdb sp!, {r4, r5, r6, r7, r8, r9, sl}
mov.w r2, #286331153
and.w r1, r2, r0, lsr #1
and.w r0, r0, r2
mov.w r4, #17
eor.w r3, r4, r4, lsl #12
mov.w r4, r3, lsl #12
umull r7, r8, r1, r1
umull r5, r6, r0, r0
and.w r7, r7, #286331153
and.w r6, r6, #286331153
mla r6, r8, r4, r6
umlal r5, r6, r7, r4
mov.w r7, r6, lsr #20
umlal r5, r6, r7, r3
and.w r9, r5, r2
and.w sl, r6, r2, lsr #12
umull r7, r8, sl, r1
umull r5, r6, r9, r0
umlal r6, r7, r9, r1
and.w r6, r6, #286331153
and.w r7, r7, #286331153
umlal r6, r7, sl, r0
and.w r7, r7, #286331153
and.w r6, r6, #286331153
mla r6, r8, r4, r6
umlal r5, r6, r7, r4
mov.w r7, r6, lsr #20
umlal r5, r6, r7, r3
and.w r0, r5, r2
and.w r1, r6, r2, lsr #12
umull r7, r8, r1, r1
umull r5, r6, r0, r0
and.w r7, r7, #286331153
and.w r6, r6, #286331153
mla r6, r8, r4, r6
umlal r5, r6, r7, r4
mov.w r7, r6, lsr #20
umlal r5, r6, r7, r3
and.w r9, r5, r2
and.w sl, r6, r2, lsr #12
umull r7, r8, sl, sl
umull r5, r6, r9, r9
and.w r7, r7, #286331153
and.w r6, r6, #286331153
mla r6, r8, r4, r6
umlal r5, r6, r7, r4
mov.w r7, r6, lsr #20
umlal r5, r6, r7, r3
and.w r9, r5, r2
and.w sl, r6, r2, lsr #12
umull r7, r8, sl, r1
umull r5, r6, r9, r0
umlal r6, r7, r9, r1
and.w r6, r6, #286331153
and.w r7, r7, #286331153
umlal r6, r7, sl, r0
and.w r7, r7, #286331153
and.w r6, r6, #286331153
mla r6, r8, r4, r6
umlal r5, r6, r7, r4
mov.w r7, r6, lsr #20
umlal r5, r6, r7, r3
and.w r0, r5, r2
and.w r1, r6, r2, lsr #12
umull r7, r8, r1, r1
umull r5, r6, r0, r0
and.w r7, r7, #286331153
and.w r6, r6, #286331153
mla r6, r8, r4, r6
umlal r5, r6, r7, r4
mov.w r7, r6, lsr #20
umlal r5, r6, r7, r3
and.w r9, r5, r2
and.w sl, r6, r2, lsr #12
umull r7, r8, sl, sl
umull r5, r6, r9, r9
and.w r7, r7, #286331153
and.w r6, r6, #286331153
mla r6, r8, r4, r6
umlal r5, r6, r7, r4
mov.w r7, r6, lsr #20
umlal r5, r6, r7, r3
and.w r9, r5, r2
and.w sl, r6, r2, lsr #12
umull r7, r8, sl, sl
umull r5, r6, r9, r9
and.w r7, r7, #286331153
and.w r6, r6, #286331153
mla r6, r8, r4, r6
umlal r5, r6, r7, r4
mov.w r7, r6, lsr #20
umlal r5, r6, r7, r3
and.w r9, r5, r2
and.w sl, r6, r2, lsr #12
umull r7, r8, sl, sl
umull r5, r6, r9, r9
and.w r7, r7, #286331153
and.w r6, r6, #286331153
mla r6, r8, r4, r6
umlal r5, r6, r7, r4
mov.w r7, r6, lsr #20
umlal r5, r6, r7, r3
and.w r9, r5, r2
and.w sl, r6, r2, lsr #12
umull r7, r8, sl, r1
umull r5, r6, r9, r0
umlal r6, r7, r9, r1
and.w r6, r6, #286331153
and.w r7, r7, #286331153
umlal r6, r7, sl, r0
and.w r7, r7, #286331153
and.w r6, r6, #286331153
mla r6, r8, r4, r6
umlal r5, r6, r7, r4
mov.w r7, r6, lsr #20
umlal r5, r6, r7, r3
and.w r9, r5, r2
and.w sl, r6, r2, lsr #12
umull r7, r8, sl, sl
umull r5, r6, r9, r9
and.w r7, r7, #286331153
and.w r6, r6, #286331153
mla r6, r8, r4, r6
umlal r5, r6, r7, r4
mov.w r7, r6, lsr #20
umlal r5, r6, r7, r3
and.w r9, r5, r2
and.w sl, r6, r2, lsr #12
umull r7, r8, sl, sl
umull r5, r6, r9, r9
and.w r7, r7, #286331153
and.w r6, r6, #286331153
mla r6, r8, r4, r6
umlal r5, r6, r7, r4
mov.w r7, r6, lsr #20
umlal r5, r6, r7, r3
and.w r9, r5, r2
and.w sl, r6, r2, lsr #12
umull r7, r8, sl, sl
umull r5, r6, r9, r9
and.w r7, r7, #286331153
and.w r6, r6, #286331153
mla r6, r8, r4, r6
umlal r5, r6, r7, r4
mov.w r7, r6, lsr #20
umlal r5, r6, r7, r3
and.w r9, r5, r2
and.w sl, r6, r2, lsr #12
umull r7, r8, sl, sl
umull r5, r6, r9, r9
and.w r7, r7, #286331153
and.w r6, r6, #286331153
mla r6, r8, r4, r6
umlal r5, r6, r7, r4
mov.w r7, r6, lsr #20
umlal r5, r6, r7, r3
and.w r9, r5, r2
and.w sl, r6, r2, lsr #12
umull r7, r8, sl, r1
umull r5, r6, r9, r0
umlal r6, r7, r9, r1
and.w r6, r6, #286331153
and.w r7, r7, #286331153
umlal r6, r7, sl, r0
and.w r7, r7, #286331153
and.w r6, r6, #286331153
mla r6, r8, r4, r6
umlal r5, r6, r7, r4
mov.w r7, r6, lsr #20
umlal r5, r6, r7, r3
and.w r9, r5, r2
and.w sl, r6, r2, lsr #12
umull r7, r8, sl, sl
umull r5, r6, r9, r9
and.w r7, r7, #286331153
and.w r6, r6, #286331153
mla r6, r8, r4, r6
umlal r5, r6, r7, r4
mov.w r7, r6, lsr #20
umlal r5, r6, r7, r3
and.w r0, r5, r2
and.w r1, r6, r2, lsr #12
eor.w r0, r0, r1, lsl #1
ldmia.w sp!, {r4, r5, r6, r7, r8, r9, sl}
bx lr
.size   gfu32_2_13_inv, .-gfu32_2_13_inv
