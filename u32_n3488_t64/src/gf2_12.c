/*
  This file is for functions for field arithmetic
*/


#include "gf2_12.h"


// gf_2_12 = gf_2[x]/x^12 + x^3 + 1


#define MY_GFBITS (12)
#define MY_GFMASK ((1<<12)-1)



gf gf2_12_mul(gf in0, gf in1)
{
	int i;

	uint32_t tmp;
	uint32_t t0;
	uint32_t t1;
	uint32_t t;

	t0 = in0;
	t1 = in1;

	tmp = t0 * (t1 & 1);

	for (i = 1; i < MY_GFBITS; i++)
		tmp ^= (t0 * (t1 & (1 << i)));

	t = tmp & 0x7FC000;
	tmp ^= t >> 9;
	tmp ^= t >> 12;

	t = tmp & 0x3000;
	tmp ^= t >> 9;
	tmp ^= t >> 12;

	return tmp & MY_GFMASK;
}

/* input: field element in */
/* return: in^2 */
static inline gf gf2_12_sq(gf in)
{
	const uint32_t B[] = {0x55555555, 0x33333333, 0x0F0F0F0F, 0x00FF00FF};

	uint32_t x = in; 
	uint32_t t;

	x = (x | (x << 8)) & B[3];
	x = (x | (x << 4)) & B[2];
	x = (x | (x << 2)) & B[1];
	x = (x | (x << 1)) & B[0];

	t = x & 0x7FC000;
	x ^= t >> 9;
	x ^= t >> 12;

	t = x & 0x3000;
	x ^= t >> 9;
	x ^= t >> 12;

	return x & MY_GFMASK;
}

gf gf2_12_inv(gf in)
{
	gf tmp_11;
	gf tmp_1111;

	gf out = in;

	out = gf2_12_sq(out);
	tmp_11 = gf2_12_mul(out, in); // 11

	out = gf2_12_sq(tmp_11);
	out = gf2_12_sq(out);
	tmp_1111 = gf2_12_mul(out, tmp_11); // 1111

	out = gf2_12_sq(tmp_1111);
	out = gf2_12_sq(out);
	out = gf2_12_sq(out);
	out = gf2_12_sq(out);
	out = gf2_12_mul(out, tmp_1111); // 11111111

	out = gf2_12_sq(out);
	out = gf2_12_sq(out);
	out = gf2_12_mul(out, tmp_11); // 1111111111

	out = gf2_12_sq(out);
	out = gf2_12_mul(out, in); // 11111111111

	return gf2_12_sq(out); // 111111111110
}

/* input: field element den, num */
/* return: (num/den) */
gf gf2_12_frac(gf den, gf num)
{
	return gf2_12_mul(gf2_12_inv(den), num);
}


