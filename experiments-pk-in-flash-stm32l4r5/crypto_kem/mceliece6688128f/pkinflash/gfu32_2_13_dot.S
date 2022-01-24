	.cpu cortex-m4
	.eabi_attribute 27, 1
	.eabi_attribute 28, 1
	.eabi_attribute 20, 1
	.eabi_attribute 21, 1
	.eabi_attribute 23, 3
	.eabi_attribute 24, 1
	.eabi_attribute 25, 1
	.eabi_attribute 26, 1
	.eabi_attribute 30, 2
	.eabi_attribute 34, 1
	.eabi_attribute 18, 4
	.file	"gfu32_2_13_dot.c"
	.text
	.align	1
	.p2align 2,,3
	.global	gfu32_2_13_dot
	.syntax unified
	.thumb
	.thumb_func
	.fpu fpv4-sp-d16
	.type	gfu32_2_13_dot, %function
gfu32_2_13_dot:
	@ args = 0, pretend = 0, frame = 8
	@ frame_needed = 0, uses_anonymous_args = 0
	subs	r3, r2, #0
	mov	r2, r0
	mov	r0, #0
	ble	.L4
	push	{r4, r5, r6, r7, r8, r9, r10, r11, r12, lr}
	mov	r11, #0
	mov	r12, #0
	mov	lr,  #286331153
.L3:
	ldr	r5, [r1], #4
	ldr	r4, [r2], #4

	and	r7, lr, r5, lsr #1
	and	r6, lr, r4, lsr #1
	and	r5, r5, lr
	and	r4, r4, lr
@mult start
	umull	r10,r8, r6,r7
@ accumulate:  pack  largest coefficient with smallest coefficients
	eor	r0, r0, r8, lsl #1
@ end special accumulate
	umull	r8,r9, r4,r5
	umlal	r9, r10, r4,r7
	and	r9, r9, lr
	and	r10, r10, lr
	umlal	r9, r10, r5,r6
@ accumulate
	eor	r11, r9
	eor	r12, r10
	subs	r3, r3, #1
	eor	r0, r0, r8
	bne	.L3
@ reduce
        ldr     r2, .L6
        and     r4, r0, lr
        and     r5, r11, lr
        and     r6, r12, lr
	ubfx	r7, r0, #1, #1

        lsl     r1, r2, #12
        @umlal   r5, r2, r7, r1
        mla     r5, r7, r1, r5
        umlal   r4, r5, r6, r1
        lsr     r3, r5, #20
        umlal   r4, r5, r2, r3
@ output
        and     r0, r4, lr
        and     r1, r5, lr, lsr #12
        eor     r0, r0, r1, lsl #1
.L2:
	pop	{r4, r5, r6, r7, r8, r9, r10, r11, r12, lr}
.L4:
	bx	lr
	.align	2
.L6:
	.word	69649
	.size	gfu32_2_13_dot, .-gfu32_2_13_dot
	.ident	"GCC: (15:7-2018-q2-6) 7.3.1 20180622 (release) [ARM/embedded-7-branch revision 261907]"
