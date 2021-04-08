/*
  This file is for the inversion-free Berlekamp-Massey algorithm
  see https://ieeexplore.ieee.org/document/87857
*/

#include "bm_vec32.h"

#include "gf.h"

#include "vec32.h"

#include "util.h"

static inline uint16_t mask_nonzero(gf a)
{
	uint32_t ret = a;

	ret -= 1;
	ret >>= 31;
	ret -= 1;

	return ret;
}

static inline uint16_t mask_leq(uint16_t a, uint16_t b)
{
	uint32_t a_tmp = a;
	uint32_t b_tmp = b;
	uint32_t ret = b_tmp - a_tmp; 

	ret >>= 31;
	ret -= 1;

	return ret;
}


static inline void cmov_u32(uint32_t out[GFBITS], uint32_t in[GFBITS], uint16_t mask)
{
	int i;

	uint32_t m0, m1;
	m0 = mask;
	m0 ^= m0<<16;
	m1 = ~m0;

	for (i = 0; i < GFBITS; i++) out[i] = (in[i] & m0) | (out[i] & m1);
}

static inline void get_32_coefs(gf *out, uint32_t in[GFBITS] )
{
	uint16_t *mat1 = out;
	uint16_t *mat2 = out+16;

	bs32_to_bs16(mat1, mat2, in, GFBITS);
	for(int i=GFBITS;i<16;i++) mat1[i] = 0;
	for(int i=GFBITS;i<16;i++) mat2[i] = 0;

	transpose_16x16( mat1 , mat1 );
	transpose_16x16( mat2 , mat2 );
}


static void shr_1_vec( gf e , uint32_t in[][GFBITS] , unsigned idx_maxlimb, unsigned n_limbs )
{
	int i;
	unsigned j;
	uint32_t tmp;

	for (i = 0; i < GFBITS; i++)
	{
		tmp = ((e >> i) & 1)<<31;
		for(j=0;j<n_limbs;j++) {
			uint32_t t = in[idx_maxlimb-j][i]<<31;
			in[idx_maxlimb-j][i] = (in[idx_maxlimb-j][i]>>1)|tmp;
			tmp = t;
		}
	}
}

static inline gf reduce_u32(const uint32_t in[GFBITS])
{
	int i;
	uint32_t tmp;
	uint32_t ret = 0;

	for (i = GFBITS-1; i >= 0; i--)
	{
		tmp = in[i];
		tmp ^= tmp >> 16;
		tmp ^= tmp >> 8;
		tmp ^= tmp >> 4;
		tmp ^= tmp >> 2;
		tmp ^= tmp >> 1;

		ret <<= 1;
		ret |= tmp & 1;
	}

	return (gf)(ret&0xffff);
}

#define min(a, b) ((a < b) ? a : b)

// Slower. Only for comparison
//#define _INV_FREE_


#if !defined(_INV_FREE_)

#define _USE_GFU32_

#if defined(_USE_GFU32_)
#include "gfu32.h"
#endif


