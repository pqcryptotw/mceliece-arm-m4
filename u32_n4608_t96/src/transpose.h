/*
  This file is for matrix transposition
*/

#ifndef TRANSPOSE_H
#define TRANSPOSE_H

#include "stdint.h"


/* input: in, a 64x64 matrix over GF(2) */
/* output: out, transpose of in */
static inline void transpose_64x64(uint64_t * out, uint64_t * in)
{
	int i, j, s, d;

	uint64_t x, y;
	uint64_t masks[6][2] = {
	                        {0x5555555555555555, 0xAAAAAAAAAAAAAAAA},
	                        {0x3333333333333333, 0xCCCCCCCCCCCCCCCC},
	                        {0x0F0F0F0F0F0F0F0F, 0xF0F0F0F0F0F0F0F0},
	                        {0x00FF00FF00FF00FF, 0xFF00FF00FF00FF00},
	                        {0x0000FFFF0000FFFF, 0xFFFF0000FFFF0000},
	                        {0x00000000FFFFFFFF, 0xFFFFFFFF00000000}
	                       };

	for (i = 0; i < 64; i++)
		out[i] = in[i];

	for (d = 5; d >= 0; d--)
	{
		s = 1 << d;

		for (i = 0; i < 64; i += s*2)
		for (j = i; j < i+s; j++)
		{
			x = (out[j] & masks[d][0]) | ((out[j+s] & masks[d][0]) << s);
			y = ((out[j] & masks[d][1]) >> s) | (out[j+s] & masks[d][1]);

			out[j+0] = x;
			out[j+s] = y;
		}
	}
}

/* input: out, a 64x64 matrix over GF(2) */
/* output: out, transpose of out */
static inline void transpose_64x64_in_u32(uint32_t * out)
{
	int i, j, s, d;

	uint32_t x0,x1,x2,x3,y0,y1,y2,y3;
	uint32_t masks[5][2] = {
	                        {0x55555555, 0xAAAAAAAA},
	                        {0x33333333, 0xCCCCCCCC},
	                        {0x0F0F0F0F, 0xF0F0F0F0},
	                        {0x00FF00FF, 0xFF00FF00},
	                        {0x0000FFFF, 0xFFFF0000},
	                       };

	for( j = 0; j < 32 ; j++ )
	{
		x1 = out[j*2+1];
		y0 = out[(j+32)*2];

		out[j*2+1] = y0;
		out[(j+32)*2] = x1;
	}

	for (d = 4; d >= 0; d--)
	{
		s = 1 << d;

		for (i = 0; i < 32; i += s*2)
		for (j = i; j < i+s; j++)
		{
			x0 = (out[j*2]   & masks[d][0])      | ((out[(j+s)*2]   & masks[d][0]) << s);
			x1 = (out[j*2+1] & masks[d][0])      | ((out[(j+s)*2+1] & masks[d][0]) << s);
			x2 = (out[(j+32)*2]   & masks[d][0]) | ((out[(j+s+32)*2]   & masks[d][0]) << s);
			x3 = (out[(j+32)*2+1] & masks[d][0]) | ((out[(j+s+32)*2+1] & masks[d][0]) << s);

			y0 = ((out[(j)*2]   & masks[d][1]) >> s)    | (out[(j+s)*2]   & masks[d][1]);
			y1 = ((out[(j)*2+1] & masks[d][1]) >> s)    | (out[(j+s)*2+1] & masks[d][1]);
			y2 = ((out[(j+32)*2]   & masks[d][1]) >> s) | (out[(j+s+32)*2]   & masks[d][1]);
			y3 = ((out[(j+32)*2+1] & masks[d][1]) >> s) | (out[(j+s+32)*2+1] & masks[d][1]);

			out[j*2+0]      = x0;
			out[j*2+1]      = x1;
			out[(j+32)*2+0] = x2;
			out[(j+32)*2+1] = x3;
			out[(j+s)*2   ] = y0;
			out[(j+s)*2+1 ] = y1;
			out[(j+s+32)*2] = y2;
			out[(j+s+32)*2+1] = y3;
		}
	}

}



