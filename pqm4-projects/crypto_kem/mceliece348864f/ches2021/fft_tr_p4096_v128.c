/*
  This file is for transpose of the Gao-Mateer FFT
  Functions with names ending with _tr are (roughly) the transpose of the corresponding functions in fft.c
*/

#include <stdint.h>

#include "fft_tr.h"

#include "fft_consts_p64_v4096_gf212.h"

#include "vec32.h"


//#include "vec.h"





/* input: in, a polynomial of degree <128 in bitsliced form */
/* output: in, the result of applying the TRANSPOSED radix conversions on in */
// required constants:
// const uint32_t s_2x_u32[5][4][GFBITS] = { #include "scalars_2x_u32.data" };
static void radix_conversions_tr_u32(uint32_t in[][ GFBITS ])
{
	int i, j, k;

	const uint32_t mask[4][2] = 
	{
		{0x22222222, 0x44444444},
		{0x0C0C0C0C, 0x30303030},
		{0x00F000F0, 0x0F000F00},
		{0x0000FF00, 0x00FF0000},
	};

	for (j = 5; j >= 0; j--)
	{
		if (j < 5)
		{
			vec32_mul(in[0], in[0], s_2x_u32[j][0]); // scaling
			vec32_mul(in[1], in[1], s_2x_u32[j][1]); // scaling
			vec32_mul(in[2], in[2], s_2x_u32[j][2]); // scaling
			vec32_mul(in[3], in[3], s_2x_u32[j][3]); // scaling
		}

		for (i = 0; i < GFBITS; i++) {
			for (k = j; k <= 3; k++)
			{
				in[0][i] ^= (in[0][i] & mask[k][0]) << (1 << k);
				in[1][i] ^= (in[1][i] & mask[k][0]) << (1 << k);
				in[0][i] ^= (in[0][i] & mask[k][1]) << (1 << k);
				in[1][i] ^= (in[1][i] & mask[k][1]) << (1 << k);

				in[2][i] ^= (in[2][i] & mask[k][0]) << (1 << k);
				in[3][i] ^= (in[3][i] & mask[k][0]) << (1 << k);
				in[2][i] ^= (in[2][i] & mask[k][1]) << (1 << k);
				in[3][i] ^= (in[3][i] & mask[k][1]) << (1 << k);
			}
			if(j <= 4)
			{
				in[1][i] ^= (in[0][i] ) >> 16;
				in[1][i] ^= (in[1][i] ) << 16;

				in[3][i] ^= (in[2][i] ) >> 16;
				in[3][i] ^= (in[3][i] ) << 16;
			}
			//if (j <= 5)
			//{
				in[2][i] ^= in[1][i];
				in[3][i] ^= in[2][i];
			//}
		}
	}
}



#define vec_add(z, x, y) for (b = 0; b < GFBITS; b++) { z[b] = x[b]^y[b]; }



static void butterflies_tr_6_u32(uint32_t in[][ GFBITS ])
{
	int i, j, k, s, b;

	uint32_t tmp[ GFBITS ];

	// const uint32_t consts_u32[ 126 ][ GFBITS ] = { #include "consts_u32.data" };  // 6 KB

	uint32_t consts_ptr = 126;

	// butterflies

	for (i = 6; i >= 1; i--)
	{
		s = 1 << i;
		consts_ptr -= s;

		for (j = 0; j < 128; j += 2*s)
		for (k = j; k < j+s; k++)
		{
			vec_add(in[k], in[k], in[k+s]);
			vec32_mul(tmp, in[k], consts_u32[ consts_ptr + (k-j) ]);
			vec_add(in[k+s], in[k+s], tmp);
		}
	}

}





#include "transpose.h"



static inline void transpose_u32(uint32_t in[][ GFBITS ])
{
	uint64_t buf[64];
	int i,j;
	union {
	uint64_t v64;
	uint32_t v32[2];
	} t;

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

	// transpose

	for (i = 0; i < GFBITS; i++)
	{
		for (j = 0; j < 64; j++) {
			t.v32[0] = in[j*2][i];
			t.v32[1] = in[j*2+1][i];
			buf[ reversal[j] ] = t.v64;
			//buf[ reversal[j] ] = in[j][i];
		}

		//transpose_64x64(buf, buf);
		transpose_64x64_in_u32((uint32_t*)buf);

		for (j = 0; j < 64; j++) {
			t.v64 = buf[j];
			in[j*2][i] = t.v32[0];
			in[j*2+1][i] = t.v32[1];
			//in[j][i] = buf[ j ];
		}
	}
}




