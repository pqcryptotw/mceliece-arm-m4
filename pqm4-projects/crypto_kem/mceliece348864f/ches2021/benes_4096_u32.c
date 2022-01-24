/*
  This file is for Benes network related functions
*/

#include "util.h"
#include "transpose.h"


/* middle layers of the benes network */
// data[0].... data[128]
static void layer_u32(uint32_t data[], const uint32_t * bits, int lgs)
{
	int i, j, s;

	uint32_t d;

	s = 1 << lgs;

	for (i = 0; i < 128; i += s*2)
	for (j = i; j < i+s; j++)
	{
		d = (data[j+0] ^ data[j+s]);
		d &= (*bits++);
		data[j+0] ^= d;
		data[j+s] ^= d;
	}
}

#define _MULTI_LAYERS_

#if defined(_MULTI_LAYERS_)
static void layer_u32_x2_inc(uint32_t data[], const uint32_t * bits0, const uint32_t *bits1, int lgs0 )
{
	int i, j, s0, s1;

	s0 = 1 << lgs0;
	s1 = s0 << 1;

	for (i = 0; i < 128; i += s1*2) {
		const uint32_t *bits0_ = bits0 + s0;
		const uint32_t *bits1_ = bits1 + s0;
		for (j = i; j < i+s0; j++) {
			uint32_t d0 = data[j+0];
			uint32_t d1 = data[j+s0];
			uint32_t d2 = data[j+s1];
			uint32_t d3 = data[j+s1+s0];
			uint32_t tmp0, tmp1;

			tmp0 = d0^d1;
			tmp1 = d2^d3;
			tmp0 &= (*bits0++);
			tmp1 &= (*bits0_++);

			d0 ^= tmp0;
			d1 ^= tmp0;
			d2 ^= tmp1;
			d3 ^= tmp1;

			tmp0 = d0^d2;
			tmp1 = d1^d3;
			tmp0 &= (*bits1++);
			tmp1 &= (*bits1_++);

			d0 ^= tmp0;
			d1 ^= tmp1;
			d2 ^= tmp0;
			d3 ^= tmp1;

			data[j+0] = d0;
			data[j+s0] = d1;
			data[j+s1] = d2;
			data[j+s1+s0] = d3;
		}
		bits0 = bits0_;
		bits1 = bits1_;
	}
}

static void layer_u32_x2_dec(uint32_t data[], const uint32_t * bits0, const uint32_t *bits1, int lgs0 )
{
	int i, j, s0, s1;

	s0 = 1 << (lgs0-1);
	s1 = s0 << 1;

	for (i = 0; i < 128; i += s1*2) {
		const uint32_t *bits0_ = bits0 + s0;
		const uint32_t *bits1_ = bits1 + s0;
		for (j = i; j < i+s0; j++) {
			uint32_t d0 = data[j+0];
			uint32_t d1 = data[j+s0];
			uint32_t d2 = data[j+s1];
			uint32_t d3 = data[j+s1+s0];
			uint32_t tmp0,tmp1;

			tmp0 = d0^d2;
			tmp1 = d1^d3;
			tmp0 &= (*bits0++);
			tmp1 &= (*bits0_++);
			d0 ^= tmp0;
			d1 ^= tmp1;
			d2 ^= tmp0;
			d3 ^= tmp1;

			tmp0 = d0^d1;
			tmp1 = d2^d3;
			tmp0 &= (*bits1++);
			tmp1 &= (*bits1_++);
			d0 ^= tmp0;
			d1 ^= tmp0;
			d2 ^= tmp1;
			d3 ^= tmp1;

			data[j+0] = d0;
			data[j+s0] = d1;
			data[j+s1] = d2;
			data[j+s1+s0] = d3;
		}
		bits0 = bits0_;
		bits1 = bits1_;
	}
}

#endif



