.syntax unified
.cpu cortex-m4
.global transpose_32x32_in_asm
.type transpose_32x32_in_asm, %function
.align 2
transpose_32x32_in_asm:
stmdb sp!, {r4, r5, r6, r7, r8, r9, sl, fp, ip, lr}
mov.w r1, r0
ldmia.w r0, {r4, r5, r6, r7, r8, r9, sl, fp}
and.w r2, r4, #252645135
and.w r3, r8, #252645135
orr.w ip, r2, r3, lsl #4
and.w r2, r4, #4042322160
and.w r3, r8, #4042322160
orr.w lr, r3, r2, lsr #4
and.w r2, r5, #252645135
and.w r3, r9, #252645135
orr.w r4, r2, r3, lsl #4
and.w r2, r5, #4042322160
and.w r3, r9, #4042322160
orr.w r8, r3, r2, lsr #4
and.w r2, r6, #252645135
and.w r3, sl, #252645135
orr.w r5, r2, r3, lsl #4
and.w r2, r6, #4042322160
and.w r3, sl, #4042322160
orr.w r9, r3, r2, lsr #4
and.w r2, r7, #252645135
and.w r3, fp, #252645135
orr.w r6, r2, r3, lsl #4
and.w r2, r7, #4042322160
and.w r3, fp, #4042322160
orr.w sl, r3, r2, lsr #4
and.w r2, ip, #858993459
and.w r3, r5, #858993459
orr.w r7, r2, r3, lsl #2
and.w r2, ip, #3435973836
and.w r3, r5, #3435973836
orr.w fp, r3, r2, lsr #2
and.w r2, r4, #858993459
and.w r3, r6, #858993459
orr.w ip, r2, r3, lsl #2
and.w r2, r4, #3435973836
and.w r3, r6, #3435973836
orr.w r5, r3, r2, lsr #2
and.w r2, lr, #858993459
and.w r3, r9, #858993459
orr.w r4, r2, r3, lsl #2
and.w r2, lr, #3435973836
and.w r3, r9, #3435973836
orr.w r6, r3, r2, lsr #2
and.w r2, r8, #858993459
and.w r3, sl, #858993459
orr.w lr, r2, r3, lsl #2
and.w r2, r8, #3435973836
and.w r3, sl, #3435973836
orr.w r9, r3, r2, lsr #2
and.w r2, r7, #1431655765
and.w r3, ip, #1431655765
orr.w r8, r2, r3, lsl #1
and.w r2, r7, #2863311530
and.w r3, ip, #2863311530
orr.w sl, r3, r2, lsr #1
and.w r2, fp, #1431655765
and.w r3, r5, #1431655765
orr.w r7, r2, r3, lsl #1
and.w r2, fp, #2863311530
and.w r3, r5, #2863311530
orr.w ip, r3, r2, lsr #1
and.w r2, r4, #1431655765
and.w r3, lr, #1431655765
orr.w fp, r2, r3, lsl #1
and.w r2, r4, #2863311530
and.w r3, lr, #2863311530
orr.w r5, r3, r2, lsr #1
and.w r2, r6, #1431655765
and.w r3, r9, #1431655765
orr.w r4, r2, r3, lsl #1
and.w r2, r6, #2863311530
and.w r3, r9, #2863311530
orr.w lr, r3, r2, lsr #1
str.w r8, [r0]
str.w sl, [r0, #4]
str.w r7, [r0, #8]
str.w ip, [r0, #12]
str.w fp, [r0, #16]
str r5, [r0, #20]
str r4, [r0, #24]
str.w lr, [r0, #28]
add.w r0, r1, #32
ldmia.w r0, {r4, r5, r6, r7, r8, r9, sl, fp}
and.w r2, r4, #252645135
and.w r3, r8, #252645135
orr.w ip, r2, r3, lsl #4
and.w r2, r4, #4042322160
and.w r3, r8, #4042322160
orr.w lr, r3, r2, lsr #4
and.w r2, r5, #252645135
and.w r3, r9, #252645135
orr.w r4, r2, r3, lsl #4
and.w r2, r5, #4042322160
and.w r3, r9, #4042322160
orr.w r8, r3, r2, lsr #4
and.w r2, r6, #252645135
and.w r3, sl, #252645135
orr.w r5, r2, r3, lsl #4
and.w r2, r6, #4042322160
and.w r3, sl, #4042322160
orr.w r9, r3, r2, lsr #4
and.w r2, r7, #252645135
and.w r3, fp, #252645135
orr.w r6, r2, r3, lsl #4
and.w r2, r7, #4042322160
and.w r3, fp, #4042322160
orr.w sl, r3, r2, lsr #4
and.w r2, ip, #858993459
and.w r3, r5, #858993459
orr.w r7, r2, r3, lsl #2
and.w r2, ip, #3435973836
and.w r3, r5, #3435973836
orr.w fp, r3, r2, lsr #2
and.w r2, r4, #858993459
and.w r3, r6, #858993459
orr.w ip, r2, r3, lsl #2
and.w r2, r4, #3435973836
and.w r3, r6, #3435973836
orr.w r5, r3, r2, lsr #2
and.w r2, lr, #858993459
and.w r3, r9, #858993459
orr.w r4, r2, r3, lsl #2
and.w r2, lr, #3435973836
and.w r3, r9, #3435973836
orr.w r6, r3, r2, lsr #2
and.w r2, r8, #858993459
and.w r3, sl, #858993459
orr.w lr, r2, r3, lsl #2
and.w r2, r8, #3435973836
and.w r3, sl, #3435973836
orr.w r9, r3, r2, lsr #2
and.w r2, r7, #1431655765
and.w r3, ip, #1431655765
orr.w r8, r2, r3, lsl #1
and.w r2, r7, #2863311530
and.w r3, ip, #2863311530
orr.w sl, r3, r2, lsr #1
and.w r2, fp, #1431655765
and.w r3, r5, #1431655765
orr.w r7, r2, r3, lsl #1
and.w r2, fp, #2863311530
and.w r3, r5, #2863311530
orr.w ip, r3, r2, lsr #1
and.w r2, r4, #1431655765
and.w r3, lr, #1431655765
orr.w fp, r2, r3, lsl #1
and.w r2, r4, #2863311530
and.w r3, lr, #2863311530
orr.w r5, r3, r2, lsr #1
and.w r2, r6, #1431655765
and.w r3, r9, #1431655765
orr.w r4, r2, r3, lsl #1
and.w r2, r6, #2863311530
and.w r3, r9, #2863311530
orr.w lr, r3, r2, lsr #1
str.w r8, [r0]
str.w sl, [r0, #4]
str.w r7, [r0, #8]
str.w ip, [r0, #12]
str.w fp, [r0, #16]
str r5, [r0, #20]
str r4, [r0, #24]
str.w lr, [r0, #28]
add.w r0, r1, #64
ldmia.w r0, {r4, r5, r6, r7, r8, r9, sl, fp}
and.w r2, r4, #252645135
and.w r3, r8, #252645135
orr.w ip, r2, r3, lsl #4
and.w r2, r4, #4042322160
and.w r3, r8, #4042322160
orr.w lr, r3, r2, lsr #4
and.w r2, r5, #252645135
and.w r3, r9, #252645135
orr.w r4, r2, r3, lsl #4
and.w r2, r5, #4042322160
and.w r3, r9, #4042322160
orr.w r8, r3, r2, lsr #4
and.w r2, r6, #252645135
and.w r3, sl, #252645135
orr.w r5, r2, r3, lsl #4
and.w r2, r6, #4042322160
and.w r3, sl, #4042322160
orr.w r9, r3, r2, lsr #4
and.w r2, r7, #252645135
and.w r3, fp, #252645135
orr.w r6, r2, r3, lsl #4
and.w r2, r7, #4042322160
and.w r3, fp, #4042322160
orr.w sl, r3, r2, lsr #4
and.w r2, ip, #858993459
and.w r3, r5, #858993459
orr.w r7, r2, r3, lsl #2
and.w r2, ip, #3435973836
and.w r3, r5, #3435973836
orr.w fp, r3, r2, lsr #2
and.w r2, r4, #858993459
and.w r3, r6, #858993459
orr.w ip, r2, r3, lsl #2
and.w r2, r4, #3435973836
and.w r3, r6, #3435973836
orr.w r5, r3, r2, lsr #2
and.w r2, lr, #858993459
and.w r3, r9, #858993459
orr.w r4, r2, r3, lsl #2
and.w r2, lr, #3435973836
and.w r3, r9, #3435973836
orr.w r6, r3, r2, lsr #2
and.w r2, r8, #858993459
and.w r3, sl, #858993459
orr.w lr, r2, r3, lsl #2
and.w r2, r8, #3435973836
and.w r3, sl, #3435973836
orr.w r9, r3, r2, lsr #2
and.w r2, r7, #1431655765
and.w r3, ip, #1431655765
orr.w r8, r2, r3, lsl #1
and.w r2, r7, #2863311530
and.w r3, ip, #2863311530
orr.w sl, r3, r2, lsr #1
and.w r2, fp, #1431655765
and.w r3, r5, #1431655765
orr.w r7, r2, r3, lsl #1
and.w r2, fp, #2863311530
and.w r3, r5, #2863311530
orr.w ip, r3, r2, lsr #1
and.w r2, r4, #1431655765
and.w r3, lr, #1431655765
orr.w fp, r2, r3, lsl #1
and.w r2, r4, #2863311530
and.w r3, lr, #2863311530
orr.w r5, r3, r2, lsr #1
and.w r2, r6, #1431655765
and.w r3, r9, #1431655765
orr.w r4, r2, r3, lsl #1
and.w r2, r6, #2863311530
and.w r3, r9, #2863311530
orr.w lr, r3, r2, lsr #1
str.w r8, [r0]
str.w sl, [r0, #4]
str.w r7, [r0, #8]
str.w ip, [r0, #12]
str.w fp, [r0, #16]
str r5, [r0, #20]
str r4, [r0, #24]
str.w lr, [r0, #28]
add.w r0, r1, #96
ldmia.w r0, {r4, r5, r6, r7, r8, r9, sl, fp}
and.w r2, r4, #252645135
and.w r3, r8, #252645135
orr.w ip, r2, r3, lsl #4
and.w r2, r4, #4042322160
and.w r3, r8, #4042322160
orr.w lr, r3, r2, lsr #4
and.w r2, r5, #252645135
and.w r3, r9, #252645135
orr.w r4, r2, r3, lsl #4
and.w r2, r5, #4042322160
and.w r3, r9, #4042322160
orr.w r8, r3, r2, lsr #4
and.w r2, r6, #252645135
and.w r3, sl, #252645135
orr.w r5, r2, r3, lsl #4
and.w r2, r6, #4042322160
and.w r3, sl, #4042322160
orr.w r9, r3, r2, lsr #4
and.w r2, r7, #252645135
and.w r3, fp, #252645135
orr.w r6, r2, r3, lsl #4
and.w r2, r7, #4042322160
and.w r3, fp, #4042322160
orr.w sl, r3, r2, lsr #4
and.w r2, ip, #858993459
and.w r3, r5, #858993459
orr.w r7, r2, r3, lsl #2
and.w r2, ip, #3435973836
and.w r3, r5, #3435973836
orr.w fp, r3, r2, lsr #2
and.w r2, r4, #858993459
and.w r3, r6, #858993459
orr.w ip, r2, r3, lsl #2
and.w r2, r4, #3435973836
and.w r3, r6, #3435973836
orr.w r5, r3, r2, lsr #2
and.w r2, lr, #858993459
and.w r3, r9, #858993459
orr.w r4, r2, r3, lsl #2
and.w r2, lr, #3435973836
and.w r3, r9, #3435973836
orr.w r6, r3, r2, lsr #2
and.w r2, r8, #858993459
and.w r3, sl, #858993459
orr.w lr, r2, r3, lsl #2
and.w r2, r8, #3435973836
and.w r3, sl, #3435973836
orr.w r9, r3, r2, lsr #2
and.w r2, r7, #1431655765
and.w r3, ip, #1431655765
orr.w r8, r2, r3, lsl #1
and.w r2, r7, #2863311530
and.w r3, ip, #2863311530
orr.w sl, r3, r2, lsr #1
and.w r2, fp, #1431655765
and.w r3, r5, #1431655765
orr.w r7, r2, r3, lsl #1
and.w r2, fp, #2863311530
and.w r3, r5, #2863311530
orr.w ip, r3, r2, lsr #1
and.w r2, r4, #1431655765
and.w r3, lr, #1431655765
orr.w fp, r2, r3, lsl #1
and.w r2, r4, #2863311530
and.w r3, lr, #2863311530
orr.w r5, r3, r2, lsr #1
and.w r2, r6, #1431655765
and.w r3, r9, #1431655765
orr.w r4, r2, r3, lsl #1
and.w r2, r6, #2863311530
and.w r3, r9, #2863311530
orr.w lr, r3, r2, lsr #1
str.w r8, [r0]
str.w sl, [r0, #4]
str.w r7, [r0, #8]
str.w ip, [r0, #12]
str.w fp, [r0, #16]
str r5, [r0, #20]
str r4, [r0, #24]
str.w lr, [r0, #28]
mov.w r0, r1
movw r1, #65535
ldrd r4, r5, [r0]
ldrd r6, r7, [r0, #32]
ldrd r8, r9, [r0, #64]
ldrd sl, fp, [r0, #96]
and.w r2, r4, #16711935
and.w r3, r6, #16711935
orr.w ip, r2, r3, lsl #8
and.w r2, r4, #4278255360
and.w r3, r6, #4278255360
orr.w lr, r3, r2, lsr #8
and.w r2, r5, #16711935
and.w r3, r7, #16711935
orr.w r4, r2, r3, lsl #8
and.w r2, r5, #4278255360
and.w r3, r7, #4278255360
orr.w r6, r3, r2, lsr #8
and.w r2, r8, #16711935
and.w r3, sl, #16711935
orr.w r5, r2, r3, lsl #8
and.w r2, r8, #4278255360
and.w r3, sl, #4278255360
orr.w r7, r3, r2, lsr #8
and.w r2, r9, #16711935
and.w r3, fp, #16711935
orr.w r8, r2, r3, lsl #8
and.w r2, r9, #4278255360
and.w r3, fp, #4278255360
orr.w sl, r3, r2, lsr #8
and.w r2, ip, r1
orr.w r9, r2, r5, lsl #16
bic.w r3, r5, r1
orr.w fp, r3, ip, lsr #16
and.w r2, r4, r1
orr.w ip, r2, r8, lsl #16
bic.w r3, r8, r1
orr.w r5, r3, r4, lsr #16
and.w r2, lr, r1
orr.w r4, r2, r7, lsl #16
bic.w r3, r7, r1
orr.w r8, r3, lr, lsr #16
and.w r2, r6, r1
orr.w lr, r2, sl, lsl #16
bic.w r3, sl, r1
orr.w r7, r3, r6, lsr #16
str.w r9, [r0]
str.w ip, [r0, #4]
str.w r4, [r0, #32]
str.w lr, [r0, #36]
str.w fp, [r0, #64]
str.w r5, [r0, #68]
str.w r8, [r0, #96]
str.w r7, [r0, #100]
ldrd r9, ip, [r0, #8]
ldrd r4, lr, [r0, #40]
ldrd fp, r5, [r0, #72]
ldrd r8, r7, [r0, #104]
and.w r2, r9, #16711935
and.w r3, r4, #16711935
orr.w r6, r2, r3, lsl #8
and.w r2, r9, #4278255360
and.w r3, r4, #4278255360
orr.w sl, r3, r2, lsr #8
and.w r2, ip, #16711935
and.w r3, lr, #16711935
orr.w r9, r2, r3, lsl #8
and.w r2, ip, #4278255360
and.w r3, lr, #4278255360
orr.w r4, r3, r2, lsr #8
and.w r2, fp, #16711935
and.w r3, r8, #16711935
orr.w ip, r2, r3, lsl #8
and.w r2, fp, #4278255360
and.w r3, r8, #4278255360
orr.w lr, r3, r2, lsr #8
and.w r2, r5, #16711935
and.w r3, r7, #16711935
orr.w fp, r2, r3, lsl #8
and.w r2, r5, #4278255360
and.w r3, r7, #4278255360
orr.w r8, r3, r2, lsr #8
and.w r2, r6, r1
orr.w r5, r2, ip, lsl #16
bic.w r3, ip, r1
orr.w r7, r3, r6, lsr #16
and.w r2, r9, r1
orr.w r6, r2, fp, lsl #16
bic.w r3, fp, r1
orr.w ip, r3, r9, lsr #16
and.w r2, sl, r1
orr.w r9, r2, lr, lsl #16
bic.w r3, lr, r1
orr.w fp, r3, sl, lsr #16
and.w r2, r4, r1
orr.w sl, r2, r8, lsl #16
bic.w r3, r8, r1
orr.w lr, r3, r4, lsr #16
str r5, [r0, #8]
str r6, [r0, #12]
str.w r9, [r0, #40]
str.w sl, [r0, #44]
str.w r7, [r0, #72]
str.w ip, [r0, #76]
str.w fp, [r0, #104]
str.w lr, [r0, #108]
ldrd r5, r6, [r0, #16]
ldrd r9, sl, [r0, #48]
ldrd r7, ip, [r0, #80]
ldrd fp, lr, [r0, #112]
and.w r2, r5, #16711935
and.w r3, r9, #16711935
orr.w r4, r2, r3, lsl #8
and.w r2, r5, #4278255360
and.w r3, r9, #4278255360
orr.w r8, r3, r2, lsr #8
and.w r2, r6, #16711935
and.w r3, sl, #16711935
orr.w r5, r2, r3, lsl #8
and.w r2, r6, #4278255360
and.w r3, sl, #4278255360
orr.w r9, r3, r2, lsr #8
and.w r2, r7, #16711935
and.w r3, fp, #16711935
orr.w r6, r2, r3, lsl #8
and.w r2, r7, #4278255360
and.w r3, fp, #4278255360
orr.w sl, r3, r2, lsr #8
and.w r2, ip, #16711935
and.w r3, lr, #16711935
orr.w r7, r2, r3, lsl #8
and.w r2, ip, #4278255360
and.w r3, lr, #4278255360
orr.w fp, r3, r2, lsr #8
and.w r2, r4, r1
orr.w ip, r2, r6, lsl #16
bic.w r3, r6, r1
orr.w lr, r3, r4, lsr #16
and.w r2, r5, r1
orr.w r4, r2, r7, lsl #16
bic.w r3, r7, r1
orr.w r6, r3, r5, lsr #16
and.w r2, r8, r1
orr.w r5, r2, sl, lsl #16
bic.w r3, sl, r1
orr.w r7, r3, r8, lsr #16
and.w r2, r9, r1
orr.w r8, r2, fp, lsl #16
bic.w r3, fp, r1
orr.w sl, r3, r9, lsr #16
str.w ip, [r0, #16]
str r4, [r0, #20]
str r5, [r0, #48]
str.w r8, [r0, #52]
str.w lr, [r0, #80]
str r6, [r0, #84]
str r7, [r0, #112]
str.w sl, [r0, #116]
ldrd ip, r4, [r0, #24]
ldrd r5, r8, [r0, #56]
ldrd lr, r6, [r0, #88]
ldrd r7, sl, [r0, #120]
and.w r2, ip, #16711935
and.w r3, r5, #16711935
orr.w r9, r2, r3, lsl #8
and.w r2, ip, #4278255360
and.w r3, r5, #4278255360
orr.w fp, r3, r2, lsr #8
and.w r2, r4, #16711935
and.w r3, r8, #16711935
orr.w ip, r2, r3, lsl #8
and.w r2, r4, #4278255360
and.w r3, r8, #4278255360
orr.w r5, r3, r2, lsr #8
and.w r2, lr, #16711935
and.w r3, r7, #16711935
orr.w r4, r2, r3, lsl #8
and.w r2, lr, #4278255360
and.w r3, r7, #4278255360
orr.w r8, r3, r2, lsr #8
and.w r2, r6, #16711935
and.w r3, sl, #16711935
orr.w lr, r2, r3, lsl #8
and.w r2, r6, #4278255360
and.w r3, sl, #4278255360
orr.w r7, r3, r2, lsr #8
and.w r2, r9, r1
orr.w r6, r2, r4, lsl #16
bic.w r3, r4, r1
orr.w sl, r3, r9, lsr #16
and.w r2, ip, r1
orr.w r9, r2, lr, lsl #16
bic.w r3, lr, r1
orr.w r4, r3, ip, lsr #16
and.w r2, fp, r1
orr.w ip, r2, r8, lsl #16
bic.w r3, r8, r1
orr.w lr, r3, fp, lsr #16
and.w r2, r5, r1
orr.w fp, r2, r7, lsl #16
bic.w r3, r7, r1
orr.w r8, r3, r5, lsr #16
str.w r6, [r0, #24]
str.w r9, [r0, #28]
str.w ip, [r0, #56]
str.w fp, [r0, #60]
str.w sl, [r0, #88]
str.w r4, [r0, #92]
str.w lr, [r0, #120]
str.w r8, [r0, #124]
ldmia.w sp!, {r4, r5, r6, r7, r8, r9, sl, fp, ip, lr}
bx lr
