/*
  This file is for transpose of the Gao-Mateer FFT
  Functions with names ending with _tr are (roughly) the transpose of the corresponding functions in fft.c
*/

#include "fft_tr.h"

#include "fft_consts.h"

#include "transpose.h"

#include "util.h"

#include <stdint.h>

#include "vec32.h"



/* input: in, a polynomial of degree <256 in bitsliced form */
/* output: in, the result of applying the TRANSPOSED radix conversions on in */
// required constants:
// const uint32_t s_4x_u32[5][4][GFBITS] = { #include "scalars_4x_u32.data" };
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

	for (j = 6; j >= 0; j--)
	{
		if (j < 6)
		{
			vec32_mul(in[0], in[0], s_4x_u32[j][0]); // scaling
			vec32_mul(in[1], in[1], s_4x_u32[j][1]); // scaling
			vec32_mul(in[2], in[2], s_4x_u32[j][2]); // scaling
			vec32_mul(in[3], in[3], s_4x_u32[j][3]); // scaling
			vec32_mul(in[4], in[4], s_4x_u32[j][4]); // scaling
			vec32_mul(in[5], in[5], s_4x_u32[j][5]); // scaling
			vec32_mul(in[6], in[6], s_4x_u32[j][6]); // scaling
			vec32_mul(in[7], in[7], s_4x_u32[j][7]); // scaling
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

				in[4][i] ^= (in[4][i] & mask[k][0]) << (1 << k);
				in[5][i] ^= (in[5][i] & mask[k][0]) << (1 << k);
				in[4][i] ^= (in[4][i] & mask[k][1]) << (1 << k);
				in[5][i] ^= (in[5][i] & mask[k][1]) << (1 << k);

				in[6][i] ^= (in[6][i] & mask[k][0]) << (1 << k);
				in[7][i] ^= (in[7][i] & mask[k][0]) << (1 << k);
				in[6][i] ^= (in[6][i] & mask[k][1]) << (1 << k);
				in[7][i] ^= (in[7][i] & mask[k][1]) << (1 << k);
			}
			if(j <= 4)
			{
				in[1][i] ^= (in[0][i] ) >> 16;
				in[1][i] ^= (in[1][i] ) << 16;

				in[3][i] ^= (in[2][i] ) >> 16;
				in[3][i] ^= (in[3][i] ) << 16;

				in[5][i] ^= (in[4][i] ) >> 16;
				in[5][i] ^= (in[5][i] ) << 16;

				in[7][i] ^= (in[6][i] ) >> 16;
				in[7][i] ^= (in[7][i] ) << 16;
			}
			if (j <= 5)
			{
				in[2][i] ^= in[1][i];
				in[3][i] ^= in[2][i];

				in[6][i] ^= in[5][i];
				in[7][i] ^= in[6][i];
			}

			in[4][i] ^= in[2][i];
			in[5][i] ^= in[3][i];

			in[6][i] ^= in[4][i];
			in[7][i] ^= in[5][i];
		}
	}
}



// transposed butterflies
// input: 8192 terms, bitslice form
// required constants:
// const uint32_t consts_u32[256][GFBITS] = { #include "consts_u32.data" };
static void butterflies_tr_7_u32( uint32_t in[][ GFBITS ] )
{
	uint32_t tmp[ GFBITS ];

	int i, j, k, s, b;
	int consts_ptr = 256;

	for (i = 7; i >= 1; i--)
	{
		s = 1 << i;
		consts_ptr -= s;

		for (j = 0; j < 256; j += 2*s)
		for (k = j; k < j+s; k++)
		{
			for (b = 0; b < GFBITS; b++) in[k][b] ^= in[k+s][b];

			vec32_mul(tmp, in[k], consts_u32[ consts_ptr + (k-j) ]);

			for (b = 0; b < GFBITS; b++) in[k+s][b] ^= tmp[b];
		}
	}
}



// transposed butterflies
// input: 8192 terms, bitslice form
// out: 256 terms, bitslice form

