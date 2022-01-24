#include "gf2_13_vec32.h"

#define MY_GFBITS (13)

#include "run_config.h"

// gf_2_13 = gf_2[x]/x^13 + x^4 + x^3 + x + 1

#if !defined( _M4_ASM_ )

void vec32_2_13_mul(uint32_t * h, const uint32_t * f, const uint32_t * g)
{
	int i, j;
	uint32_t buf[ 2*MY_GFBITS-1 ];

	for (i = 0; i < 2*MY_GFBITS-1; i++)
		buf[i] = 0;

	for (i = 0; i < MY_GFBITS; i++)
	for (j = 0; j < MY_GFBITS; j++)
		buf[i+j] ^= f[i] & g[j];

	for (i = 2*MY_GFBITS-2; i >= MY_GFBITS; i--)
	{
		buf[i-MY_GFBITS+4] ^= buf[i];
		buf[i-MY_GFBITS+3] ^= buf[i];
		buf[i-MY_GFBITS+1] ^= buf[i];
		buf[i-MY_GFBITS+0] ^= buf[i];
	}

	for (i = 0; i < MY_GFBITS; i++)
		h[i] = buf[i];
}

void vec32_2_13_accu_mul(uint32_t * h, const uint32_t * f, const uint32_t * g, unsigned limb)
{
  uint32_t buf[MY_GFBITS];
  if(limb) for(int j=0;j<MY_GFBITS;j++) h[j]=0;
  for(unsigned i=0;i<limb;i++) {
    vec32_2_13_mul( buf, f+(i*MY_GFBITS) , g+(i*MY_GFBITS) );
    for(int j=0;j<MY_GFBITS;j++) h[j] ^= buf[j];
  }
}

#endif

/* bitsliced field squarings */
void vec32_2_13_sq(uint32_t * out, const uint32_t * in)
{
	int i;
	uint32_t result[MY_GFBITS], t;

	t = in[11] ^ in[12];

	result[0] = in[0] ^ in[11];
	result[1] = in[7] ^ t;
	result[2] = in[1] ^ in[7];
	result[3] = in[8] ^ t;
	result[4] = in[2] ^ in[7];
	result[4] = result[4] ^ in[8];
	result[4] = result[4] ^ t;
	result[5] = in[7] ^ in[9];
	result[6] = in[3] ^ in[8];
	result[6] = result[6] ^ in[9];
	result[6] = result[6] ^ in[12];
	result[7] = in[8] ^ in[10];
	result[8] = in[4] ^ in[9];
	result[8] = result[8] ^ in[10];
	result[9] = in[9] ^ in[11];
	result[10] = in[5] ^ in[10];
	result[10] = result[10] ^ in[11];
	result[11] = in[10] ^ in[12];
	result[12] = in[6] ^ t;

	for (i = 0; i < MY_GFBITS; i++)
		out[i] = result[i];
}

/* bitsliced field inverses */
void vec32_2_13_inv(uint32_t * out, const uint32_t * in)
{
	uint32_t tmp_11[ MY_GFBITS ];
	uint32_t tmp_1111[ MY_GFBITS ];

	for(int i=0;i<MY_GFBITS;i++) out[i]=in[i];

	vec32_2_13_sq(out, out);
	vec32_2_13_mul(tmp_11, out, in); // ^11

	vec32_2_13_sq(out, tmp_11);
	vec32_2_13_sq(out, out);
	vec32_2_13_mul(tmp_1111, out, tmp_11); // ^1111

	vec32_2_13_sq(out, tmp_1111);
	vec32_2_13_sq(out, out);
	vec32_2_13_sq(out, out);
	vec32_2_13_sq(out, out);
	vec32_2_13_mul(out, out, tmp_1111); // ^11111111

	vec32_2_13_sq(out, out);
	vec32_2_13_sq(out, out);
	vec32_2_13_sq(out, out);
	vec32_2_13_sq(out, out);
	vec32_2_13_mul(out, out, tmp_1111); // ^111111111111

	vec32_2_13_sq(out, out); // ^1111111111110
}


