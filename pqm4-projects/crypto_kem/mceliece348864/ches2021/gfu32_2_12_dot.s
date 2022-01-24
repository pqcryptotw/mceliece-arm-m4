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
	.file	"gfu32_2_12_dot.c"
	.text
	.align	1
	.p2align 2,,3
	.global	gfu32_2_12_dot
	.syntax unified
	.thumb
	.thumb_func
	.fpu fpv4-sp-d16
	.type	gfu32_2_12_dot, %function
gfu32_2_12_dot:
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
@unpack inputs
	and	r7, lr, r5, lsr #1
	and	r6, lr, r4, lsr #1
	and	r5, r5, lr
	and	r4, r4, lr
@ mult
	umull	r10,r9, r6,r7
	umull	r8,r9, r4,r5
	umlal	r9, r10, r4,r7
	and	r9, r9, lr
	and	r10, r10, lr
	umlal	r9, r10, r5,r6
@accumulate
	eor	r11, r9
	eor	r12, r10
	subs	r3, r3, #1
	eor	r0, r0, r8
	bne	.L3

@reduce
        movw    r2, #4097
        and     r11, lr
        and     r12, lr
        and     r0, lr
        lsl     r1, r2, #16
        umlal   r0, r11, r12, r1
        and     r11, lr
        and     r0, lr
        lsr     r3, r11, #16
        umlal   r0, r11, r2, r3
@ output
        and     r0, lr
        and     r1, r11, lr, lsr #16
        eor     r0, r0, r1, lsl #1
.L2:
	pop	{r4, r5, r6, r7, r8, r9, r10, r11, r12, lr}
.L4:
	bx	lr
	.size	gfu32_2_12_dot, .-gfu32_2_12_dot
	.ident	"GCC: (15:7-2018-q2-6) 7.3.1 20180622 (release) [ARM/embedded-7-branch revision 261907]"