static void butterflies_tr_u32(uint32_t out_32[][ GFBITS ], uint32_t in_32[][ GFBITS ])
{
	int i, j, k, b;

	uint32_t pre[12][2][ GFBITS ];
	uint32_t buf[2][128];
	uint32_t tmp[ GFBITS ];

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

	static const uint16_t beta[6] = {5246, 5306, 6039, 6685, 4905, 6755};

	//

	for (i = 0; i < GFBITS; i++)
	{
		for (k = 0; k < 128; k++) {
			(&buf[0][0])[ k*2   ] = in_32[ reversal[k]*2 ][i];
			(&buf[0][0])[ k*2+1 ] = in_32[ reversal[k]*2+1 ][i];
		}

		transpose_64x64_in_u32(buf[0]);
		transpose_64x64_in_u32(buf[1]);

		for (k = 0; k < 2; k++)
		{
			pre[0][k][i] = buf[k][64]; buf[k][66] ^= buf[k][64];
			pre[1][k][i] = buf[k][65]; buf[k][67] ^= buf[k][65];
			pre[2][k][i] = buf[k][66]; buf[k][70] ^= buf[k][66];
			pre[3][k][i] = buf[k][67]; buf[k][71] ^= buf[k][67];
			pre[0][k][i] ^= buf[k][70]; buf[k][68] ^= buf[k][70];
			pre[1][k][i] ^= buf[k][71]; buf[k][69] ^= buf[k][71];
			pre[4][k][i] = buf[k][68]; buf[k][76] ^= buf[k][68];
			pre[5][k][i] = buf[k][69]; buf[k][77] ^= buf[k][69];

			pre[0][k][i] ^= buf[k][76]; buf[k][78] ^= buf[k][76];
			pre[1][k][i] ^= buf[k][77]; buf[k][79] ^= buf[k][77];
			pre[2][k][i] ^= buf[k][78]; buf[k][74] ^= buf[k][78];
			pre[3][k][i] ^= buf[k][79]; buf[k][75] ^= buf[k][79];
			pre[0][k][i] ^= buf[k][74]; buf[k][72] ^= buf[k][74];
			pre[1][k][i] ^= buf[k][75]; buf[k][73] ^= buf[k][75];
			pre[6][k][i] = buf[k][72]; buf[k][88] ^= buf[k][72];
			pre[7][k][i] = buf[k][73]; buf[k][89] ^= buf[k][73];

			pre[0][k][i] ^= buf[k][88]; buf[k][90] ^= buf[k][88];
			pre[1][k][i] ^= buf[k][89]; buf[k][91] ^= buf[k][89];
			pre[2][k][i] ^= buf[k][90]; buf[k][94] ^= buf[k][90];
			pre[3][k][i] ^= buf[k][91]; buf[k][95] ^= buf[k][91];
			pre[0][k][i] ^= buf[k][94]; buf[k][92] ^= buf[k][94];
			pre[1][k][i] ^= buf[k][95]; buf[k][93] ^= buf[k][95];
			pre[4][k][i] ^= buf[k][92]; buf[k][84] ^= buf[k][92];
			pre[5][k][i] ^= buf[k][93]; buf[k][85] ^= buf[k][93];

			pre[0][k][i] ^= buf[k][84]; buf[k][86] ^= buf[k][84];
			pre[1][k][i] ^= buf[k][85]; buf[k][87] ^= buf[k][85];
			pre[2][k][i] ^= buf[k][86]; buf[k][82] ^= buf[k][86];
			pre[3][k][i] ^= buf[k][87]; buf[k][83] ^= buf[k][87];
			pre[0][k][i] ^= buf[k][82]; buf[k][80] ^= buf[k][82];
			pre[1][k][i] ^= buf[k][83]; buf[k][81] ^= buf[k][83];
			pre[8][k][i] = buf[k][80]; buf[k][112] ^= buf[k][80];
			pre[9][k][i] = buf[k][81]; buf[k][113] ^= buf[k][81];

			pre[0][k][i] ^= buf[k][112]; buf[k][114] ^= buf[k][112];
			pre[1][k][i] ^= buf[k][113]; buf[k][115] ^= buf[k][113];
			pre[2][k][i] ^= buf[k][114]; buf[k][118] ^= buf[k][114];
			pre[3][k][i] ^= buf[k][115]; buf[k][119] ^= buf[k][115];
			pre[0][k][i] ^= buf[k][118]; buf[k][116] ^= buf[k][118];
			pre[1][k][i] ^= buf[k][119]; buf[k][117] ^= buf[k][119];
			pre[4][k][i] ^= buf[k][116]; buf[k][124] ^= buf[k][116];
			pre[5][k][i] ^= buf[k][117]; buf[k][125] ^= buf[k][117];

			pre[0][k][i] ^= buf[k][124]; buf[k][126] ^= buf[k][124];
			pre[1][k][i] ^= buf[k][125]; buf[k][127] ^= buf[k][125];
			pre[2][k][i] ^= buf[k][126]; buf[k][122] ^= buf[k][126];
			pre[3][k][i] ^= buf[k][127]; buf[k][123] ^= buf[k][127];
			pre[0][k][i] ^= buf[k][122]; buf[k][120] ^= buf[k][122];
			pre[1][k][i] ^= buf[k][123]; buf[k][121] ^= buf[k][123];
			pre[6][k][i] ^= buf[k][120]; buf[k][104] ^= buf[k][120];
			pre[7][k][i] ^= buf[k][121]; buf[k][105] ^= buf[k][121];

			pre[0][k][i] ^= buf[k][104]; buf[k][106] ^= buf[k][104];
			pre[1][k][i] ^= buf[k][105]; buf[k][107] ^= buf[k][105];
			pre[2][k][i] ^= buf[k][106]; buf[k][110] ^= buf[k][106];
			pre[3][k][i] ^= buf[k][107]; buf[k][111] ^= buf[k][107];
			pre[0][k][i] ^= buf[k][110]; buf[k][108] ^= buf[k][110];
			pre[1][k][i] ^= buf[k][111]; buf[k][109] ^= buf[k][111];
			pre[4][k][i] ^= buf[k][108]; buf[k][100] ^= buf[k][108];
			pre[5][k][i] ^= buf[k][109]; buf[k][101] ^= buf[k][109];

			pre[0][k][i] ^= buf[k][100]; buf[k][102] ^= buf[k][100];
			pre[1][k][i] ^= buf[k][101]; buf[k][103] ^= buf[k][101];
			pre[2][k][i] ^= buf[k][102]; buf[k][98] ^= buf[k][102];
			pre[3][k][i] ^= buf[k][103]; buf[k][99] ^= buf[k][103];
			pre[0][k][i] ^= buf[k][98]; buf[k][96] ^= buf[k][98];
			pre[1][k][i] ^= buf[k][99]; buf[k][97] ^= buf[k][99];
			pre[10][k][i] = buf[k][96]; buf[k][32] ^= buf[k][96];
			pre[11][k][i] = buf[k][97]; buf[k][33] ^= buf[k][97];

			pre[0][k][i] ^= buf[k][32]; buf[k][34] ^= buf[k][32];
			pre[1][k][i] ^= buf[k][33]; buf[k][35] ^= buf[k][33];
			pre[2][k][i] ^= buf[k][34]; buf[k][38] ^= buf[k][34];
			pre[3][k][i] ^= buf[k][35]; buf[k][39] ^= buf[k][35];
			pre[0][k][i] ^= buf[k][38]; buf[k][36] ^= buf[k][38];
			pre[1][k][i] ^= buf[k][39]; buf[k][37] ^= buf[k][39];
			pre[4][k][i] ^= buf[k][36]; buf[k][44] ^= buf[k][36];
			pre[5][k][i] ^= buf[k][37]; buf[k][45] ^= buf[k][37];

			pre[0][k][i] ^= buf[k][44]; buf[k][46] ^= buf[k][44];
			pre[1][k][i] ^= buf[k][45]; buf[k][47] ^= buf[k][45];
			pre[2][k][i] ^= buf[k][46]; buf[k][42] ^= buf[k][46];
			pre[3][k][i] ^= buf[k][47]; buf[k][43] ^= buf[k][47];
			pre[0][k][i] ^= buf[k][42]; buf[k][40] ^= buf[k][42];
			pre[1][k][i] ^= buf[k][43]; buf[k][41] ^= buf[k][43];
			pre[6][k][i] ^= buf[k][40]; buf[k][56] ^= buf[k][40];
			pre[7][k][i] ^= buf[k][41]; buf[k][57] ^= buf[k][41];

			pre[0][k][i] ^= buf[k][56]; buf[k][58] ^= buf[k][56];
			pre[1][k][i] ^= buf[k][57]; buf[k][59] ^= buf[k][57];
			pre[2][k][i] ^= buf[k][58]; buf[k][62] ^= buf[k][58];
			pre[3][k][i] ^= buf[k][59]; buf[k][63] ^= buf[k][59];
			pre[0][k][i] ^= buf[k][62]; buf[k][60] ^= buf[k][62];
			pre[1][k][i] ^= buf[k][63]; buf[k][61] ^= buf[k][63];
			pre[4][k][i] ^= buf[k][60]; buf[k][52] ^= buf[k][60];
			pre[5][k][i] ^= buf[k][61]; buf[k][53] ^= buf[k][61];

			pre[0][k][i] ^= buf[k][52]; buf[k][54] ^= buf[k][52];
			pre[1][k][i] ^= buf[k][53]; buf[k][55] ^= buf[k][53];
			pre[2][k][i] ^= buf[k][54]; buf[k][50] ^= buf[k][54];
			pre[3][k][i] ^= buf[k][55]; buf[k][51] ^= buf[k][55];
			pre[0][k][i] ^= buf[k][50]; buf[k][48] ^= buf[k][50];
			pre[1][k][i] ^= buf[k][51]; buf[k][49] ^= buf[k][51];
			pre[8][k][i] ^= buf[k][48]; buf[k][16] ^= buf[k][48];
			pre[9][k][i] ^= buf[k][49]; buf[k][17] ^= buf[k][49];

			pre[0][k][i] ^= buf[k][16]; buf[k][18] ^= buf[k][16];
			pre[1][k][i] ^= buf[k][17]; buf[k][19] ^= buf[k][17];
			pre[2][k][i] ^= buf[k][18]; buf[k][22] ^= buf[k][18];
			pre[3][k][i] ^= buf[k][19]; buf[k][23] ^= buf[k][19];
			pre[0][k][i] ^= buf[k][22]; buf[k][20] ^= buf[k][22];
			pre[1][k][i] ^= buf[k][23]; buf[k][21] ^= buf[k][23];
			pre[4][k][i] ^= buf[k][20]; buf[k][28] ^= buf[k][20];
			pre[5][k][i] ^= buf[k][21]; buf[k][29] ^= buf[k][21];

			pre[0][k][i] ^= buf[k][28]; buf[k][30] ^= buf[k][28];
			pre[1][k][i] ^= buf[k][29]; buf[k][31] ^= buf[k][29];
			pre[2][k][i] ^= buf[k][30]; buf[k][26] ^= buf[k][30];
			pre[3][k][i] ^= buf[k][31]; buf[k][27] ^= buf[k][31];
			pre[0][k][i] ^= buf[k][26]; buf[k][24] ^= buf[k][26];
			pre[1][k][i] ^= buf[k][27]; buf[k][25] ^= buf[k][27];
			pre[6][k][i] ^= buf[k][24]; buf[k][8] ^= buf[k][24];
			pre[7][k][i] ^= buf[k][25]; buf[k][9] ^= buf[k][25];

			pre[0][k][i] ^= buf[k][8]; buf[k][10] ^= buf[k][8];
			pre[1][k][i] ^= buf[k][9]; buf[k][11] ^= buf[k][9];
			pre[2][k][i] ^= buf[k][10]; buf[k][14] ^= buf[k][10];
			pre[3][k][i] ^= buf[k][11]; buf[k][15] ^= buf[k][11];
			pre[0][k][i] ^= buf[k][14]; buf[k][12] ^= buf[k][14];
			pre[1][k][i] ^= buf[k][15]; buf[k][13] ^= buf[k][15];
			pre[4][k][i] ^= buf[k][12]; buf[k][4] ^= buf[k][12];
			pre[5][k][i] ^= buf[k][13]; buf[k][5] ^= buf[k][13];

			pre[0][k][i] ^= buf[k][4]; buf[k][6] ^= buf[k][4];
			pre[1][k][i] ^= buf[k][5]; buf[k][7] ^= buf[k][5];
			pre[2][k][i] ^= buf[k][6]; buf[k][2] ^= buf[k][6];
			pre[3][k][i] ^= buf[k][7]; buf[k][3] ^= buf[k][7];
			pre[0][k][i] ^= buf[k][2]; out_32[k*2][  i] = buf[k][0] ^ buf[k][2];
			pre[1][k][i] ^= buf[k][3]; out_32[k*2+1][i] = buf[k][1] ^ buf[k][3];
		}
	}

	for (j = 0; j < GFBITS; j++) tmp[j] = -((uint32_t)((beta[0] >> j) & 1));

	vec32_mul(out_32[4], pre[0][0], tmp);
	vec32_mul(out_32[5], pre[1][0], tmp);
	vec32_mul(out_32[6], pre[0][1], tmp);
	vec32_mul(out_32[7], pre[1][1], tmp);

	for (i = 1; i < 6; i++)
	{
		for (j = 0; j < GFBITS; j++) tmp[j] = -((uint32_t)((beta[i] >> j) & 1));

		vec32_mul(pre[i*2  ][0], pre[i*2  ][0], tmp);
		vec32_mul(pre[i*2+1][0], pre[i*2+1][0], tmp);
		vec32_mul(pre[i*2  ][1], pre[i*2  ][1], tmp);
		vec32_mul(pre[i*2+1][1], pre[i*2+1][1], tmp);

		for (b = 0; b < GFBITS; b++) 
		{
			out_32[4][b] ^= pre[i*2  ][0][b];
			out_32[5][b] ^= pre[i*2+1][0][b];
			out_32[6][b] ^= pre[i*2  ][1][b];
			out_32[7][b] ^= pre[i*2+1][1][b];
		}
	}
}





void fft_tr_p8192_v256(uint32_t out_32[][GFBITS], uint32_t in_32[][ GFBITS ])
{

	butterflies_tr_7_u32( in_32 );

	butterflies_tr_u32(out_32, in_32);

	radix_conversions_tr_u32(out_32);

}