/* input: r, sequence of bits to be permuted */
/*        bits, condition bits of the Benes network */
/*        rev, 0 for normal application; !0 for inverse */
/* output: r, permuted bits */
// 23 layers, 4096 bits(512 bytes) per layer.
void benes_4096(uint32_t * r32, const unsigned char * bits, int rev)
{
	int i, iter, inc;

	const unsigned char *bits_ptr;

	if (rev) { bits_ptr = bits + (2*12-2)*256; inc = -512; }
	else     { bits_ptr = bits;         inc = 0;    }

	uint32_t data[128];
	uint32_t conbit[64];

////////////////
	for (int j=0;j<128;j+=64) {
		for (i = 0; i < 32; i++)
		{
			data[j+ i]    = r32[j+ i*2 + 0];
			data[j+ 32+i] = r32[j+ i*2 + 1];
		}
		transpose_32x32_in( &data[j] );
		transpose_32x32_in( &data[j+32] );
	}

	for (iter = 0; iter <= 4; iter++)
	{
		for (i = 0; i < 64; i++) { conbit[i] = load4(bits_ptr); bits_ptr += 4; }
		bits_ptr += inc;

		transpose_32x32_in( conbit );
		transpose_32x32_in( &conbit[32] );

		layer_u32( data, conbit, iter);
	}
	for (int j=0;j<128;j+=64) {
		transpose_32x32_in( &data[j] );
		transpose_32x32_in( &data[j+32] );

		for (i = 0; i < 32; i++)
		{
			r32[j+ i*2 + 0] = data[j+ i];
			r32[j+ i*2 + 1] = data[j+ 32+i];
		}
	}

///////////////////////////////

#if defined(_MULTI_LAYERS_)
	for (iter = 0; iter < 6; iter+=2)
	{
		uint32_t conbit2[64];
		for (i = 0; i < 64; i++) { conbit[i] = load4(bits_ptr); bits_ptr += 4; }
		bits_ptr += inc;
		for (i = 0; i < 64; i++) { conbit2[i] = load4(bits_ptr); bits_ptr += 4; }
		bits_ptr += inc;

		layer_u32_x2_inc(r32, conbit, conbit2, iter);
	}
	for (iter = 6; iter <= 6; iter++)
#else
	for (iter = 0; iter <= 6; iter++)
#endif
	{
		for (i = 0; i < 64; i++) { conbit[i] = load4(bits_ptr); bits_ptr += 4; }
		bits_ptr += inc;

		layer_u32(r32, conbit, iter);
	}

#if defined(_MULTI_LAYERS_)
	for (iter = 5; iter >= 0; iter-=2)
	{
		uint32_t conbit2[64];
		for (i = 0; i < 64; i++) { conbit[i] = load4(bits_ptr); bits_ptr += 4; }
		bits_ptr += inc;
		for (i = 0; i < 64; i++) { conbit2[i] = load4(bits_ptr); bits_ptr += 4; }
		bits_ptr += inc;

		layer_u32_x2_dec(r32, conbit, conbit2, iter );
	}
#else
	for (iter = 5; iter >= 0; iter--)
	{
		for (i = 0; i < 64; i++) { conbit[i] = load4(bits_ptr); bits_ptr += 4; }
		bits_ptr += inc;

		layer_u32(r32, conbit, iter);
	}
#endif

////////////////////////////////

	for (int j=0;j<128;j+=64) {
		for (i = 0; i < 32; i++)
		{
			data[j+ i]    = r32[j+ i*2 + 0];
			data[j+ 32+i] = r32[j+ i*2 + 1];
		}
		transpose_32x32_in( &data[j] );
		transpose_32x32_in( &data[j+32] );
	}

	for (iter = 4; iter >=0; iter--)
	{
		for (i = 0; i < 64; i++) { conbit[i] = load4(bits_ptr); bits_ptr += 4; }

		bits_ptr += inc;

		transpose_32x32_in( conbit );
		transpose_32x32_in( &conbit[32] );

		layer_u32( data, conbit, iter);
	}
	for (int j=0;j<128;j+=64) {
		transpose_32x32_in( &data[j] );
		transpose_32x32_in( &data[j+32] );

		for (i = 0; i < 32; i++)
		{
			r32[j+ i*2 + 0] = data[j+ i];
			r32[j+ i*2 + 1] = data[j+ 32+i];
		}
	}

}



