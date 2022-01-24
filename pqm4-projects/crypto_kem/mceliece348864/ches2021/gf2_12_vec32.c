#include "gf2_12_vec32.h"

#define MY_GFBITS (12)

#include "run_config.h"

// gf_2_12 = gf_2[x]/x^12 + x^3 + 1

#if !defined( _M4_ASM_ )

void vec32_2_12_mul(uint32_t * h, const uint32_t * f, const uint32_t * g)
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
		buf[i-MY_GFBITS+3] ^= buf[i];
		buf[i-MY_GFBITS+0] ^= buf[i];
	}

	for (i = 0; i < MY_GFBITS; i++)
		h[i] = buf[i];
}


void vec32_2_12_accu_mul(uint32_t * h, const uint32_t * f, const uint32_t * g, unsigned limb)
{
  uint32_t buf[MY_GFBITS];
  if(limb) for(int j=0;j<MY_GFBITS;j++) h[j]=0;
  for(unsigned i=0;i<limb;i++) {
    vec32_2_12_mul( buf, f+(i*MY_GFBITS) , g+(i*MY_GFBITS) );
    for(int j=0;j<MY_GFBITS;j++) h[j] ^= buf[j];
  }
}


#endif

/* bitsliced field squarings */
void vec32_2_12_sq(uint32_t * out, const uint32_t * in)
{
	int i;
	uint32_t result[MY_GFBITS];

	result[0] = in[0]^in[6];
	result[1] = in[11];
	result[2] = in[1]^in[7];
	result[3] = in[6];
	result[4] = in[2] ^ in[11] ^ in[8];
	result[5] = in[7];
	result[6] = in[3]^in[9];
	result[7] = in[8];
	result[8] = in[4]^in[10];
	result[9] = in[9];
	result[10] = in[5] ^ in[11];
	result[11] = in[10];

	for (i = 0; i < MY_GFBITS; i++)
		out[i] = result[i];
}

/* bitsliced field inverses */
void vec32_2_12_inv(uint32_t * out, const uint32_t * in)
{
	uint32_t tmp_11[ MY_GFBITS ];
	uint32_t tmp_1111[ MY_GFBITS ];

	for(int i=0;i<MY_GFBITS;i++) out[i]=in[i];

	vec32_2_12_sq(out, out);
	vec32_2_12_mul(tmp_11, out, in); // ^11

	vec32_2_12_sq(out, tmp_11);
	vec32_2_12_sq(out, out);
	vec32_2_12_mul(tmp_1111, out, tmp_11); // ^1111

	vec32_2_12_sq(out, tmp_1111);
	vec32_2_12_sq(out, out);
	vec32_2_12_sq(out, out);
	vec32_2_12_sq(out, out);
	vec32_2_12_mul(out, out, tmp_1111); // ^1111,1111

	vec32_2_12_sq(out, out);
	vec32_2_12_sq(out, out);
	vec32_2_12_mul(out, out, tmp_11); // ^11,1111,1111
	vec32_2_12_sq(out, out);
	vec32_2_12_mul(out, out, in); // ^111,1111,1111

	vec32_2_12_sq(out, out); // ^1111,1111,1110
}