/* input: out, a 32x32 matrix over GF(2) */
/* output: out, transpose of out */
void transpose_32x32_in(uint32_t * out);



/* input: in, a 32x32 matrix over GF(2) */
/* output: out, transpose of in */
static inline void transpose_32x32(uint32_t * out, uint32_t * in)
{
	if( out != in ) {
		for (int i = 0; i < 32; i++) out[i] = in[i];
	}
	transpose_32x32_in( out );
}


/* input: out, a 16x16 matrix over GF(2) */
/* output: out, transpose of out */
static inline void transpose_16x16_in(uint16_t * out )
{
	int i, j, s, d;

	uint16_t x, y;
	uint16_t masks[4][2] = {
	                        {0x5555, 0xAAAA},
	                        {0x3333, 0xCCCC},
	                        {0x0F0F, 0xF0F0},
	                        {0x00FF, 0xFF00},
	                       };

	for (d = 3; d >= 0; d--)
	{
		s = 1 << d;

		for (i = 0; i < 16; i += s*2)
		for (j = i; j < i+s; j++)
		{
			x = (out[j] & masks[d][0]) | ((out[j+s] & masks[d][0]) << s);
			y = ((out[j] & masks[d][1]) >> s) | (out[j+s] & masks[d][1]);

			out[j+0] = x;
			out[j+s] = y;
		}
	}
}



// it seems something wrong with inlining this function for arm-none-eabi-gcc-10.2.1

/* input: out, a 16x16 matrix over GF(2) */
/* output: out, transpose of out */
void transpose_16x16_in_u32(uint32_t * out );


/* input: in, a 16x16 matrix over GF(2) */
/* output: out, transpose of in */
static inline void transpose_16x16(uint16_t * out, uint16_t * in)
{
#if 0
	if( out != in ) { for (int i = 0; i < 16; i++) out[i] = in[i]; }
	transpose_16x16_in( out );
#else
	uint32_t *in32 = (uint32_t*)in;
	uint32_t *out32 = (uint32_t*)out;
	if( out != in ) { for (int i = 0; i < 8; i++) out32[i]=in32[i]; }
	transpose_16x16_in_u32( out32 );
#endif
}


static inline
void transpose_16x32_in(uint32_t *in)
{
	uint32_t *m1 = in;
	uint32_t *m2 = in+8;
	transpose_16x16_in_u32( m1 );
	transpose_16x16_in_u32( m2 );
	uint32_t tmp[8];
	for(int i=0;i<8;i++) tmp[i] = m1[i];
	for(int i=0;i<8;i++) {
		uint32_t src0 = tmp[i];
		uint32_t src1 = m2[i];
		in[i*2]   = (src0&0xffff)|(src1<<16);
		in[i*2+1] = (src0>>16)|(src1&~0xffff);
	}
}

static inline
void transpose_32x16_in(uint32_t *in)
{
	uint32_t *m1 = in;
	uint32_t *m2 = in+8;
	uint32_t tmp[16];
	for(int i=0;i<16;i++) tmp[i] = in[i];
	for(int i=0;i<8;i++) {
		uint32_t src0 = tmp[i*2];
		uint32_t src1 = tmp[i*2+1];
		m1[i] = (src0&0xffff)|(src1<<16);
		m2[i] = (src0>>16)|(src1&~0xffff);
	}
	transpose_16x16_in_u32( m1 );
	transpose_16x16_in_u32( m2 );
}





static inline void transpose_8x8_in_u32(uint32_t * out )
{
	uint32_t x;
	uint32_t i0 = out[0];
	uint32_t i1 = out[1];


	x = (i0&0x0f0f0f0f) | ((i1&0x0f0f0f0f)<<4);
	i1 = ((i0&~0x0f0f0f0f)>>4) | (i1&~0x0f0f0f0f);
	i0 = x;

	i0 = (i0&0xcccc3333)|((i0&0x0000cccc)<<14)|((i0&0x33330000)>>14);
	i1 = (i1&0xcccc3333)|((i1&0x0000cccc)<<14)|((i1&0x33330000)>>14);

	i0 = (i0&0xaa55aa55)|((i0&0x00aa00aa)<<7)|((i0&0x55005500)>>7);
	i1 = (i1&0xaa55aa55)|((i1&0x00aa00aa)<<7)|((i1&0x55005500)>>7);

	out[0] = i0;
	out[1] = i1;
}


