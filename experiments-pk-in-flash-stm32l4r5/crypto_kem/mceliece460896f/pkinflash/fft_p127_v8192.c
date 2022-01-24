/*
  This file is for the Gao-Mateer FFT
  sse http://www.math.clemson.edu/~sgao/papers/GM10.pdf
*/

#include "fft.h"

#include "fft_consts.h"

#include "util.h"

#include "transpose.h"

#include <stdint.h>

#include "vec32.h"


/* input: in, a polynomial of degree <128 in bitsliced form */
/* output: in, the result of applying the radix conversions on in */
// required constants:
// const uint32_t s_2x_u32[5][4][GFBITS] = { #include "scalars_2x_u32.data" };
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

	for (j = 0; j <= 5; j++)
	{
		for (i = 0; i < GFBITS; i++)
		{
			in[2][i] ^= in[3][i];
			in[1][i] ^= in[2][i];

			if(4>=j) {
				in[1][i] ^= in[1][i]>>16;
				in[0][i] ^= in[1][i]<<16;

				in[3][i] ^= in[3][i]>>16;
				in[2][i] ^= in[3][i]<<16;

			}

			for (k = 3; k >= j; k--) {
				in[0][i] ^= (in[0][i] & mask_32[k][0])>>(1<<k);
				in[1][i] ^= (in[1][i] & mask_32[k][0])>>(1<<k);
				in[0][i] ^= (in[0][i] & mask_32[k][1])>>(1<<k);
				in[1][i] ^= (in[1][i] & mask_32[k][1])>>(1<<k);

				in[2][i] ^= (in[2][i] & mask_32[k][0])>>(1<<k);
				in[3][i] ^= (in[3][i] & mask_32[k][0])>>(1<<k);
				in[2][i] ^= (in[2][i] & mask_32[k][1])>>(1<<k);
				in[3][i] ^= (in[3][i] & mask_32[k][1])>>(1<<k);
			}
		}

		if (j < 5)
		{
			vec32_mul(in[0], in[0], s_2x_u32[j][0]);
			vec32_mul(in[1], in[1], s_2x_u32[j][1]);
			vec32_mul(in[2], in[2], s_2x_u32[j][2]);
			vec32_mul(in[3], in[3], s_2x_u32[j][3]);
		}
	}
}




