/*
  this file is for functions for field arithmetic
*/

#include "run_config.h"

#include "gf2_12_u32.h"

#define MY_BGBITS (12)
#define MY_GFMASK ((1<<12)-1)


// gf_2_12 = gf_2[x]/x^12 + x^3 + 1


#if !defined( _M4_ASM_ )

/* field multiplication */
// the 1-bit data is stored in raidx-16.
#if defined( _M4_ASM_ )
gfu32 gfu32_2_12_mul_C( gfu32 a, gfu32 b)
#else
gfu32 gfu32_2_12_mul( gfu32 a, gfu32 b)
#endif
{
	const uint32_t mask32 = 0x11111111;
	uint32_t a0 = a&mask32; uint32_t a1 = (a>>1)&mask32;
	uint32_t b0 = b&mask32; uint32_t b1 = (b>>1)&mask32;

	union{ uint64_t v64; uint32_t v32[2]; } t0, t1;
	uint32_t t2;

	// multiplication
	t0.v64  = ((uint64_t)a0) * ((uint64_t)b0);
	t2  = a1 * b1;

	t1.v32[0] = t0.v32[1];
	t1.v32[1] = t2;

	t1.v64  += ((uint64_t)a1) * ((uint64_t)b0);
	t1.v64  &= 0x1111111111111111ULL;
	t1.v64  += ((uint64_t)a0) * ((uint64_t)b1);

	t0.v32[1] = t1.v32[0];
	t2        = t1.v32[1];

	// reduction
	const uint32_t rd_x12 = 0x1001;     // x^12 = x^3 + 1
	const uint32_t rd_x16 = rd_x12<<16; // x^16 = x^7 + x^4
	t0.v64   += ((uint64_t)(t2&mask32)) * ((uint64_t)(rd_x16));
	t0.v64   &= 0x1111111111111111ULL;
	t0.v64   += ( ((uint64_t)rd_x12) * ((uint64_t)(t0.v32[1]>>16)) );

	// output
	t0.v32[0] &= mask32;  // # 6
	t0.v32[1] &= (mask32>>16);  // # 7

   return t0.v32[0]^(t0.v32[1]<<1);
}

#endif



//////////////////////////////////


static inline
gfu32 gfu32_2_12_squ( gfu32 a )
{
	static const uint32_t mask32 = 0x11111111;
	static const uint32_t rd_x12 = 0x1001;
	static const uint32_t rd_x16 = rd_x12<<16;

	uint32_t a0 = a&mask32;
	uint32_t a1 = (a>>1)&mask32;

	union{
		uint64_t v64;
		uint32_t v32[2];
	} t0, t2;

	// multiplication
	t0.v64 = ((uint64_t)a0)*((uint64_t)a0);  // #1
	t2.v64 = ((uint64_t)a1)*((uint64_t)a1);  // #2

	// reduction
	// t2.v32[1] contains 0 or 1
	t2.v32[0] &= mask32;
	t0.v64 += ((uint64_t)t2.v32[0]) * ((uint64_t)(rd_x16));  // #2 // pure 32-bits operation
	t0.v32[1] &= mask32;  // # 1
	t0.v64 += ( ((uint64_t)rd_x12) * ((uint64_t)(t0.v32[1]>>16)) );  // # 5 : rd_x13 x x13

	// output
	t0.v32[0] &= mask32;  // # 6
	t0.v32[1] &= (mask32>>16);  // # 7

   gfu32 r;
   r = t0.v32[0]^(t0.v32[1]<<1);
   return r;
}





///////////////////////////////////



