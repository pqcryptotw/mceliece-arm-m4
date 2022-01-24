/*
  this file is for functions for field arithmetic
*/

#include "run_config.h"

#include "gf2_13_u32.h"

#define MY_BGBITS (13)
#define MY_GFMASK ((1<<13)-1)


// gf_2_13 = gf_2[x]/x^13 + x^4 + x^3 + x + 1

#if !defined( _M4_ASM_ )

/* field multiplication */
// the 1-bit data is stored in raidx-16.
// ~47 cycles for ASM code.
#if defined( _M4_ASM_ )
gfu32 gfu32_2_13_mul_C( gfu32 a, gfu32 b)
#else
gfu32 gfu32_2_13_mul( gfu32 a, gfu32 b)
#endif
{
	static const uint32_t mask32 = 0x11111111;
	static const uint32_t rd_x13 = 0x11011;
	static const uint32_t rd_x16 = rd_x13<<12;

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

	// reduction
	// t2.v32[1] contains 0 or 1
	t0.v32[1] += t2.v32[1] * (rd_x16);  // #2 // pure 32-bits operation
	t0.v32[1] &= mask32;  // # 1
	t2.v32[0] &= mask32;  // # 3
	t0.v64 += ((uint64_t)t2.v32[0]) * ((uint64_t)(rd_x16)); // # 4

	t0.v64 += ( ((uint64_t)rd_x13) * ((uint64_t)(t0.v32[1]>>20)) );  // # 5 : rd_x13 x x13

	// output
	t0.v32[0] &= mask32;  // # 6
	t0.v32[1] &= (mask32>>12);  // # 7

   gfu32 r;
   r = t0.v32[0]^(t0.v32[1]<<1);
   return r;
}


#endif


//////////////////////////////////


static inline
gfu32 gfu32_2_13_squ( gfu32 a )
{
	static const uint32_t mask32 = 0x11111111;
	static const uint32_t rd_x13 = 0x11011;
	static const uint32_t rd_x16 = rd_x13<<12;

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
	t0.v32[1] &= mask32;  // # 1
	t0.v32[1] += t2.v32[1] * (rd_x16);  // #2 // pure 32-bits operation

	t2.v32[0] &= mask32;  // # 3
	t0.v64 += ((uint64_t)t2.v32[0]) * ((uint64_t)(rd_x16)); // # 4

//	uint32_t x13 = t0.v32[1] >> 20;  // nop
	t0.v64 += ( ((uint64_t)rd_x13) * ((uint64_t)(t0.v32[1]>>20)) );  // # 5 : rd_x13 x x13

	// output
	t0.v32[0] &= mask32;  // # 6
	t0.v32[1] &= (mask32>>12);  // # 7

   gfu32 r;
   r = t0.v32[0]^(t0.v32[1]<<1);
   return r;
}





///////////////////////////////////



/* c = 1/a */
#if defined( _M4_ASM_ )
gfu32 gfu32_2_13_inv_C( gfu32 a )
#else
gfu32 gfu32_2_13_inv( gfu32 a )
#endif
{
#if 0
  uint32_t op = 0x3;  // 0011 = bit_reverse( 12 = 1100 )
  int t = 1;
  gfu32 r = a;
  for(int i=3;i>0;i--) {
    gfu32 tmp = r;
    for(int j=t;j>0;j--) tmp = gfu32_2_13_squ(tmp);
    r = gfu32_2_13_mul(tmp,r);
    t <<= 1;
    op >>= 1;
    if( op&1 ) {
      r = gfu32_2_13_squ( r );
      r = gfu32_2_13_mul(r,a);
      t += 1;
    }
  }
  return gfu32_2_13_squ(r);
#else
   gfu32 tmp_10 = gfu32_2_13_squ(a);
   gfu32 tmp_11 = gfu32_2_13_mul(tmp_10,a);

   gfu32 tmp_1100 = gfu32_2_13_squ(tmp_11);
   tmp_1100 = gfu32_2_13_squ(tmp_1100);

   gfu32 tmp_1111 = gfu32_2_13_mul(tmp_1100, tmp_11);

   gfu32 tmp_11110000 = gfu32_2_13_squ( tmp_1111 );
   tmp_11110000 = gfu32_2_13_squ( tmp_11110000 );
   tmp_11110000 = gfu32_2_13_squ( tmp_11110000 );
   tmp_11110000 = gfu32_2_13_squ( tmp_11110000 );

   gfu32 tmp_11111111 = gfu32_2_13_mul( tmp_11110000, tmp_1111 );


   gfu32 tmp_1x8_0000 = gfu32_2_13_squ( tmp_11111111 );
   tmp_1x8_0000 = gfu32_2_13_squ( tmp_1x8_0000 );
   tmp_1x8_0000 = gfu32_2_13_squ( tmp_1x8_0000 );
   tmp_1x8_0000 = gfu32_2_13_squ( tmp_1x8_0000 );

   gfu32 tmp_1x12 = gfu32_2_13_mul( tmp_1x8_0000 , tmp_1111 );

   return gfu32_2_13_squ( tmp_1x12 );
#endif
}






//////////////////////////////////////////////


#if !defined( _M4_ASM_ )

static inline
uint32_t __u32_2_13_reduce( uint32_t a )
{
	static const uint32_t mask32 = 0x11111111;
	static const uint32_t rd_x13 = 0x11011;
	static const uint32_t rd_x16 = rd_x13<<12;

	union{
		uint64_t v64;
		uint32_t v32[2];
	} t0, t2;

	t0.v32[0] = a&mask32;
	t0.v32[1] = (a>>1)&mask32;
	t2.v32[0] = (a>>2)&mask32;
	t2.v32[1] = (a>>3)&mask32;

	// reduction
	// t2.v32[1] contains 0 or 1
	t0.v32[1] += t2.v32[1] * (rd_x16);  // #2 // pure 32-bits operation
	t0.v64 += ((uint64_t)t2.v32[0]) * ((uint64_t)(rd_x16)); // # 4
	t0.v64 += ( ((uint64_t)rd_x13) * ((uint64_t)(t0.v32[1]>>20)) );  // # 5 : rd_x13 x x13

	// output
	t0.v32[0] &= mask32;  // # 6
	t0.v32[1] &= (mask32>>12);  // # 7

   uint32_t r;
   r = t0.v32[0]^(t0.v32[1]<<1);
   return r;
}


static inline
uint32_t __u32_2_13_mul( uint32_t a, uint32_t b)
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
   r = (t0.v32[0]&mask32) ^ ((t0.v32[1]&mask32)<<1) ^ ((t2.v32[0]&mask32)<<2) ^ ((t2.v32[1]&mask32)<<3);
   return r;
}

#if defined( _M4_ASM_ )
gfu32 gfu32_2_13_dot_C(const gfu32 *a, const gfu32 *b, unsigned len)
#else
gfu32 gfu32_2_13_dot(const gfu32 *a, const gfu32 *b, unsigned len)
#endif
{
	gfu32 accu = 0;
	for(int i=len;i>0;i--) {
		accu ^= __u32_2_13_mul(*a++,*b++);
	}
	return __u32_2_13_reduce( accu );
}



#if defined( _M4_ASM_ )
void gfu32_2_13_smul_C( gfu32 * c , const gfu32 * a , gfu32 b , unsigned len )
#else
void gfu32_2_13_smul( gfu32 * c , const gfu32 * a , gfu32 b , unsigned len )
#endif
{
  for(int i=len;i>0;i--) {
    *c = gfu32_2_13_mul( *a , b );
    c++;
    a++;
  }
}



#endif
