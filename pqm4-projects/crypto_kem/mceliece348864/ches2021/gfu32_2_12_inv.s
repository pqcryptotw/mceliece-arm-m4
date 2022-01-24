.syntax unified
.cpu cortex-m4
.fpu fpv4-sp-d16
.global gfu32_2_12_inv
.type gfu32_2_12_inv, %function
.align 2
gfu32_2_12_inv:
stmdb sp!, {r4, r5, r6, r7, r8, r9, sl, fp, ip}
mov.w ip, #286331153
and.w r1, ip, r0, lsr #1
and.w r0, r0, ip
movw r2, #4097
mov.w r3, r2, lsl #16
umull r6, r4, r1, r1
umull r4, r5, r0, r0
and.w r6, r6, #286331153
umlal r4, r5, r6, r3
and.w r4, r4, #286331153
and.w r5, r5, #286331153
mov.w r6, r5, lsr #16
umlal r4, r5, r6, r2
and.w r7, r4, ip
and.w r8, r5, ip, lsr #16
umull r6, r9, r1, r8
umull r4, r5, r0, r7
umlal r5, r6, r0, r8
and.w r5, r5, #286331153
and.w r6, r6, #286331153
umlal r5, r6, r1, r7
and.w r6, r6, #286331153
umlal r4, r5, r6, r3
and.w r4, r4, #286331153
and.w r5, r5, #286331153
mov.w r6, r5, lsr #16
umlal r4, r5, r6, r2
and.w r7, r4, ip
and.w r8, r5, ip, lsr #16
umull r6, r4, r8, r8
umull r4, r5, r7, r7
and.w r6, r6, #286331153
umlal r4, r5, r6, r3
and.w r4, r4, #286331153
and.w r5, r5, #286331153
mov.w r6, r5, lsr #16
umlal r4, r5, r6, r2
and.w r7, r4, ip
and.w r8, r5, ip, lsr #16
umull r6, r9, r1, r8
umull r4, r5, r0, r7
umlal r5, r6, r0, r8
and.w r5, r5, #286331153
and.w r6, r6, #286331153
umlal r5, r6, r1, r7
and.w r6, r6, #286331153
umlal r4, r5, r6, r3
and.w r4, r4, #286331153
and.w r5, r5, #286331153
mov.w r6, r5, lsr #16
umlal r4, r5, r6, r2
and.w sl, r4, ip
and.w fp, r5, ip, lsr #16
umull r6, r4, fp, fp
umull r4, r5, sl, sl
and.w r6, r6, #286331153
umlal r4, r5, r6, r3
and.w r4, r4, #286331153
and.w r5, r5, #286331153
mov.w r6, r5, lsr #16
umlal r4, r5, r6, r2
and.w r7, r4, ip
and.w r8, r5, ip, lsr #16
umull r6, r9, r1, r8
umull r4, r5, r0, r7
umlal r5, r6, r0, r8
and.w r5, r5, #286331153
and.w r6, r6, #286331153
umlal r5, r6, r1, r7
and.w r6, r6, #286331153
umlal r4, r5, r6, r3
and.w r4, r4, #286331153
and.w r5, r5, #286331153
mov.w r6, r5, lsr #16
umlal r4, r5, r6, r2
and.w r0, r4, ip
and.w r1, r5, ip, lsr #16
umull r6, r4, r1, r1
umull r4, r5, r0, r0
and.w r6, r6, #286331153
umlal r4, r5, r6, r3
and.w r4, r4, #286331153
and.w r5, r5, #286331153
mov.w r6, r5, lsr #16
umlal r4, r5, r6, r2
and.w r7, r4, ip
and.w r8, r5, ip, lsr #16
umull r6, r4, r8, r8
umull r4, r5, r7, r7
and.w r6, r6, #286331153
umlal r4, r5, r6, r3
and.w r4, r4, #286331153
and.w r5, r5, #286331153
mov.w r6, r5, lsr #16
umlal r4, r5, r6, r2
and.w r7, r4, ip
and.w r8, r5, ip, lsr #16
umull r6, r4, r8, r8
umull r4, r5, r7, r7
and.w r6, r6, #286331153
umlal r4, r5, r6, r3
and.w r4, r4, #286331153
and.w r5, r5, #286331153
mov.w r6, r5, lsr #16
umlal r4, r5, r6, r2
and.w r7, r4, ip
and.w r8, r5, ip, lsr #16
umull r6, r4, r8, r8
umull r4, r5, r7, r7
and.w r6, r6, #286331153
umlal r4, r5, r6, r3
and.w r4, r4, #286331153
and.w r5, r5, #286331153
mov.w r6, r5, lsr #16
umlal r4, r5, r6, r2
and.w r7, r4, ip
and.w r8, r5, ip, lsr #16
umull r6, r9, r1, r8
umull r4, r5, r0, r7
umlal r5, r6, r0, r8
and.w r5, r5, #286331153
and.w r6, r6, #286331153
umlal r5, r6, r1, r7
and.w r6, r6, #286331153
umlal r4, r5, r6, r3
and.w r4, r4, #286331153
and.w r5, r5, #286331153
mov.w r6, r5, lsr #16
umlal r4, r5, r6, r2
and.w r7, r4, ip
and.w r8, r5, ip, lsr #16
umull r6, r4, r8, r8
umull r4, r5, r7, r7
and.w r6, r6, #286331153
umlal r4, r5, r6, r3
and.w r4, r4, #286331153
and.w r5, r5, #286331153
mov.w r6, r5, lsr #16
umlal r4, r5, r6, r2
and.w r7, r4, ip
and.w r8, r5, ip, lsr #16
umull r6, r4, r8, r8
umull r4, r5, r7, r7
and.w r6, r6, #286331153
umlal r4, r5, r6, r3
and.w r4, r4, #286331153
and.w r5, r5, #286331153
mov.w r6, r5, lsr #16
umlal r4, r5, r6, r2
and.w r7, r4, ip
and.w r8, r5, ip, lsr #16
umull r6, r4, r8, r8
umull r4, r5, r7, r7
and.w r6, r6, #286331153
umlal r4, r5, r6, r3
and.w r4, r4, #286331153
and.w r5, r5, #286331153
mov.w r6, r5, lsr #16
umlal r4, r5, r6, r2
and.w r7, r4, ip
and.w r8, r5, ip, lsr #16
umull r6, r9, fp, r8
umull r4, r5, sl, r7
umlal r5, r6, sl, r8
and.w r5, r5, #286331153
and.w r6, r6, #286331153
umlal r5, r6, fp, r7
and.w r6, r6, #286331153
umlal r4, r5, r6, r3
and.w r4, r4, #286331153
and.w r5, r5, #286331153
mov.w r6, r5, lsr #16
umlal r4, r5, r6, r2
and.w r7, r4, ip
and.w r8, r5, ip, lsr #16
umull r6, r4, r8, r8
umull r4, r5, r7, r7
and.w r6, r6, #286331153
umlal r4, r5, r6, r3
and.w r4, r4, #286331153
and.w r5, r5, #286331153
mov.w r6, r5, lsr #16
umlal r4, r5, r6, r2
and.w r0, r4, ip
and.w r1, r5, ip, lsr #16
eor.w r0, r0, r1, lsl #1
ldmia.w sp!, {r4, r5, r6, r7, r8, r9, sl, fp, ip}
bx lr
.size   gfu32_2_12_inv, .-gfu32_2_12_inv