/* input: in, polynomial of degree < 128 after radix conversion */
/* output: out, results by applying the FFT butterflies of first 7 layers */
static void butterflies_7_expand_32(uint32_t out_32[][ GFBITS ], uint32_t in_32[][ GFBITS ])
{
	int i, j;


	uint32_t pre_32[14][ GFBITS ];
	uint32_t buf_32[256];

	static const unsigned char reversal[128] = 
	{ 
	  0, 64, 32, 96, 16, 80, 48, 112, 
	  8, 72, 40, 104, 24, 88, 56, 120, 
	  4, 68, 36, 100, 20, 84, 52, 116, 
	  12, 76, 44, 108, 28, 92, 60, 124, 
	  2, 66, 34, 98, 18, 82, 50, 114, 
	  10, 74, 42, 106, 26, 90, 58, 122, 
	  6, 70, 38, 102, 22, 86, 54, 118, 
	  14, 78, 46, 110, 30, 94, 62, 126, 
	  1, 65, 33, 97, 17, 81, 49, 113, 
	  9, 73, 41, 105, 25, 89, 57, 121, 
	  5, 69, 37, 101, 21, 85, 53, 117, 
	  13, 77, 45, 109, 29, 93, 61, 125, 
	  3, 67, 35, 99, 19, 83, 51, 115, 
	  11, 75, 43, 107, 27, 91, 59, 123, 
	  7, 71, 39, 103, 23, 87, 55, 119, 
	  15, 79, 47, 111, 31, 95, 63, 127
	};

	static const uint16_t beta[7] = {2522, 7827, 7801, 8035, 6897, 8167, 3476};
	//

	for (i = 0; i < 7; i++)
	{
		for (j = 0; j < GFBITS; j++)
		{
			pre_32[i*2][j] = (beta[i] >> j) & 1; 
			pre_32[i*2][j] = -pre_32[i*2][j];
		}

		vec32_mul(pre_32[i*2+1], in_32[3], pre_32[i*2]);
		vec32_mul(pre_32[i*2]  , in_32[2], pre_32[i*2]);
	}

	for (i = 0; i < GFBITS; i++)
	{
		buf_32[0] = in_32[0][i];
		buf_32[1] = in_32[1][i];

		buf_32[2] = buf_32[0] ^ pre_32[0][i];      buf_32[64] = in_32[0][i] ^ pre_32[10][i];
		buf_32[3] = buf_32[1] ^ pre_32[1][i];      buf_32[65] = in_32[1][i] ^ pre_32[11][i];
		buf_32[6] = buf_32[2] ^ pre_32[2][i];      buf_32[192] = buf_32[64] ^ pre_32[12][i];
		buf_32[7] = buf_32[3] ^ pre_32[3][i];      buf_32[193] = buf_32[65] ^ pre_32[13][i];
                                              buf_32[194] = buf_32[192] ^ pre_32[0][i];
                                              buf_32[195] = buf_32[193] ^ pre_32[1][i];
		buf_32[4] = in_32[0][i] ^ pre_32[2][i];  buf_32[198] = buf_32[194] ^ pre_32[2][i];
		buf_32[5] = in_32[1][i] ^ pre_32[3][i];  buf_32[199] = buf_32[195] ^ pre_32[3][i];
		buf_32[12] = buf_32[4] ^ pre_32[4][i];      buf_32[196] = buf_32[198] ^ pre_32[0][i];
		buf_32[13] = buf_32[5] ^ pre_32[5][i];      buf_32[197] = buf_32[199] ^ pre_32[1][i];
		buf_32[14] = buf_32[12] ^ pre_32[0][i];      buf_32[204] = buf_32[196] ^ pre_32[4][i];
		buf_32[15] = buf_32[13] ^ pre_32[1][i];      buf_32[205] = buf_32[197] ^ pre_32[5][i];
		buf_32[10] = buf_32[14] ^ pre_32[2][i];      buf_32[206] = buf_32[204] ^ pre_32[0][i];
		buf_32[11] = buf_32[15] ^ pre_32[3][i];      buf_32[207] = buf_32[205] ^ pre_32[1][i];
                                              buf_32[202] = buf_32[206] ^ pre_32[2][i];
                                              buf_32[203] = buf_32[207] ^ pre_32[3][i];
		buf_32[8] = in_32[0][i] ^ pre_32[4][i];  buf_32[200] = buf_32[202] ^ pre_32[0][i];
		buf_32[9] = in_32[1][i] ^ pre_32[5][i];  buf_32[201] = buf_32[203] ^ pre_32[1][i];
		buf_32[24] = buf_32[8] ^ pre_32[6][i];     buf_32[216] = buf_32[200] ^ pre_32[6][i];
		buf_32[25] = buf_32[9] ^ pre_32[7][i];     buf_32[217] = buf_32[201] ^ pre_32[7][i];
		buf_32[26] = buf_32[24] ^ pre_32[0][i];    buf_32[218] = buf_32[216] ^ pre_32[0][i];
		buf_32[27] = buf_32[25] ^ pre_32[1][i];    buf_32[219] = buf_32[217] ^ pre_32[1][i];
		buf_32[30] = buf_32[26] ^ pre_32[2][i];    buf_32[222] = buf_32[218] ^ pre_32[2][i];
		buf_32[31] = buf_32[27] ^ pre_32[3][i];    buf_32[223] = buf_32[219] ^ pre_32[3][i];
		buf_32[28] = buf_32[30] ^ pre_32[0][i];    buf_32[220] = buf_32[222] ^ pre_32[0][i];
		buf_32[29] = buf_32[31] ^ pre_32[1][i];    buf_32[221] = buf_32[223] ^ pre_32[1][i];
		buf_32[20] = buf_32[28] ^ pre_32[4][i];    buf_32[212] = buf_32[220] ^ pre_32[4][i];
		buf_32[21] = buf_32[29] ^ pre_32[5][i];    buf_32[213] = buf_32[221] ^ pre_32[5][i];
		buf_32[22] = buf_32[20] ^ pre_32[0][i];    buf_32[214] = buf_32[212] ^ pre_32[0][i];
		buf_32[23] = buf_32[21] ^ pre_32[1][i];    buf_32[215] = buf_32[213] ^ pre_32[1][i];
		buf_32[18] = buf_32[22] ^ pre_32[2][i];     buf_32[210] = buf_32[214] ^ pre_32[2][i];
		buf_32[19] = buf_32[23] ^ pre_32[3][i];     buf_32[211] = buf_32[215] ^ pre_32[3][i];
                                              buf_32[208] = buf_32[210] ^ pre_32[0][i];
                                              buf_32[209] = buf_32[211] ^ pre_32[1][i];
		buf_32[16] = in_32[0][i] ^ pre_32[6][i];  buf_32[240] = buf_32[208] ^ pre_32[8][i];
		buf_32[17] = in_32[1][i] ^ pre_32[7][i];  buf_32[241] = buf_32[209] ^ pre_32[9][i];
		buf_32[48] = buf_32[16] ^ pre_32[8][i];     buf_32[242] = buf_32[240] ^ pre_32[0][i];
		buf_32[49] = buf_32[17] ^ pre_32[9][i];     buf_32[243] = buf_32[241] ^ pre_32[1][i];
		buf_32[50] = buf_32[48] ^ pre_32[0][i];    buf_32[246] = buf_32[242] ^ pre_32[2][i];
		buf_32[51] = buf_32[49] ^ pre_32[1][i];    buf_32[247] = buf_32[243] ^ pre_32[3][i];
		buf_32[54] = buf_32[50] ^ pre_32[2][i];    buf_32[244] = buf_32[246] ^ pre_32[0][i];
		buf_32[55] = buf_32[51] ^ pre_32[3][i];    buf_32[245] = buf_32[247] ^ pre_32[1][i];
		buf_32[52] = buf_32[54] ^ pre_32[0][i];    buf_32[252] = buf_32[244] ^ pre_32[4][i];
		buf_32[53] = buf_32[55] ^ pre_32[1][i];    buf_32[253] = buf_32[245] ^ pre_32[5][i];
		buf_32[60] = buf_32[52] ^ pre_32[4][i];    buf_32[254] = buf_32[252] ^ pre_32[0][i];
		buf_32[61] = buf_32[53] ^ pre_32[5][i];    buf_32[255] = buf_32[253] ^ pre_32[1][i];
		buf_32[62] = buf_32[60] ^ pre_32[0][i];    buf_32[250] = buf_32[254] ^ pre_32[2][i];
		buf_32[63] = buf_32[61] ^ pre_32[1][i];    buf_32[251] = buf_32[255] ^ pre_32[3][i];
		buf_32[58] = buf_32[62] ^ pre_32[2][i];    buf_32[248] = buf_32[250] ^ pre_32[0][i];
		buf_32[59] = buf_32[63] ^ pre_32[3][i];    buf_32[249] = buf_32[251] ^ pre_32[1][i];
		buf_32[56] = buf_32[58] ^ pre_32[0][i];    buf_32[232] = buf_32[248] ^ pre_32[6][i];
		buf_32[57] = buf_32[59] ^ pre_32[1][i];    buf_32[233] = buf_32[249] ^ pre_32[7][i];
		buf_32[40] = buf_32[56] ^ pre_32[6][i];    buf_32[234] = buf_32[232] ^ pre_32[0][i];
		buf_32[41] = buf_32[57] ^ pre_32[7][i];    buf_32[235] = buf_32[233] ^ pre_32[1][i];
		buf_32[42] = buf_32[40] ^ pre_32[0][i];    buf_32[238] = buf_32[234] ^ pre_32[2][i];
		buf_32[43] = buf_32[41] ^ pre_32[1][i];    buf_32[239] = buf_32[235] ^ pre_32[3][i];
		buf_32[46] = buf_32[42] ^ pre_32[2][i];    buf_32[236] = buf_32[238] ^ pre_32[0][i];
		buf_32[47] = buf_32[43] ^ pre_32[3][i];    buf_32[237] = buf_32[239] ^ pre_32[1][i];
		buf_32[44] = buf_32[46] ^ pre_32[0][i];    buf_32[228] = buf_32[236] ^ pre_32[4][i];
		buf_32[45] = buf_32[47] ^ pre_32[1][i];    buf_32[229] = buf_32[237] ^ pre_32[5][i];
		buf_32[36] = buf_32[44] ^ pre_32[4][i];    buf_32[230] = buf_32[228] ^ pre_32[0][i];
		buf_32[37] = buf_32[45] ^ pre_32[5][i];    buf_32[231] = buf_32[229] ^ pre_32[1][i];
		buf_32[38] = buf_32[36] ^ pre_32[0][i];    buf_32[226] = buf_32[230] ^ pre_32[2][i];
		buf_32[39] = buf_32[37] ^ pre_32[1][i];    buf_32[227] = buf_32[231] ^ pre_32[3][i];
		buf_32[34] = buf_32[38] ^ pre_32[2][i];    buf_32[224] = buf_32[226] ^ pre_32[0][i];
		buf_32[35] = buf_32[39] ^ pre_32[3][i];    buf_32[225] = buf_32[227] ^ pre_32[1][i];
                                              buf_32[160] = buf_32[224] ^ pre_32[10][i];
                                              buf_32[161] = buf_32[225] ^ pre_32[11][i];

		buf_32[32] = in_32[0][i] ^ pre_32[8][i]; buf_32[162] = buf_32[160] ^ pre_32[0][i];
		buf_32[33] = in_32[1][i] ^ pre_32[9][i]; buf_32[163] = buf_32[161] ^ pre_32[1][i];
		buf_32[96] = buf_32[32] ^ pre_32[10][i];    buf_32[166] = buf_32[162] ^ pre_32[2][i];
		buf_32[97] = buf_32[33] ^ pre_32[11][i];    buf_32[167] = buf_32[163] ^ pre_32[3][i];
		buf_32[98] = buf_32[96] ^ pre_32[0][i];    buf_32[164] = buf_32[166] ^ pre_32[0][i];
		buf_32[99] = buf_32[97] ^ pre_32[1][i];    buf_32[165] = buf_32[167] ^ pre_32[1][i];
		buf_32[102] = buf_32[98] ^ pre_32[2][i];    buf_32[172] = buf_32[164] ^ pre_32[4][i];
		buf_32[103] = buf_32[99] ^ pre_32[3][i];    buf_32[173] = buf_32[165] ^ pre_32[5][i];
		buf_32[100] = buf_32[102] ^ pre_32[0][i];    buf_32[174] = buf_32[172] ^ pre_32[0][i];
		buf_32[101] = buf_32[103] ^ pre_32[1][i];    buf_32[175] = buf_32[173] ^ pre_32[1][i];
		buf_32[108] = buf_32[100] ^ pre_32[4][i];    buf_32[170] = buf_32[174] ^ pre_32[2][i];
		buf_32[109] = buf_32[101] ^ pre_32[5][i];    buf_32[171] = buf_32[175] ^ pre_32[3][i];
		buf_32[110] = buf_32[108] ^ pre_32[0][i];    buf_32[168] = buf_32[170] ^ pre_32[0][i];
		buf_32[111] = buf_32[109] ^ pre_32[1][i];    buf_32[169] = buf_32[171] ^ pre_32[1][i];
		buf_32[106] = buf_32[110] ^ pre_32[2][i];    buf_32[184] = buf_32[168] ^ pre_32[6][i];
		buf_32[107] = buf_32[111] ^ pre_32[3][i];    buf_32[185] = buf_32[169] ^ pre_32[7][i];
		buf_32[104] = buf_32[106] ^ pre_32[0][i];    buf_32[186] = buf_32[184] ^ pre_32[0][i];
		buf_32[105] = buf_32[107] ^ pre_32[1][i];    buf_32[187] = buf_32[185] ^ pre_32[1][i];
		buf_32[120] = buf_32[104] ^ pre_32[6][i];    buf_32[190] = buf_32[186] ^ pre_32[2][i];
		buf_32[121] = buf_32[105] ^ pre_32[7][i];    buf_32[191] = buf_32[187] ^ pre_32[3][i];
		buf_32[122] = buf_32[120] ^ pre_32[0][i];    buf_32[188] = buf_32[190] ^ pre_32[0][i];
		buf_32[123] = buf_32[121] ^ pre_32[1][i];    buf_32[189] = buf_32[191] ^ pre_32[1][i];
		buf_32[126] = buf_32[122] ^ pre_32[2][i];    buf_32[180] = buf_32[188] ^ pre_32[4][i];
		buf_32[127] = buf_32[123] ^ pre_32[3][i];    buf_32[181] = buf_32[189] ^ pre_32[5][i];
		buf_32[124] = buf_32[126] ^ pre_32[0][i];    buf_32[182] = buf_32[180] ^ pre_32[0][i];
		buf_32[125] = buf_32[127] ^ pre_32[1][i];    buf_32[183] = buf_32[181] ^ pre_32[1][i];
		buf_32[116] = buf_32[124] ^ pre_32[4][i];    buf_32[178] = buf_32[182] ^ pre_32[2][i];
		buf_32[117] = buf_32[125] ^ pre_32[5][i];    buf_32[179] = buf_32[183] ^ pre_32[3][i];
		buf_32[118] = buf_32[116] ^ pre_32[0][i];    buf_32[176] = buf_32[178] ^ pre_32[0][i];
		buf_32[119] = buf_32[117] ^ pre_32[1][i];    buf_32[177] = buf_32[179] ^ pre_32[1][i];
		buf_32[114] = buf_32[118] ^ pre_32[2][i];    buf_32[144] = buf_32[176] ^ pre_32[8][i];
		buf_32[115] = buf_32[119] ^ pre_32[3][i];    buf_32[145] = buf_32[177] ^ pre_32[9][i];
		buf_32[112] = buf_32[114] ^ pre_32[0][i];    buf_32[146] = buf_32[144] ^ pre_32[0][i];
		buf_32[113] = buf_32[115] ^ pre_32[1][i];    buf_32[147] = buf_32[145] ^ pre_32[1][i];
		buf_32[80] = buf_32[112] ^ pre_32[8][i];    buf_32[150] = buf_32[146] ^ pre_32[2][i];
		buf_32[81] = buf_32[113] ^ pre_32[9][i];    buf_32[151] = buf_32[147] ^ pre_32[3][i];
		buf_32[82] = buf_32[80] ^ pre_32[0][i];    buf_32[148] = buf_32[150] ^ pre_32[0][i];
		buf_32[83] = buf_32[81] ^ pre_32[1][i];    buf_32[149] = buf_32[151] ^ pre_32[1][i];
		buf_32[86] = buf_32[82] ^ pre_32[2][i];    buf_32[156] = buf_32[148] ^ pre_32[4][i];
		buf_32[87] = buf_32[83] ^ pre_32[3][i];    buf_32[157] = buf_32[149] ^ pre_32[5][i];
		buf_32[84] = buf_32[86] ^ pre_32[0][i];    buf_32[158] = buf_32[156] ^ pre_32[0][i];
		buf_32[85] = buf_32[87] ^ pre_32[1][i];    buf_32[159] = buf_32[157] ^ pre_32[1][i];
		buf_32[92] = buf_32[84] ^ pre_32[4][i];    buf_32[154] = buf_32[158] ^ pre_32[2][i];
		buf_32[93] = buf_32[85] ^ pre_32[5][i];    buf_32[155] = buf_32[159] ^ pre_32[3][i];
		buf_32[94] = buf_32[92] ^ pre_32[0][i];    buf_32[152] = buf_32[154] ^ pre_32[0][i];
		buf_32[95] = buf_32[93] ^ pre_32[1][i];    buf_32[153] = buf_32[155] ^ pre_32[1][i];
		buf_32[90] = buf_32[94] ^ pre_32[2][i];    buf_32[136] = buf_32[152] ^ pre_32[6][i];
		buf_32[91] = buf_32[95] ^ pre_32[3][i];    buf_32[137] = buf_32[153] ^ pre_32[7][i];

		buf_32[88] = buf_32[90] ^ pre_32[0][i];    buf_32[138] = buf_32[136] ^ pre_32[0][i];
		buf_32[89] = buf_32[91] ^ pre_32[1][i];    buf_32[139] = buf_32[137] ^ pre_32[1][i];
		buf_32[72] = buf_32[88] ^ pre_32[6][i];    buf_32[142] = buf_32[138] ^ pre_32[2][i];
		buf_32[73] = buf_32[89] ^ pre_32[7][i];    buf_32[143] = buf_32[139] ^ pre_32[3][i];
		buf_32[74] = buf_32[72] ^ pre_32[0][i];    buf_32[140] = buf_32[142] ^ pre_32[0][i];
		buf_32[75] = buf_32[73] ^ pre_32[1][i];    buf_32[141] = buf_32[143] ^ pre_32[1][i];
		buf_32[78] = buf_32[74] ^ pre_32[2][i];    buf_32[132] = buf_32[140] ^ pre_32[4][i];
		buf_32[79] = buf_32[75] ^ pre_32[3][i];    buf_32[133] = buf_32[141] ^ pre_32[5][i];

		buf_32[76] = buf_32[78] ^ pre_32[0][i];    buf_32[134] = buf_32[132] ^ pre_32[0][i];
		buf_32[77] = buf_32[79] ^ pre_32[1][i];    buf_32[135] = buf_32[133] ^ pre_32[1][i];
		buf_32[68] = buf_32[76] ^ pre_32[4][i];    buf_32[130] = buf_32[134] ^ pre_32[2][i];
		buf_32[69] = buf_32[77] ^ pre_32[5][i];    buf_32[131] = buf_32[135] ^ pre_32[3][i];
		buf_32[70] = buf_32[68] ^ pre_32[0][i];                                     
		buf_32[71] = buf_32[69] ^ pre_32[1][i];                                     
		buf_32[66] = buf_32[70] ^ pre_32[2][i];    buf_32[128] = in_32[0][i] ^ pre_32[12][i];
		buf_32[67] = buf_32[71] ^ pre_32[3][i];    buf_32[129] = in_32[1][i] ^ pre_32[13][i];

		transpose_64x64_in_u32(buf_32 +  0 );
		transpose_64x64_in_u32(buf_32 + 128);

		for (j = 0; j < 128; j++) {
			out_32[ reversal[j]*2   ][i] = buf_32[j*2];
			out_32[ reversal[j]*2+1 ][i] = buf_32[j*2+1];
		}
	}
}