/* c = 1/a */
#if defined( _M4_ASM_ )
gfu32 gfu32_2_12_inv_C( gfu32 a )
#else
gfu32 gfu32_2_12_inv( gfu32 a )
#endif
{
#if 0
  uint32_t op = 0xd;  // 1101 = bit_reverse( 11 = 1011 )
  int t = 1;
  gfu32 r = a;
  for(int i=3;i>0;i--) {
    gfu32 tmp = r;
    for(int j=t;j>0;j--) tmp = gfu32_2_12_squ(tmp);
    r = gfu32_2_12_mul(tmp,r);
    t <<= 1;
    op >>= 1;
    if( op&1 ) {
      r = gfu32_2_12_squ( r );
      r = gfu32_2_12_mul(r,a);
      t += 1;
    }
  }
  return gfu32_2_12_squ(r);
#else
   gfu32 tmp_10 = gfu32_2_12_squ(a);
   gfu32 tmp_11 = gfu32_2_12_mul(tmp_10,a);

   gfu32 tmp_111 = gfu32_2_12_squ( tmp_11 );
   tmp_111 = gfu32_2_12_mul(tmp_111,a);

   gfu32 tmp_1111 = gfu32_2_12_squ(tmp_111);
   tmp_1111 = gfu32_2_12_mul(tmp_1111, a);

   gfu32 tmp_11110000 = gfu32_2_12_squ( tmp_1111 );
   tmp_11110000 = gfu32_2_12_squ( tmp_11110000 );
   tmp_11110000 = gfu32_2_12_squ( tmp_11110000 );
   tmp_11110000 = gfu32_2_12_squ( tmp_11110000 );
   gfu32 tmp_1x8 = gfu32_2_12_mul( tmp_11110000, tmp_1111 );

   gfu32 tmp_1x11 = gfu32_2_12_squ( tmp_1x8 );
   tmp_1x11 = gfu32_2_12_squ( tmp_1x11 );
   tmp_1x11 = gfu32_2_12_squ( tmp_1x11 );
   tmp_1x11 = gfu32_2_12_mul( tmp_1x11, tmp_111 );

   return gfu32_2_12_squ( tmp_1x11 );
#endif
}






//////////////////////////////////////////////

#if !defined( _M4_ASM_ )

static inline
uint32_t __u32_2_12_reduce( uint32_t a )
{
	static const uint32_t mask32 = 0x11111111;
	static const uint32_t rd_x12 = 0x1001;
	static const uint32_t rd_x16 = rd_x12<<16;

	union{
		uint64_t v64;
		uint32_t v32[2];
	} t0;

	t0.v32[0] = a&mask32;
	t0.v32[1] = (a>>1)&mask32;
	uint32_t a2 = (a>>2)&mask32;

	// reduction
	t0.v64 += ((uint64_t)a2) * ((uint64_t)(rd_x16));  // #2 // pure 32-bits operation
	t0.v64 += ( ((uint64_t)rd_x12) * ((uint64_t)(t0.v32[1]>>16)) );  // # 5 : rd_x13 x x13

	// output
	t0.v32[0] &= mask32;  // # 6
	t0.v32[1] &= (mask32>>16);  // # 7

   uint32_t r;
   r = t0.v32[0]^(t0.v32[1]<<1);
   return r;
}


static inline
uint32_t __u32_2_12_mul( uint32_t a, uint32_t b)
{
	static const uint32_t mask32 = 0x11111111;

	uint32_t a0 = a&mask32;
	uint32_t a1 = (a>>1)&mask32;
	uint32_t b0 = b&mask32;
	uint32_t b1 = (b>>1)&mask32;

	union{
		uint64_t v64;
		uint32_t v32[2];
	} t0, t1, t2;

	// multiplication
	t0.v64 = ((uint64_t)a0)*((uint64_t)b0);  // #1
	t2.v64 = ((uint64_t)a1)*((uint64_t)b1);  // #2

	t1.v32[0] = t0.v32[1];
	t1.v32[1] = t2.v32[0];

	t1.v64 += ((uint64_t)a1)*((uint64_t)b0);  // #3
	t1.v32[0] &= mask32;  // #4
	t1.v32[1] &= mask32;  // #5
	t1.v64 += ((uint64_t)a0)*((uint64_t)b1);  // #6

	t0.v32[1] = t1.v32[0];
	t2.v32[0] = t1.v32[1];

	// output
   uint32_t r;
   r = (t0.v32[0]&mask32) ^ ((t0.v32[1]&mask32)<<1) ^ ((t2.v32[0]&mask32)<<2) ;
   return r;
}

#if defined( _M4_ASM_ )
gfu32 gfu32_2_12_dot_C(const gfu32 *a, const gfu32 *b, unsigned len)
#else
gfu32 gfu32_2_12_dot(const gfu32 *a, const gfu32 *b, unsigned len)
#endif
{
	gfu32 accu = 0;
	for(int i=len;i>0;i--) {
		accu ^= __u32_2_12_mul(*a++,*b++);
	}
	return __u32_2_12_reduce( accu );
}


//////////////////////////////////////////////////////////////////////

#if defined( _M4_ASM_ )
void gfu32_2_12_smul_C( gfu32 * c , const gfu32 * a , gfu32 b , unsigned len )
#else
void gfu32_2_12_smul( gfu32 * c , const gfu32 * a , gfu32 b , unsigned len )
#endif
{
  for(int i=len;i>0;i--) {
    *c = gfu32_2_12_mul( *a , b );
    c++;
    a++;
  }
}


#endif
