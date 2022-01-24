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
		prod[i - 121] ^= prod[i];
		prod[i - 126] ^= prod[i];
		prod[i - 127] ^= prod[i];
		prod[i - 128] ^= prod[i];
	}

	for (i = 0; i < 128; i++)
		out[i] = prod[i];
}