/* input: in, result of applying the radix conversions to the input polynomial */
/* output: out, evaluation results (by applying the FFT butterflies) */
// required constants:
// const uint32_t consts_u32[ 256 ][ GFBITS ] = { #include "consts_u32.data" };  // 16 KB
static void butterflies_6_u32( uint32_t values[][ GFBITS ] )
{

	uint32_t tmp[ GFBITS ];
	int i, j, k, s, b;
	int consts_ptr = 4;

	for (i = 2; i <= 7; i++)
	{
		s = 1 << i;

		for (j = 0; j < 256; j += 2*s)
		for (k = j; k < j+s; k++)
		{
			vec32_mul(tmp, values[k+s], consts_u32[ consts_ptr + (k-j) ]);

			for (b = 0; b < GFBITS; b++) values[k  ][b] ^= tmp[b];
			for (b = 0; b < GFBITS; b++) values[k+s][b] ^= values[k][b];
		}

		consts_ptr += s;
	}
}





/* input: in, polynomial in bitsliced form */
/* output: out, bitsliced results of evaluating in all the field elements */
void fft_p127_v8192(uint32_t values [][GFBITS], uint32_t poly [][GFBITS])
{
	radix_conversions_u32(poly);

	butterflies_7_expand_32(values, poly);

	butterflies_6_u32(values);
}




