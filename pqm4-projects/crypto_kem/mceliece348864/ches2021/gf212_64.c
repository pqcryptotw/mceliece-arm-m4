/*
  This file is for functions for field arithmetic
*/



#include "gf2_12.h"




/* input: in0, in1 in GF((2^12)^64)*/
/* output: out = in0*in1 */
void GF212_64_mul(gf *out, gf *in0, gf *in1)
{
	int i, j;

	gf prod[ 64*2-1 ];

	for (i = 0; i < 64*2-1; i++)
		prod[i] = 0;

	for (i = 0; i < 64; i++)
		for (j = 0; j < 64; j++)
			prod[i+j] ^= gf2_12_mul(in0[i], in1[j]);

	//
 
	for (i = (64-1)*2; i >= 64; i--)
	{
		prod[i - 64 +  3] ^= prod[i];
		prod[i - 64 +  1] ^= prod[i];
		prod[i - 64 +  0] ^= gf2_12_mul(prod[i], (gf)  2);
	}

	for (i = 0; i < 64; i++)
		out[i] = prod[i];
}

