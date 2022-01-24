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
	.file	"gfu32_2_12_mul.c"
	.text
	.align	1
	.p2align 2,,3
	.global	gfu32_2_12_mul
	.syntax unified
	.thumb
	.thumb_func
	.fpu fpv4-sp-d16
	.type	gfu32_2_12_mul, %function
gfu32_2_12_mul:
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, r5, lr}
@ unpack inputs
	mov	ip, #286331153
	and	r2, ip, r0, lsr #1
	and	r3, ip, r1, lsr #1
	and	r0, ip, r0
	and	r1, ip, r1
@ mult start
	umull	lr, r4, r2, r3
	umull	r4, r5, r0, r1
	umlal	r5, lr, r0, r3
	and	r5, ip
	and	lr, ip
	umlal	r5, lr, r1, r2
@ reduce
	movw	r2, #4097
	and	lr, ip
	lsl	r1, r2, #16
	umlal	r4, r5, lr, r1
	and	r5, ip
	and	r4, ip
	lsr	r3, r5, #16
	umlal	r4, r5, r2, r3
@ output
	and	r0, r4, ip
	and	r1, r5, ip, lsr #16
	eor	r0, r0, r1, lsl #1

	pop	{r4, r5, pc}
.L5:
	.size	gfu32_2_12_mul, .-gfu32_2_12_mul
	.ident	"GCC: (15:7-2018-q2-6) 7.3.1 20180622 (release) [ARM/embedded-7-branch revision 261907]"