/* input: seq_256, sequence of field elements of length 256 */
/* output: poly_128, minimal polynomial of in of degree < 128 */
// WARNNING: len_seq <= 256 !!!!!!!!!
//void bm_128(vec out[][ GFBITS ], vec in[][ GFBITS ])
void bm_vec32( uint32_t poly [][GFBITS], uint32_t seq [][ GFBITS ] , unsigned len_seq )
{
	// assert( len_seq <= 256 );
	unsigned maxlen_poly = len_seq>>1;
	int maxlimb = (maxlen_poly+31)>>5;
	int idx_maxlimb = maxlimb -1;

	int i,j;
	uint16_t N, L;
	uint16_t mask;

	uint32_t temp[GFBITS];
	//uint32_t inner_prod[GFBITS];
	uint32_t interval[4][GFBITS];
	uint32_t dd[GFBITS];
	uint32_t B[4][GFBITS], C[4][GFBITS];
	uint32_t B_tmp[4][GFBITS];

	gf d, c0;
	gf coefs[256];

#if defined(_USE_GFU32_)
	gfu32 b;
	gfu32 f, tmp;
#else
	gf b;
	gf f, tmp;
#endif

	// initialization
	for(i=0;i*32<(int)len_seq;i++){
		get_32_coefs(&coefs[i*32], seq[i] );
	}

	for(i=0;i<maxlimb;i++) for(j=0;j<GFBITS;j++) C[i][j] = 0;
	for(i=0;i<maxlimb;i++) for(j=0;j<GFBITS;j++) interval[i][j] = 0;
	for(i=0;i<maxlimb;i++) for(j=0;j<GFBITS;j++) B[i][j] = 0;
	B[idx_maxlimb][0] = 1<<31;

	b = 1;
	c0 = 1;
	L = 0;

	//

	for (N = 0; N < len_seq; N++)
	{
		int len = min(N,maxlen_poly-1) + 1;
		int limb = (len+31)>>5;
		//if( limb > maxlimb ) limb = maxlimb;

		//vec32_mul(inner_prod, C[idx_maxlimb], interval[idx_maxlimb] );
		//for(int k=1;k<limb;k++) {
		//vec32_mul(temp, C[idx_maxlimb-k], interval[idx_maxlimb-k] ); for(i=0;i<GFBITS;i++) inner_prod[i] ^= temp[i];
		//}
		//d = reduce_u32( inner_prod );
		vec32_accu_mul(temp, C[idx_maxlimb-(limb-1)] , interval[idx_maxlimb-(limb-1)] , limb );
		d = reduce_u32( temp );

		//d ^= gf_mul(c0,coefs[N]);
		d ^= coefs[N];  // c0 = 1

		shr_1_vec( coefs[N] , interval , idx_maxlimb, limb );

		mask = mask_nonzero(d) & mask_leq(L*2, N);

#if defined(_USE_GFU32_)
		gfu32 d32 = gf_to_gfu32(d);
		tmp = gfu32_inv(b);
		f = gfu32_mul(d32,tmp);
#if 8 > GFBITS
error here.
#endif
		for(i=0;i<8;i++) { dd[i] = -((f >> (i*4)) & 1); }
		for(i=8;i<GFBITS;i++) { dd[i] = -( (f >> ((i-8)*4+1)) & 1); }
#else
		tmp = gf_inv(b);
		f = gf_mul(d,tmp);
		//next_c0 = gf_mul(c0,b);
		for (i = 0; i < GFBITS; i++)
		{
			dd[i] = -((uint32_t)((f >> i) & 1));
		}
#endif
		for(int k=0;k<limb;k++) {
		vec32_mul(B_tmp[idx_maxlimb-k], dd, B[idx_maxlimb-k]);
		//vec32_mul(C_tmp[idx_maxlimb-k], bb, C[idx_maxlimb-k]);
		cmov_u32(B[idx_maxlimb-k], C[idx_maxlimb-k], mask);
		}

		shr_1_vec( c0&mask , B , idx_maxlimb , limb );

		for(int k=0;k<limb;k++) {
		for (i = 0; i < GFBITS; i++)
		{
			C[idx_maxlimb-k][i] = B_tmp[idx_maxlimb-k][i] ^ C[idx_maxlimb-k][i];
		}
		}

#if defined(_USE_GFU32_)
		uint32_t mask32 = mask; mask32 = (mask32<<16)|mask;
		b = (d32 & mask32) | (b & ~mask32);
#else
		//c0 = next_c0;
		b = (d & mask) | (b & ~mask);
#endif
		L = ((N+1-L) & mask) | (L & ~mask);
	}

	//c0 = gf_inv(c0);
	//for (i = 0; i < GFBITS; i++)
	//	temp[i] = -((uint32_t)((c0 >> i) & 1));
	for(i=0;i<maxlimb;i++) {
		//vec32_mul(poly[i], C[i] , temp );
		for(j=0;j<GFBITS;j++) poly[i][j] = C[i][j];
	}
}



#else


