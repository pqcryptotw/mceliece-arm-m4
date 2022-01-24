/*
  this file is for functions for field arithmetic
*/

#include "run_config.h"

#include "gf2_13.h"


#define MY_GFBITS (13)
#define MY_GFMASK ((1<<13)-1)


// gf_2_13 = gf_2[x]/x^13 + x^4 + x^3 + x + 1



/* field multiplication */
// 71 m4 cycles
gf gf2_13_mul(gf in0, gf in1)
{
	int i;

	uint32_t tmp;
	uint32_t t0;
	uint32_t t1;
	uint32_t t;

	t0 = in0;
	t1 = in1;

	tmp = t0 * (t1 & 1);

	for (i = 1; i < MY_GFBITS; i++)
		tmp ^= (t0 * (t1 & (1 << i)));

	//

	t = tmp & 0x1FF0000;
	tmp ^= (t >> 9) ^ (t >> 10) ^ (t >> 12) ^ (t >> 13);

	t = tmp & 0x000E000;
	tmp ^= (t >> 9) ^ (t >> 10) ^ (t >> 12) ^ (t >> 13);

	return tmp & MY_GFMASK;
}



///////////////////////////////////////



static inline gf gf_sq(gf in)
{
	const uint32_t irr = 0x201b;
	const uint32_t x_22 = 0x161b;
	const uint32_t x_24 = 0x185a;
	uint32_t r = 0;

	uint32_t t = in;
	r = (t&1);  t >>= 1;
	r ^= (t&1)<<2;  t >>= 1;
	r ^= (t&1)<<4;  t >>= 1;
	r ^= (t&1)<<6;  t >>= 1;
	r ^= (t&1)<<8;  t >>= 1;
	r ^= (t&1)<<10;  t >>= 1;
	r ^= (t&1)<<12;  t >>= 1;
	r ^= (t&1)*(irr<<1); t >>= 1; // x^7
	r ^= (t&1)*(irr<<3); t >>= 1; // x^8
	r ^= (t&1)*(irr<<5); t >>= 1; // x^9
	r ^= (t&1)*(irr<<7); t >>= 1; // x^10
	r ^= (t&1)*x_22;     t >>= 1; // x^11
	r ^= (t&1)*x_24;              // x^12

	return r& MY_GFMASK;
}



gf gf2_13_squ(gf in) { return gf_sq(in); }


////////////////////////////////


/* 2 field squarings */
static inline gf gf_sq2(gf in)
{
#if 1
	const uint32_t irr = 0x201b;
	const uint32_t x_24 = 0x185a;
	const uint32_t x_28 = 0x514;
	const uint32_t x_32 = 0x1176;
	const uint32_t x_36 = 0x17b8;
	const uint32_t x_40 = 0x1b75;
	const uint32_t x_44 = 0x17ff;
	const uint32_t x_48 = 0x1f05;
	uint32_t r = 0;

	uint32_t t = in;
	r = (t&1);  t >>= 1;
	r ^= (t&1)<<4;   t >>= 1;  // x^1
	r ^= (t&1)<<8;   t >>= 1;  // x^2
	r ^= (t&1)<<12;  t >>= 1;  // x^3
	r ^= (t&1)*(irr<<3);  t >>= 1;  // x^4
	r ^= (t&1)*(irr<<7);  t >>= 1;  // x^5
	r ^= (t&1)*x_24;     t >>= 1; // x^6
	r ^= (t&1)*x_28;     t >>= 1; // x^7
	r ^= (t&1)*x_32;     t >>= 1; // x^8
	r ^= (t&1)*x_36;     t >>= 1; // x^9
	r ^= (t&1)*x_40;     t >>= 1; // x^10
	r ^= (t&1)*x_44;     t >>= 1; // x^11
	r ^= (t&1)*x_48;              // x^12

	return r& MY_GFMASK;
#else
	gf t = gf_sq(in);
	return gf_sq(t);
#endif
}

/* square and multiply */
static inline gf gf_sqmul(gf in, gf m)
{
        gf t = gf_sq(in);
        return gf2_13_mul(t,m);
}

/* square twice and multiply */
static inline gf gf_sq2mul(gf in, gf m)
{
        gf t = gf_sq2(in);
        return gf2_13_mul(t,m);
}


///////////////////////////////////////////////


// 582727/1024 m4 cycles
gf gf2_13_inv_fermat(gf den)
{
	gf tmp_11;
	gf tmp_1111;
	gf out;

	tmp_11 = gf_sqmul(den, den); // 11
	tmp_1111 = gf_sq2mul(tmp_11, tmp_11); // 1111
	out = gf_sq2(tmp_1111);
	out = gf_sq2mul(out, tmp_1111); // 11111111
	out = gf_sq2(out);
	out = gf_sq2mul(out, tmp_1111); // 111111111111

	return gf_sq(out); // 1111111111110
}

////////////////////////////////////////////////



// 16-bits unit

// x^13 + x^4 + x^3 + x + 1
//   10, 0000, 0001, 1011  --> 2,0,1,b
// 891975/1024 m4 cycles
static inline
uint16_t _extgcd_fieldpoly( uint16_t _g )
{
    // extended GCD
    uint16_t f = 0x201b;
    uint16_t g = ((uint16_t)_g)<<1;
    int16_t delta = 1;

    uint16_t r = 0x2000;
    uint16_t v = 0;

    for(int i=0;i<(13*2-1);i++) {
      uint16_t g0 = (g>>13)&1;
      int16_t minus_delta = -delta;
      uint16_t swap = (minus_delta>>15) & g0;  // >>15 -> get sign bit
      //uint16_t f0g0 = g0;  // f0 is always 1

      // update delta
      delta = swap*(minus_delta<<1)+delta+1;

      // update f, g, v, r
      uint16_t f_g = (f^g);
      g ^= (f*g0);
      f ^= (f_g)*swap;

      uint16_t v_r = (v^r);
      r ^= (v)*g0;
      v ^= (v_r)*swap;

      g <<= 1;
      v >>= 1;
    }
    return v&MY_GFMASK;
}



#if 1
/* return 1/den */
//gf gf2_13_inv_extgcd(gf den)
gf gf2_13_inv(gf den)
{
	return _extgcd_fieldpoly(den);
}


////////////////////////////////////////

#else
gf gf2_13_inv(gf den)
{
	return gf2_13_inv_fermat(den);
}
#endif

/////////////////////////////////////////


/* return num/den */
gf gf2_13_frac(gf den, gf num)
{
	gf inv = gf2_13_inv(den);
	return gf2_13_mul(num,inv);
}


