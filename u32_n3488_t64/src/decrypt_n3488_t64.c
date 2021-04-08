/*
  This file is for Niederreiter decryption
*/

#include "decrypt.h"

#include "params.h"
#include "fft_tr.h"
#include "benes.h"
#include "util.h"
#include "fft.h"
#include "bm.h"

#include "vec32.h"


#include "run_config.h"
#include "sk_util_n4096_t64.h"


#include <stdio.h>






static void scaling( uint32_t out[][GFBITS] , uint32_t values[][GFBITS], const uint32_t * received )
{
	int i, j;
	for (i = 0; i < 128; i++)
	for (j = 0; j < GFBITS; j++)
		out[i][j] = values[i][j] & received[i];
}


static int weight_3488(uint32_t *v)
{
	int i, w = 0;
	for (i = 0; i < 3488; i++)
		w += (v[i>>5] >> (i&31)) & 1;

	return w;
}

static inline uint32_t get_nonzero_mask_u32(const uint32_t * a, int len)
{
	uint32_t ret = a[0];
	for (int i = 1; i < len; i++) ret |= a[i];
	return ret;
}

static inline uint32_t is_zero( uint32_t m )
{
	uint32_t r = m;
	r = (r|(r>>16));
	r = (r&0xffff)-1;
	return (r>>16)&1;
}


#define N_EXT (4096)

/* Niederreiter decryption with the Berlekamp decoder */
/* intput: sk, secret key */
/*         s, ciphertext (syndrome) */
/* output: e, error vector */
/* return: 0 for success; 1 for failure */
int decrypt_n3488_t64(unsigned char *e, const unsigned char *sk, const unsigned char *s)
{
	int i;

	uint32_t check_synd;
	uint32_t check_weight;

	uint32_t temp[128][GFBITS];   // 8 KB. (1) received * (1/g(alpha)) befroe computing syndromes. (2) results of FFT( error_locator ) <-- need to know position only actually.
	uint32_t s_priv[4][GFBITS];     // 0.25KB. syndrome
	uint32_t s_priv_cmp[4][GFBITS]; // 0.25KB. syndrome from re-encrypting
	uint32_t locator[2][GFBITS];  // 0.12 KB.
	uint32_t error[128];  // output, 4096 bits, 0.5KB
	uint32_t recv[128];   // 0.5KB. expanded input data. padding with 0 before the secret permutation.

	uint32_t inv_g[128][GFBITS];    // 8KB. temporary data for  1/g(alpha)^2, can be derived from secret key. can be removed.
	calc_1_over_g_sq(inv_g, sk);    // derive 1/g(alpha)^2 from SK.

	// Berlekamp decoder
	for(i=0;i<SYND_BYTES/4;i++) recv[i]=load4(s+i*4);
	for(i=SYND_BYTES/4;i<((N_EXT/8)/4);i++) recv[i]=0;
	//benes(recv , sk + IRR_BYTES, 1);  // secret permutation
	benes_4096(recv , sk + IRR_BYTES, 1);  // secret permutation

	scaling(temp, inv_g, recv);

	fft_tr_p4096_v128_u32( s_priv, temp );  // calculate syndrome
	bm( locator , s_priv );
	fft_p64_v4096_u32( temp , locator );

	// need to know the position only
	for(i=0;i<128;i++) error[i] = get_nonzero_mask_u32( temp[i] , GFBITS ) ^ 0xffffffff;


	// post processing:

	// re-encrypt
	scaling(temp, inv_g, error );
	fft_tr_p4096_v128_u32(s_priv_cmp, temp);  // calculate syndrome

	// check if syndrome is equal ?
	uint32_t cmp_eq = 0;
	for(i=0;i<4;i++) for(int j=0;j<GFBITS;j++) cmp_eq |= s_priv_cmp[i][j]^s_priv[i][j];
	check_synd = is_zero( cmp_eq );

	// check if weight is equal ?
	//benes(error, sk + IRR_BYTES, 0); /// !!!!!!!!!!!!!!!!!!!
	benes_4096(error, sk + IRR_BYTES, 0); /// !!!!!!!!!!!!!!!!!!!
	check_weight = weight_3488(error) ^ SYS_T;

	// check [3488-4096] == 0
	check_weight |= get_nonzero_mask_u32( error + (3488/32) , (4096-3488)/32 );
	check_weight = is_zero( check_weight );

	// output
	for (i = 0; i < (3488/32); i++)
		store4(e + i*4, error[i]);

#ifdef KAT
  {
    int k;
    printf("decrypt e: positions");
    for (k = 0;k < 8192;++k)
      if (e[k/8] & (1 << (k&7)))
        printf(" %d",k);
    printf("\n");
  }
#endif

	return 1 - (check_synd & check_weight);
}