static inline void transpose_8x32_in(uint32_t *in)
{
	uint32_t tmp[8];
	for(int i=0;i<8;i++) tmp[i] = in[i];
	transpose_8x8_in_u32(tmp);
	transpose_8x8_in_u32(tmp+2);
	transpose_8x8_in_u32(tmp+4);
	transpose_8x8_in_u32(tmp+6);

	in[0] = (tmp[0]&0xff)|((tmp[2]&0xff)<<8)|((tmp[4]&0xff)<<16)|((tmp[6]&0xff)<<24);
	in[1] = ((tmp[0]&0xff00)>>8)|((tmp[2]&0xff00))|((tmp[4]&0xff00)<<8)|((tmp[6]&0xff00)<<16);
	in[2] = ((tmp[0]&0xff0000)>>16)|((tmp[2]&0xff0000)>>8)|((tmp[4]&0xff0000))|((tmp[6]&0xff0000)<<8);
	in[3] = ((tmp[0]&0xff000000)>>24)|((tmp[2]&0xff000000)>>16)|((tmp[4]&0xff000000)>>8)|((tmp[6]&0xff000000));
	in[4] = (tmp[1]&0xff)|((tmp[3]&0xff)<<8)|((tmp[5]&0xff)<<16)|((tmp[7]&0xff)<<24);
	in[5] = ((tmp[1]&0xff00)>>8)|((tmp[3]&0xff00))|((tmp[5]&0xff00)<<8)|((tmp[7]&0xff00)<<16);
	in[6] = ((tmp[1]&0xff0000)>>16)|((tmp[3]&0xff0000)>>8)|((tmp[5]&0xff0000))|((tmp[7]&0xff0000)<<8);
	in[7] = ((tmp[1]&0xff000000)>>24)|((tmp[3]&0xff000000)>>16)|((tmp[5]&0xff000000)>>8)|((tmp[7]&0xff000000));
}








static inline void transpose_4x16_in(uint32_t * out )
{

	uint32_t x =  (out[0]&0x00ff00ff)    |((out[1]&0x00ff00ff)<<8);
	uint32_t y = ((out[0]&0xff00ff00)>>8)| (out[1]&0xff00ff00);
	out[0] = x;
	out[1] = y;

	uint32_t invariant,comp_ur,comp_dl,temp;
	{
		temp = out[0];
		invariant = temp & 0xf0f00f0f;
		comp_ur   = temp & 0x0000f0f0;
		comp_dl   = temp & 0x0f0f0000;
		temp = invariant ^ (comp_ur<<12) ^ (comp_dl>>12);

		invariant = temp & 0xcc33cc33;
		comp_ur   = temp & 0x00cc00cc;
		comp_dl   = temp & 0x33003300;
		temp = invariant ^ (comp_ur<<6) ^ (comp_dl>>6);

		invariant = temp & 0xa5a5a5a5;
		comp_ur   = temp & 0x0a0a0a0a;
		comp_dl   = temp & 0x50505050;
		temp = invariant ^ (comp_ur<<3) ^ (comp_dl>>3);
		out[0] = temp;
	}
	{
		temp = out[1];
		invariant = temp & 0xf0f00f0f;
		comp_ur   = temp & 0x0000f0f0;
		comp_dl   = temp & 0x0f0f0000;
		temp = invariant ^ (comp_ur<<12) ^ (comp_dl>>12);

		invariant = temp & 0xcc33cc33;
		comp_ur   = temp & 0x00cc00cc;
		comp_dl   = temp & 0x33003300;
		temp = invariant ^ (comp_ur<<6) ^ (comp_dl>>6);

		invariant = temp & 0xa5a5a5a5;
		comp_ur   = temp & 0x0a0a0a0a;
		comp_dl   = temp & 0x50505050;
		temp = invariant ^ (comp_ur<<3) ^ (comp_dl>>3);
		out[1] = temp;
	}
}