/* input: seq_256, sequence of field elements of length 256 */
/* output: poly_128, minimal polynomial of in of degree < 128 */
// WARNNING: len_seq <= 256 !!!!!!!!!
//void bm_128(vec out[][ GFBITS ], vec in[][ GFBITS ])
void bm_vec32( uint32_t poly [][GFBITS], uint32_t seq [][ GFBITS ] , unsigned len_seq )
{
	// assert( len_seq <= 256 );
	unsigned maxlen_poly = len_seq>>1;
	int maxlimb = (maxlen_poly+31)>>5;
	int idx_maxlimb = maxlimb -1;

	int i,j;
	uint16_t N, L;
	uint16_t mask;

	uint32_t temp[GFBITS];
	//uint32_t inner_prod[GFBITS];
	uint32_t interval[4][GFBITS];
	uint32_t dd[GFBITS], bb[GFBITS];
	uint32_t B[4][GFBITS], C[4][GFBITS];
	uint32_t B_tmp[4][GFBITS], C_tmp[4][GFBITS];

	gf next_c0;
	gf d, b, c0;
	gf coefs[256];

	// initialization
	for(i=0;i*32<(int)len_seq;i++){
		get_32_coefs(&coefs[i*32], seq[i] );
	}

	for(i=0;i<maxlimb;i++) for(j=0;j<GFBITS;j++) C[i][j] = 0;
	for(i=0;i<maxlimb;i++) for(j=0;j<GFBITS;j++) interval[i][j] = 0;
	for(i=0;i<maxlimb;i++) for(j=0;j<GFBITS;j++) B[i][j] = 0;
	B[idx_maxlimb][0] = 1<<31;

	b = 1;
	c0 = 1;
	L = 0;

	//

	for (N = 0; N < len_seq; N++)
	{
		int len = min(N,maxlen_poly-1) + 1;
		int limb = (len+31)>>5;
		//if( limb > maxlimb ) limb = maxlimb;

		//vec32_mul(inner_prod, C[idx_maxlimb], interval[idx_maxlimb] );
		//for(int k=1;k<limb;k++) {
		//vec32_mul(temp, C[idx_maxlimb-k], interval[idx_maxlimb-k] ); for(i=0;i<GFBITS;i++) inner_prod[i] ^= temp[i];
		//}
		//d = reduce_u32( inner_prod );
		vec32_accu_mul(temp, C[idx_maxlimb-(limb-1)] , interval[idx_maxlimb-(limb-1)] , limb );
		d = reduce_u32( temp );

		d ^= gf_mul(c0,coefs[N]);

		shr_1_vec( coefs[N] , interval , idx_maxlimb, limb );

		mask = mask_nonzero(d) & mask_leq(L*2, N);

		next_c0 = gf_mul(c0,b);
		for (i = 0; i < GFBITS; i++)
		{
			dd[i] = -((uint32_t)((d >> i) & 1));
			bb[i] = -((uint32_t)((b >> i) & 1));
		}
		for(int k=0;k<limb;k++) {
		vec32_mul(B_tmp[idx_maxlimb-k], dd, B[idx_maxlimb-k]);
		vec32_mul(C_tmp[idx_maxlimb-k], bb, C[idx_maxlimb-k]);
		cmov_u32(B[idx_maxlimb-k], C[idx_maxlimb-k], mask);
		}

		shr_1_vec( c0&mask , B , idx_maxlimb , limb );

		for(int k=0;k<limb;k++) {
		for (i = 0; i < GFBITS; i++)
		{
			C[idx_maxlimb-k][i] = B_tmp[idx_maxlimb-k][i] ^ C_tmp[idx_maxlimb-k][i];
		}
		}

		c0 = next_c0;
		b = (d & mask) | (b & ~mask);
		L = ((N+1-L) & mask) | (L & ~mask);
	}

	c0 = gf_inv(c0);

	for (i = 0; i < GFBITS; i++)
		temp[i] = -((uint32_t)((c0 >> i) & 1));

	for(i=0;i<maxlimb;i++) {
		vec32_mul(poly[i], C[i] , temp );

	}
}


#endif



#if 0

// wrapper for vec64

void bm_64_vec( vec * poly64 , vec seq128 [][ GFBITS ] )
{
  uint32_t seq32[4][GFBITS];
  uint32_t poly32[2][GFBITS];

  bs64_to_bs32( seq32[0] , seq32[1] , seq128[0] , GFBITS);
  bs64_to_bs32( seq32[2] , seq32[3] , seq128[1] , GFBITS);

  bm_vec32( poly32, seq32 , 128 );

  bs32_to_bs64( poly64 , poly32[0] , poly32[1] , GFBITS );

}


#endif