#define vec_cpy(z, x)  for(b=0;b<GFBITS;b++) { z[b] = x[b]; }



static void truncate_u32(uint32_t out32[][ GFBITS ], uint32_t in32[][ GFBITS ])
{
	uint32_t pre32[12][ GFBITS ];
	int i, j, b;
	// boradcast

	vec_cpy(pre32[0*2], in32[32*2]); vec_add(in32[33*2], in32[33*2], in32[32*2]);
	vec_cpy(pre32[1*2], in32[33*2]); vec_add(in32[35*2], in32[35*2], in32[33*2]);
	vec_add(pre32[0*2], pre32[0*2], in32[35*2]); vec_add(in32[34*2], in32[34*2], in32[35*2]);
	vec_cpy(pre32[2*2], in32[34*2]); vec_add(in32[38*2], in32[38*2], in32[34*2]);
	vec_add(pre32[0*2], pre32[0*2], in32[38*2]); vec_add(in32[39*2], in32[39*2], in32[38*2]);
	vec_add(pre32[1*2], pre32[1*2], in32[39*2]); vec_add(in32[37*2], in32[37*2], in32[39*2]);
	vec_add(pre32[0*2], pre32[0*2], in32[37*2]); vec_add(in32[36*2], in32[36*2], in32[37*2]);
	vec_cpy(pre32[3*2], in32[36*2]); vec_add(in32[44*2], in32[44*2], in32[36*2]);
	vec_add(pre32[0*2], pre32[0*2], in32[44*2]); vec_add(in32[45*2], in32[45*2], in32[44*2]);
	vec_add(pre32[1*2], pre32[1*2], in32[45*2]); vec_add(in32[47*2], in32[47*2], in32[45*2]);
	vec_add(pre32[0*2], pre32[0*2], in32[47*2]); vec_add(in32[46*2], in32[46*2], in32[47*2]);
	vec_add(pre32[2*2], pre32[2*2], in32[46*2]); vec_add(in32[42*2], in32[42*2], in32[46*2]);
	vec_add(pre32[0*2], pre32[0*2], in32[42*2]); vec_add(in32[43*2], in32[43*2], in32[42*2]);
	vec_add(pre32[1*2], pre32[1*2], in32[43*2]); vec_add(in32[41*2], in32[41*2], in32[43*2]);
	vec_add(pre32[0*2], pre32[0*2], in32[41*2]); vec_add(in32[40*2], in32[40*2], in32[41*2]);
	vec_cpy(pre32[4*2], in32[40*2]); vec_add(in32[56*2], in32[56*2], in32[40*2]);
	vec_add(pre32[0*2], pre32[0*2], in32[56*2]); vec_add(in32[57*2], in32[57*2], in32[56*2]);
	vec_add(pre32[1*2], pre32[1*2], in32[57*2]); vec_add(in32[59*2], in32[59*2], in32[57*2]);
	vec_add(pre32[0*2], pre32[0*2], in32[59*2]); vec_add(in32[58*2], in32[58*2], in32[59*2]);
	vec_add(pre32[2*2], pre32[2*2], in32[58*2]); vec_add(in32[62*2], in32[62*2], in32[58*2]);
	vec_add(pre32[0*2], pre32[0*2], in32[62*2]); vec_add(in32[63*2], in32[63*2], in32[62*2]);
	vec_add(pre32[1*2], pre32[1*2], in32[63*2]); vec_add(in32[61*2], in32[61*2], in32[63*2]);
	vec_add(pre32[0*2], pre32[0*2], in32[61*2]); vec_add(in32[60*2], in32[60*2], in32[61*2]);
	vec_add(pre32[3*2], pre32[3*2], in32[60*2]); vec_add(in32[52*2], in32[52*2], in32[60*2]);
	vec_add(pre32[0*2], pre32[0*2], in32[52*2]); vec_add(in32[53*2], in32[53*2], in32[52*2]);
	vec_add(pre32[1*2], pre32[1*2], in32[53*2]); vec_add(in32[55*2], in32[55*2], in32[53*2]);
	vec_add(pre32[0*2], pre32[0*2], in32[55*2]); vec_add(in32[54*2], in32[54*2], in32[55*2]);
	vec_add(pre32[2*2], pre32[2*2], in32[54*2]); vec_add(in32[50*2], in32[50*2], in32[54*2]);
	vec_add(pre32[0*2], pre32[0*2], in32[50*2]); vec_add(in32[51*2], in32[51*2], in32[50*2]);
	vec_add(pre32[1*2], pre32[1*2], in32[51*2]); vec_add(in32[49*2], in32[49*2], in32[51*2]);
	vec_add(pre32[0*2], pre32[0*2], in32[49*2]); vec_add(in32[48*2], in32[48*2], in32[49*2]);
	vec_cpy(pre32[5*2], in32[48*2]); vec_add(in32[16*2], in32[16*2], in32[48*2]);
	vec_add(pre32[0*2], pre32[0*2], in32[16*2]); vec_add(in32[17*2], in32[17*2], in32[16*2]);
	vec_add(pre32[1*2], pre32[1*2], in32[17*2]); vec_add(in32[19*2], in32[19*2], in32[17*2]);
	vec_add(pre32[0*2], pre32[0*2], in32[19*2]); vec_add(in32[18*2], in32[18*2], in32[19*2]);
	vec_add(pre32[2*2], pre32[2*2], in32[18*2]); vec_add(in32[22*2], in32[22*2], in32[18*2]);
	vec_add(pre32[0*2], pre32[0*2], in32[22*2]); vec_add(in32[23*2], in32[23*2], in32[22*2]);
	vec_add(pre32[1*2], pre32[1*2], in32[23*2]); vec_add(in32[21*2], in32[21*2], in32[23*2]);
	vec_add(pre32[0*2], pre32[0*2], in32[21*2]); vec_add(in32[20*2], in32[20*2], in32[21*2]);
	vec_add(pre32[3*2], pre32[3*2], in32[20*2]); vec_add(in32[28*2], in32[28*2], in32[20*2]);
	vec_add(pre32[0*2], pre32[0*2], in32[28*2]); vec_add(in32[29*2], in32[29*2], in32[28*2]);
	vec_add(pre32[1*2], pre32[1*2], in32[29*2]); vec_add(in32[31*2], in32[31*2], in32[29*2]);
	vec_add(pre32[0*2], pre32[0*2], in32[31*2]); vec_add(in32[30*2], in32[30*2], in32[31*2]);
	vec_add(pre32[2*2], pre32[2*2], in32[30*2]); vec_add(in32[26*2], in32[26*2], in32[30*2]);
	vec_add(pre32[0*2], pre32[0*2], in32[26*2]); vec_add(in32[27*2], in32[27*2], in32[26*2]);
	vec_add(pre32[1*2], pre32[1*2], in32[27*2]); vec_add(in32[25*2], in32[25*2], in32[27*2]);
	vec_add(pre32[0*2], pre32[0*2], in32[25*2]); vec_add(in32[24*2], in32[24*2], in32[25*2]);
	vec_add(pre32[4*2], pre32[4*2], in32[24*2]); vec_add(in32[8*2], in32[8*2], in32[24*2]);
	vec_add(pre32[0*2], pre32[0*2], in32[8*2]); vec_add(in32[9*2], in32[9*2], in32[8*2]);
	vec_add(pre32[1*2], pre32[1*2], in32[9*2]); vec_add(in32[11*2], in32[11*2], in32[9*2]);
	vec_add(pre32[0*2], pre32[0*2], in32[11*2]); vec_add(in32[10*2], in32[10*2], in32[11*2]);
	vec_add(pre32[2*2], pre32[2*2], in32[10*2]); vec_add(in32[14*2], in32[14*2], in32[10*2]);
	vec_add(pre32[0*2], pre32[0*2], in32[14*2]); vec_add(in32[15*2], in32[15*2], in32[14*2]);
	vec_add(pre32[1*2], pre32[1*2], in32[15*2]); vec_add(in32[13*2], in32[13*2], in32[15*2]);
	vec_add(pre32[0*2], pre32[0*2], in32[13*2]); vec_add(in32[12*2], in32[12*2], in32[13*2]);
	vec_add(pre32[3*2], pre32[3*2], in32[12*2]); vec_add(in32[4*2], in32[4*2], in32[12*2]);
	vec_add(pre32[0*2], pre32[0*2], in32[4*2]); vec_add(in32[5*2], in32[5*2], in32[4*2]);
	vec_add(pre32[1*2], pre32[1*2], in32[5*2]); vec_add(in32[7*2], in32[7*2], in32[5*2]);
	vec_add(pre32[0*2], pre32[0*2], in32[7*2]); vec_add(in32[6*2], in32[6*2], in32[7*2]);
	vec_add(pre32[2*2], pre32[2*2], in32[6*2]); vec_add(in32[2*2], in32[2*2], in32[6*2]);
	vec_add(pre32[0*2], pre32[0*2], in32[2*2]); vec_add(in32[3*2], in32[3*2], in32[2*2]);
	vec_add(pre32[1*2], pre32[1*2], in32[3*2]); vec_add(in32[1*2], in32[1*2], in32[3*2]);

	vec_add(pre32[0*2], pre32[0*2], in32[1*2]); vec_add(out32[0*2], in32[0*2], in32[1*2]);


	vec_cpy(pre32[0*2+1], in32[32*2+1]); vec_add(in32[33*2+1], in32[33*2+1], in32[32*2+1]);
	vec_cpy(pre32[1*2+1], in32[33*2+1]); vec_add(in32[35*2+1], in32[35*2+1], in32[33*2+1]);
	vec_add(pre32[0*2+1], pre32[0*2+1], in32[35*2+1]); vec_add(in32[34*2+1], in32[34*2+1], in32[35*2+1]);
	vec_cpy(pre32[2*2+1], in32[34*2+1]); vec_add(in32[38*2+1], in32[38*2+1], in32[34*2+1]);
	vec_add(pre32[0*2+1], pre32[0*2+1], in32[38*2+1]); vec_add(in32[39*2+1], in32[39*2+1], in32[38*2+1]);
	vec_add(pre32[1*2+1], pre32[1*2+1], in32[39*2+1]); vec_add(in32[37*2+1], in32[37*2+1], in32[39*2+1]);
	vec_add(pre32[0*2+1], pre32[0*2+1], in32[37*2+1]); vec_add(in32[36*2+1], in32[36*2+1], in32[37*2+1]);
	vec_cpy(pre32[3*2+1], in32[36*2+1]); vec_add(in32[44*2+1], in32[44*2+1], in32[36*2+1]);
	vec_add(pre32[0*2+1], pre32[0*2+1], in32[44*2+1]); vec_add(in32[45*2+1], in32[45*2+1], in32[44*2+1]);
	vec_add(pre32[1*2+1], pre32[1*2+1], in32[45*2+1]); vec_add(in32[47*2+1], in32[47*2+1], in32[45*2+1]);
	vec_add(pre32[0*2+1], pre32[0*2+1], in32[47*2+1]); vec_add(in32[46*2+1], in32[46*2+1], in32[47*2+1]);
	vec_add(pre32[2*2+1], pre32[2*2+1], in32[46*2+1]); vec_add(in32[42*2+1], in32[42*2+1], in32[46*2+1]);
	vec_add(pre32[0*2+1], pre32[0*2+1], in32[42*2+1]); vec_add(in32[43*2+1], in32[43*2+1], in32[42*2+1]);
	vec_add(pre32[1*2+1], pre32[1*2+1], in32[43*2+1]); vec_add(in32[41*2+1], in32[41*2+1], in32[43*2+1]);
	vec_add(pre32[0*2+1], pre32[0*2+1], in32[41*2+1]); vec_add(in32[40*2+1], in32[40*2+1], in32[41*2+1]);
	vec_cpy(pre32[4*2+1], in32[40*2+1]); vec_add(in32[56*2+1], in32[56*2+1], in32[40*2+1]);
	vec_add(pre32[0*2+1], pre32[0*2+1], in32[56*2+1]); vec_add(in32[57*2+1], in32[57*2+1], in32[56*2+1]);
	vec_add(pre32[1*2+1], pre32[1*2+1], in32[57*2+1]); vec_add(in32[59*2+1], in32[59*2+1], in32[57*2+1]);
	vec_add(pre32[0*2+1], pre32[0*2+1], in32[59*2+1]); vec_add(in32[58*2+1], in32[58*2+1], in32[59*2+1]);
	vec_add(pre32[2*2+1], pre32[2*2+1], in32[58*2+1]); vec_add(in32[62*2+1], in32[62*2+1], in32[58*2+1]);
	vec_add(pre32[0*2+1], pre32[0*2+1], in32[62*2+1]); vec_add(in32[63*2+1], in32[63*2+1], in32[62*2+1]);
	vec_add(pre32[1*2+1], pre32[1*2+1], in32[63*2+1]); vec_add(in32[61*2+1], in32[61*2+1], in32[63*2+1]);
	vec_add(pre32[0*2+1], pre32[0*2+1], in32[61*2+1]); vec_add(in32[60*2+1], in32[60*2+1], in32[61*2+1]);
	vec_add(pre32[3*2+1], pre32[3*2+1], in32[60*2+1]); vec_add(in32[52*2+1], in32[52*2+1], in32[60*2+1]);
	vec_add(pre32[0*2+1], pre32[0*2+1], in32[52*2+1]); vec_add(in32[53*2+1], in32[53*2+1], in32[52*2+1]);
	vec_add(pre32[1*2+1], pre32[1*2+1], in32[53*2+1]); vec_add(in32[55*2+1], in32[55*2+1], in32[53*2+1]);
	vec_add(pre32[0*2+1], pre32[0*2+1], in32[55*2+1]); vec_add(in32[54*2+1], in32[54*2+1], in32[55*2+1]);
	vec_add(pre32[2*2+1], pre32[2*2+1], in32[54*2+1]); vec_add(in32[50*2+1], in32[50*2+1], in32[54*2+1]);
	vec_add(pre32[0*2+1], pre32[0*2+1], in32[50*2+1]); vec_add(in32[51*2+1], in32[51*2+1], in32[50*2+1]);
	vec_add(pre32[1*2+1], pre32[1*2+1], in32[51*2+1]); vec_add(in32[49*2+1], in32[49*2+1], in32[51*2+1]);
	vec_add(pre32[0*2+1], pre32[0*2+1], in32[49*2+1]); vec_add(in32[48*2+1], in32[48*2+1], in32[49*2+1]);
	vec_cpy(pre32[5*2+1], in32[48*2+1]); vec_add(in32[16*2+1], in32[16*2+1], in32[48*2+1]);
	vec_add(pre32[0*2+1], pre32[0*2+1], in32[16*2+1]); vec_add(in32[17*2+1], in32[17*2+1], in32[16*2+1]);
	vec_add(pre32[1*2+1], pre32[1*2+1], in32[17*2+1]); vec_add(in32[19*2+1], in32[19*2+1], in32[17*2+1]);
	vec_add(pre32[0*2+1], pre32[0*2+1], in32[19*2+1]); vec_add(in32[18*2+1], in32[18*2+1], in32[19*2+1]);
	vec_add(pre32[2*2+1], pre32[2*2+1], in32[18*2+1]); vec_add(in32[22*2+1], in32[22*2+1], in32[18*2+1]);
	vec_add(pre32[0*2+1], pre32[0*2+1], in32[22*2+1]); vec_add(in32[23*2+1], in32[23*2+1], in32[22*2+1]);
	vec_add(pre32[1*2+1], pre32[1*2+1], in32[23*2+1]); vec_add(in32[21*2+1], in32[21*2+1], in32[23*2+1]);
	vec_add(pre32[0*2+1], pre32[0*2+1], in32[21*2+1]); vec_add(in32[20*2+1], in32[20*2+1], in32[21*2+1]);
	vec_add(pre32[3*2+1], pre32[3*2+1], in32[20*2+1]); vec_add(in32[28*2+1], in32[28*2+1], in32[20*2+1]);
	vec_add(pre32[0*2+1], pre32[0*2+1], in32[28*2+1]); vec_add(in32[29*2+1], in32[29*2+1], in32[28*2+1]);
	vec_add(pre32[1*2+1], pre32[1*2+1], in32[29*2+1]); vec_add(in32[31*2+1], in32[31*2+1], in32[29*2+1]);
	vec_add(pre32[0*2+1], pre32[0*2+1], in32[31*2+1]); vec_add(in32[30*2+1], in32[30*2+1], in32[31*2+1]);
	vec_add(pre32[2*2+1], pre32[2*2+1], in32[30*2+1]); vec_add(in32[26*2+1], in32[26*2+1], in32[30*2+1]);
	vec_add(pre32[0*2+1], pre32[0*2+1], in32[26*2+1]); vec_add(in32[27*2+1], in32[27*2+1], in32[26*2+1]);
	vec_add(pre32[1*2+1], pre32[1*2+1], in32[27*2+1]); vec_add(in32[25*2+1], in32[25*2+1], in32[27*2+1]);
	vec_add(pre32[0*2+1], pre32[0*2+1], in32[25*2+1]); vec_add(in32[24*2+1], in32[24*2+1], in32[25*2+1]);
	vec_add(pre32[4*2+1], pre32[4*2+1], in32[24*2+1]); vec_add(in32[8*2+1], in32[8*2+1], in32[24*2+1]);
	vec_add(pre32[0*2+1], pre32[0*2+1], in32[8*2+1]); vec_add(in32[9*2+1], in32[9*2+1], in32[8*2+1]);
	vec_add(pre32[1*2+1], pre32[1*2+1], in32[9*2+1]); vec_add(in32[11*2+1], in32[11*2+1], in32[9*2+1]);
	vec_add(pre32[0*2+1], pre32[0*2+1], in32[11*2+1]); vec_add(in32[10*2+1], in32[10*2+1], in32[11*2+1]);
	vec_add(pre32[2*2+1], pre32[2*2+1], in32[10*2+1]); vec_add(in32[14*2+1], in32[14*2+1], in32[10*2+1]);
	vec_add(pre32[0*2+1], pre32[0*2+1], in32[14*2+1]); vec_add(in32[15*2+1], in32[15*2+1], in32[14*2+1]);
	vec_add(pre32[1*2+1], pre32[1*2+1], in32[15*2+1]); vec_add(in32[13*2+1], in32[13*2+1], in32[15*2+1]);
	vec_add(pre32[0*2+1], pre32[0*2+1], in32[13*2+1]); vec_add(in32[12*2+1], in32[12*2+1], in32[13*2+1]);
	vec_add(pre32[3*2+1], pre32[3*2+1], in32[12*2+1]); vec_add(in32[4*2+1], in32[4*2+1], in32[12*2+1]);
	vec_add(pre32[0*2+1], pre32[0*2+1], in32[4*2+1]); vec_add(in32[5*2+1], in32[5*2+1], in32[4*2+1]);
	vec_add(pre32[1*2+1], pre32[1*2+1], in32[5*2+1]); vec_add(in32[7*2+1], in32[7*2+1], in32[5*2+1]);
	vec_add(pre32[0*2+1], pre32[0*2+1], in32[7*2+1]); vec_add(in32[6*2+1], in32[6*2+1], in32[7*2+1]);
	vec_add(pre32[2*2+1], pre32[2*2+1], in32[6*2+1]); vec_add(in32[2*2+1], in32[2*2+1], in32[6*2+1]);
	vec_add(pre32[0*2+1], pre32[0*2+1], in32[2*2+1]); vec_add(in32[3*2+1], in32[3*2+1], in32[2*2+1]);
	vec_add(pre32[1*2+1], pre32[1*2+1], in32[3*2+1]); vec_add(in32[1*2+1], in32[1*2+1], in32[3*2+1]);

	vec_add(pre32[0*2+1], pre32[0*2+1], in32[1*2+1]); vec_add(out32[0*2+1], in32[0*2+1], in32[1*2+1]);


	const uint16_t beta[6] = {8, 1300, 3408, 1354, 2341, 1154};
	uint32_t tmp[ GFBITS ];

	for (j = 0; j < GFBITS; j++) { tmp[j] = (beta[0] >> j) & 1; tmp[j] = -tmp[j]; }

	vec32_mul(out32[1*2], pre32[0*2], tmp);
	vec32_mul(out32[1*2+1], pre32[0*2+1], tmp);

	uint32_t tmp1[ GFBITS ];
	uint32_t tmp2[ GFBITS ];

	for (i = 1; i < 6; i++)
	{
		for (j = 0; j < GFBITS; j++) { tmp[j] = (beta[i] >> j) & 1; tmp[j] = -tmp[j]; }

		vec32_mul(tmp1, pre32[i*2], tmp);
		vec32_mul(tmp2, pre32[i*2+1], tmp);
		vec_add(out32[1*2], out32[1*2], tmp1);
		vec_add(out32[1*2+1], out32[1*2+1], tmp2);
	}


}







void fft_tr_p4096_v128_u32(uint32_t out32[4][GFBITS], uint32_t in32[128][GFBITS])
{
	butterflies_tr_6_u32(in32);
	transpose_u32(in32);
	truncate_u32(out32,in32);
	radix_conversions_tr_u32(out32);
}






#if 0
// wrapper for vec.

#include "util.h"

void fft_tr_p4096_v128(vec out[][GFBITS], vec in[][ GFBITS ])
{
	uint32_t in32[128][GFBITS];
	uint32_t out32[4][GFBITS];

	for(int i=0;i<64;i++) {
		bs64_to_bs32( in32[i*2] , in32[i*2+1] , in[i] , GFBITS );
	}

	fft_tr_p4096_v128_u32( out32 , in32 );

	for(int i=0;i<2;i++) {
		bs32_to_bs64( out[i] , out32[i*2+0] , out32[i*2+1] , GFBITS );
	}
}


#endif
