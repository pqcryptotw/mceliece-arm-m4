/*
  this file is for functions for field arithmetic
*/



#include "gf2_13.h"


#define MY_BGBITS (13)
#define MY_GFMASK ((1<<13)-1)






/* multiplication in GF(2^13)[x]/x^128 + 7682 x^5 + 2159 x^3 + 3597 */
void GF213_128_mul(gf *out, gf *in0, gf *in1)
{
	int i, j;

	gf prod[255];

	for (i = 0; i < 255; i++)
		prod[i] = 0;

	for (i = 0; i < 128; i++)
		for (j = 0; j < 128; j++)
			prod[i+j] ^= gf2_13_mul(in0[i], in1[j]);

	//
 
	for (i = 254; i >= 128; i--)
	{
		prod[i - 123] ^= gf2_13_mul(prod[i], (gf) 7682);
		prod[i - 125] ^= gf2_13_mul(prod[i], (gf) 2159);
		prod[i - 128] ^= gf2_13_mul(prod[i], (gf) 3597);
	}

	for (i = 0; i < 128; i++)
		out[i] = prod[i];
}


