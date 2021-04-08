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
	.file	"gfu32_2_12_smul.c"
	.text
	.align	1
	.p2align 2,,3
	.global	gfu32_2_12_smul
	.syntax unified
	.thumb
	.thumb_func
	.fpu fpv4-sp-d16
	.type	gfu32_2_12_smul, %function
gfu32_2_12_smul:
	@ args = 0, pretend = 0, frame = 8
	@ frame_needed = 0, uses_anonymous_args = 0
	cmp	r3, #0
	ble	.L4
	push	{r4, r5, r6, r7, r8, r9, r10, r11, r12, lr}
	mov	lr,  #286331153
@ constant for reduce
	mov	r12, #4097
	lsl	r11, r12, #16
@ b
	and	r10, lr, r2, lsr #1
	and	r2, lr
.L3:
	ldr	r4, [r1], #4
@unpack inputs
	and	r5, lr, r4, lsr #1
	and	r4, r4, lr
@ mult
	umull	r8, r6, r5, r10
	umull	r6, r7, r4, r2
	umlal	r7, r8, r4, r10
	and	r7, r7, lr
	and	r8, r8, lr
	umlal	r7, r8, r5, r2

@reduce
        and     r8, lr
        umlal   r6, r7, r8, r11
        and     r6, lr
        and     r7, lr
        lsr     r8, r7, #16
        umlal   r6, r7, r8, r12
@ output
        and     r6, lr
        and     r7, r7, lr, lsr #16
        eor     r6, r6, r7, lsl #1

	subs	r3, r3, #1
	str	r6, [r0], #4
	bne	.L3
.L2:
	pop	{r4, r5, r6, r7, r8, r9, r10, r11, r12, lr}
.L4:
	bx	lr
	.size	gfu32_2_12_smul, .-gfu32_2_12_smul
	.ident	"GCC: (15:7-2018-q2-6) 7.3.1 20180622 (release) [ARM/embedded-7-branch revision 261907]"
