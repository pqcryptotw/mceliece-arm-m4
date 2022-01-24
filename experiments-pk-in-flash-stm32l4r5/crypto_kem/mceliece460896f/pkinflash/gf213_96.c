/*
  this file is for functions for field arithmetic
*/



#include "gf2_13.h"


#define MY_BGBITS (13)
#define MY_GFMASK ((1<<13)-1)


/* multiplication in GF(2^13)[x]/x^96 + 714 x^11 + 5296 x^5 + 728 x^4 + 5581 */
void GF213_96_mul(gf *out, gf *in0, gf *in1)
{
        int i, j;

        gf prod[191];

        for (i = 0; i < 191; i++)
                prod[i] = 0;

        for (i = 0; i < 96; i++)
                for (j = 0; j < 96; j++)
                        prod[i+j] ^= gf2_13_mul(in0[i], in1[j]);

        //
 
        for (i = 190; i >= 96; i--)
        {
                prod[i - 86] ^= prod[i];
                prod[i - 87] ^= prod[i];
                prod[i - 90] ^= prod[i];
                prod[i - 96] ^= prod[i];
        }

        for (i = 0; i < 96; i++)
                out[i] = prod[i];
}

