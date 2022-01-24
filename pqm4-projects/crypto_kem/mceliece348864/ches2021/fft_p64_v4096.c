/*
  This file is for the Gao-Mateer FFT
  sse http://www.math.clemson.edu/~sgao/papers/GM10.pdf
*/

#include <stdint.h>

#include "fft.h"

#include "fft_consts_p64_v4096_gf212.h"

#include "vec32.h"




/* input: in, a polynomial of degree <64 in bitsliced form */
/* output: in, the result of applying the radix conversions on in */
// required constants:
// const uint32_t s_u32[5][2][GFBITS] = { #include "scalars_u32.data" };
static void radix_conversions_u32( uint32_t in[][GFBITS])
{
	int i, j, k;

	const uint32_t mask_32[4][2] = 
	{
		{0x88888888, 0x44444444},
		{0xC0C0C0C0, 0x30303030},
		{0xF000F000, 0x0F000F00},
		{0xFF000000, 0x00FF0000},
	};
//	const uint64_t s_u32[5][2][GFBITS] = { #include "scalars_u32.data" };

	for (j = 0; j <= 4; j++)
	{
		for (i = 0; i < GFBITS; i++)
		{
			in[1][i] ^= in[1][i]>>16;
			in[0][i] ^= in[1][i]<<16;
			for (k = 3; k >= j; k--) {
				in[0][i] ^= (in[0][i] & mask_32[k][0])>>(1<<k);
				in[1][i] ^= (in[1][i] & mask_32[k][0])>>(1<<k);
				in[0][i] ^= (in[0][i] & mask_32[k][1])>>(1<<k);
				in[1][i] ^= (in[1][i] & mask_32[k][1])>>(1<<k);
			}
		}
		vec32_mul(in[0],in[0],s_u32[j][0]);
		vec32_mul(in[1],in[1],s_u32[j][1]);
	}
}





/* input: in, polynomial of degree < 64 after radix conversion */
/* output: out, results, 4096 terms, by applying the FFT butterflies of first few layers */
static void broadcast_64x_32(uint32_t out_32[][ GFBITS ], uint32_t in_32[][ GFBITS ])
{
	int i, j;
	unsigned char idx;
	unsigned char pos;

	const unsigned char reversal[64] = 
	{ 
	  0, 32, 16, 48,  8, 40, 24, 56, 
	  4, 36, 20, 52, 12, 44, 28, 60, 
	  2, 34, 18, 50, 10, 42, 26, 58, 
	  6, 38, 22, 54, 14, 46, 30, 62, 
	  1, 33, 17, 49,  9, 41, 25, 57, 
	  5, 37, 21, 53, 13, 45, 29, 61, 
	  3, 35, 19, 51, 11, 43, 27, 59, 
	  7, 39, 23, 55, 15, 47, 31, 63
	};

	// boradcast
	for (j = 0; j < 64; j++) {
		pos = reversal[j]>>5;
		idx = reversal[j]&0x1f;
		for (i = 0; i < GFBITS; i++)
		{
			uint32_t vv = (in_32[pos][i] >> idx )&1;
			vv = -vv;
			out_32[j*2][i] = vv;
			out_32[j*2+1][i] = vv;
			//out[j][i] = (in[i] >> reversal[j]) & 1;
			//out[j][i] = -out[j][i];
		}
	}
}




/* input: in, result of applying the radix conversions to the input polynomial */
/* output: out, evaluation results (by applying the FFT butterflies) */
// required constants:
// const uint32_t consts_u32[ 126 ][ GFBITS ] = { #include "consts_u32.data" };  // 6 KB
static void butterflies_6_u32( uint32_t values[][ GFBITS ] )
{

	uint32_t tmp[ GFBITS ];
	int i, j, k, s, b;
	int consts_ptr = 0;

	for (i = 1; i <= 6; i++)
	{
		s = 1 << i;

		for (j = 0; j < 128; j += 2*s)
		for (k = j; k < j+s; k++)
		{
			vec32_mul(tmp, values[k+s], consts_u32[ consts_ptr + (k-j) ]);

			for (b = 0; b < GFBITS; b++) values[k  ][b] ^= tmp[b];
			for (b = 0; b < GFBITS; b++) values[k+s][b] ^= values[k][b];
		}

		consts_ptr += s;
	}
}



// adding the part contributed by x^64
// required constants:
// const uint32_t power_64_u32[ 128 ][ GFBITS ] = { #include "powers_64_u32.data" };  // 6 KB
static void add_x64_u32( uint32_t values[][ GFBITS ] )
{
	int i,b;
	for (i = 0; i < 128; i++)
	for (b = 0; b < GFBITS; b++)
		values[i][b] ^= power_64_u32[i][b];

}





/* input: in, polynomial in bitsliced form */
/* output: out, bitsliced results of evaluating in all the field elements */
void fft_p64_v4096_u32(uint32_t values [][GFBITS], uint32_t poly [][GFBITS])
{

	radix_conversions_u32(poly);

	broadcast_64x_32(values,poly);

	butterflies_6_u32(values);

	add_x64_u32(values);

}



#if 0

// wrapper for vec.

#include "vec.h"
#include "util.h"

#include "transpose.h"

void fft_p64_v4096(vec out[][ GFBITS ], vec *in)
{
	uint32_t out32[128][GFBITS];
	uint32_t in32[2][GFBITS];

	bs64_to_bs32( in32[0] , in32[1] , in , GFBITS );

	fft_p64_v4096_u32( out32 , in32 );

	for(int i=0;i<64;i++) {
		bs32_to_bs64( out[i] , out32[i*2] , out32[i*2+1] , GFBITS );
	}
}

#endif