static inline void transpose_16x4_in(uint32_t * out )
{
	uint32_t invariant,comp_ur,comp_dl,temp;
	{
		temp = out[0];
		invariant = temp & 0xa5a5a5a5;
		comp_ur   = temp & 0x0a0a0a0a;
		comp_dl   = temp & 0x50505050;
		temp = invariant ^ (comp_ur<<3) ^ (comp_dl>>3);

		invariant = temp & 0xcc33cc33;
		comp_ur   = temp & 0x00cc00cc;
		comp_dl   = temp & 0x33003300;
		temp = invariant ^ (comp_ur<<6) ^ (comp_dl>>6);

		invariant = temp & 0xf0f00f0f;
		comp_ur   = temp & 0x0000f0f0;
		comp_dl   = temp & 0x0f0f0000;
		temp = invariant ^ (comp_ur<<12) ^ (comp_dl>>12);

		out[0] = temp;
	}
	{
		temp = out[1];
		invariant = temp & 0xa5a5a5a5;
		comp_ur   = temp & 0x0a0a0a0a;
		comp_dl   = temp & 0x50505050;
		temp = invariant ^ (comp_ur<<3) ^ (comp_dl>>3);

		invariant = temp & 0xcc33cc33;
		comp_ur   = temp & 0x00cc00cc;
		comp_dl   = temp & 0x33003300;
		temp = invariant ^ (comp_ur<<6) ^ (comp_dl>>6);

		invariant = temp & 0xf0f00f0f;
		comp_ur   = temp & 0x0000f0f0;
		comp_dl   = temp & 0x0f0f0000;
		temp = invariant ^ (comp_ur<<12) ^ (comp_dl>>12);

		out[1] = temp;
	}


	uint32_t x = (out[0]&0x00ff00ff)|((out[1]&0x00ff00ff)<<8);
	uint32_t y = (out[1]&0xff00ff00)|((out[0]&0xff00ff00)>>8);
	out[0] = x;
	out[1] = y;

}




static inline uint32_t transpose_4x16_half(uint32_t in )
{
	uint32_t invariant,comp_ur,comp_dl,temp;
	temp = in;

	invariant = temp & 0xf00ff00f;
	comp_ur   = temp & 0x00f000f0;
	comp_dl   = temp & 0x0f000f00;
	temp = invariant ^ (comp_ur<<4) ^ (comp_dl>>4);

	invariant = temp & 0xcccc3333;
	comp_ur   = temp & 0x0000cccc;
	comp_dl   = temp & 0x33330000;
	temp = invariant ^ (comp_ur<<14) ^ (comp_dl>>14);

	invariant = temp & 0xa5a5a5a5;
	comp_ur   = temp & 0x0a0a0a0a;
	comp_dl   = temp & 0x50505050;
	temp = invariant ^ (comp_ur<<3) ^ (comp_dl>>3);

	invariant = temp & 0xff0000ff;
	comp_ur   = temp & 0x0000ff00;
	comp_dl   = temp & 0x00ff00ff;
	temp = invariant ^ (comp_ur<<8) ^ (comp_dl>>8);

	return temp;
}



static inline uint32_t transpose_16x4_half(uint32_t in )
{
	uint32_t invariant,comp_ur,comp_dl,temp;
	temp = in;

	invariant = temp & 0xff0000ff;
	comp_ur   = temp & 0x0000ff00;
	comp_dl   = temp & 0x00ff00ff;
	temp = invariant ^ (comp_ur<<8) ^ (comp_dl>>8);

	invariant = temp & 0xa5a5a5a5;
	comp_ur   = temp & 0x0a0a0a0a;
	comp_dl   = temp & 0x50505050;
	temp = invariant ^ (comp_ur<<3) ^ (comp_dl>>3);

	invariant = temp & 0xcccc3333;
	comp_ur   = temp & 0x0000cccc;
	comp_dl   = temp & 0x33330000;
	temp = invariant ^ (comp_ur<<14) ^ (comp_dl>>14);

	invariant = temp & 0xf00ff00f;
	comp_ur   = temp & 0x00f000f0;
	comp_dl   = temp & 0x0f000f00;
	temp = invariant ^ (comp_ur<<4) ^ (comp_dl>>4);

	return temp;

}





#endif

