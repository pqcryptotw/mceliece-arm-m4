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


#if 1
// 32-bits unit

static inline
uint32_t mask_nonzero_0or1( uint32_t g ) { return -g; }

static inline
uint32_t mask_lt0( int32_t g ) {
	return g>>31; // sign shit
}


// x^13 + x^4 + x^3 + x + 1
//   10, 0000, 0001, 1011  --> 2,0,1,b
//  729159/1024 m4 cycles
static inline
uint32_t _extgcd_fieldpoly( uint16_t _g )
{
	// align to msb
	uint32_t g = _g;  g <<= 19;
	uint32_t f = 0x201b<<18;
	uint32_t v = 0;
	uint32_t r = 0x80000000;
	int32_t delta = 1;
	int32_t minus_delta;

	uint32_t f0, g0, swap_mask, temp;

	for(int i=0;i<26;i++) {

		f0 = mask_nonzero_0or1(f>>31);
		g0 = mask_nonzero_0or1(g>>31);

		minus_delta = -delta;
		swap_mask = mask_lt0( minus_delta ) & g0;

		// update delta
		delta ^= swap_mask & (delta^minus_delta);
		delta += 1;

		// update f,g
		temp = (f0&g)^(g0&f);
		f ^= swap_mask & (f^g);
		g = temp<<1;

		// update v,r
		temp = (f0&r)^(g0&v);
		v ^= swap_mask & (v^r);
		v >>=1;
		r = temp;
	}
	return v>>17;
}




#else
// 16-bits unit

static inline
uint16_t mask_nonzero_0or1( uint16_t g ) { return -g; }

static inline
uint16_t mask_lt0( int16_t g ) {
	return g>>16; // sign shit
}


// x^13 + x^4 + x^3 + x + 1
//   10, 0000, 0001, 1011  --> 2,0,1,b
// 891975/1024 m4 cycles
static inline
uint16_t _extgcd_fieldpoly( uint16_t g )
{
	// align to msb
	g <<= 3;
	uint16_t f = 0x201b<<2;
	uint16_t v = 0;
	uint16_t r = 0x8000;
	int16_t delta = 1;
	int16_t minus_delta;

	uint16_t f0, g0, swap_mask, temp;

	for(int i=0;i<26;i++) {
		f0 = mask_nonzero_0or1(f>>15);
		g0 = mask_nonzero_0or1(g>>15);

		minus_delta = -delta;
		swap_mask = mask_lt0( minus_delta ) & g0;

		// update delta
		delta ^= swap_mask & (delta^minus_delta);
		delta += 1;

		// update f,g
		temp = (f0&g)^(g0&f);
		f ^= swap_mask & (f^g);
		g = temp<<1;

		// update v,r
		temp = (f0&r)^(g0&v);
		v ^= swap_mask & (v^r);
		v >>=1;
		r = temp;
	}
	return v>>1;
}

#endif


/* return 1/den */
gf gf2_13_inv_extgcd(gf den)
{
	return _extgcd_fieldpoly(den);
}


////////////////////////////////////////


gf gf2_13_inv(gf den)
{
	return gf2_13_inv_fermat(den);
}


/////////////////////////////////////////


/* return num/den */
gf gf2_13_frac(gf den, gf num)
{
	gf inv = gf2_13_inv(den);
	return gf2_13_mul(num,inv);
}


