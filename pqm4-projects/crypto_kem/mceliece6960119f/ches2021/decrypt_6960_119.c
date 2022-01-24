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
#include "sk_util_n8192_t119.h"


#include <stdio.h>


#if defined(_SMALL_STACK_)
static void scaling( uint32_t out[][GFBITS] , const unsigned char * poly_g , const uint32_t * received )
{
	int i, j;
	calc_1_over_g_sq(out, poly_g);

	for (i = 0; i < 256; i++)
	for (j = 0; j < GFBITS; j++)
		out[i][j] = out[i][j] & received[i];

}
#else
static void scaling( uint32_t out[][GFBITS] , uint32_t values[][GFBITS], const uint32_t * received )
{
	int i, j;
	for (i = 0; i < 256; i++)
	for (j = 0; j < GFBITS; j++)
		out[i][j] = values[i][j] & received[i];
}
#endif

static int weight_6960(uint32_t *v)
{
	int i, w = 0;
	for (i = 0; i < 6960; i++)
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


#define N_EXT (8192)

/* Niederreiter decryption with the Berlekamp decoder */
/* intput: sk, secret key */
/*         s, ciphertext (syndrome) */
/* output: e, error vector */
/* return: 0 for success; 1 for failure */
int decrypt_6960_119(unsigned char *e, const unsigned char *sk, const unsigned char *s)
{
	int i;

	uint32_t temp[256][GFBITS];   // 16 KB. (1) received * (1/g(alpha)) befroe computing syndromes. (2) results of FFT( error_locator ) <-- need to know position only actually.
	uint32_t s_priv[8][GFBITS];     // 0.5KB. syndrome
	uint32_t s_priv_cmp[8][GFBITS]; // 0.5KB. syndrome from re-encrypting
	uint32_t locator[4][GFBITS];  // 0.25 KB.
	uint32_t error[ 256 ];  // output, 8192 bits, 1KB
	uint32_t recv[256];   // 1KB. expanded input data. padding with 0 before the secret permutation.

#if !defined(_SMALL_STACK_)
	uint32_t inv_g[256][GFBITS];    // 16KB. temporary data for  1/g(alpha)^2, can be derived from secret key. can be removed.
	calc_1_over_g_sq(inv_g, sk);    // derive 1/g(alpha)^2 from SK.
#endif

	// SYND_BYTES = 194

	// Berlekamp decoder
	for(i=0;i<SYND_BYTES/4;i++) recv[i]=load4(s+i*4);  // 194/4 = 48 ... 2
	if(SYND_BYTES%4) {
		uint32_t v=0;
		for(int j=(SYND_BYTES%4)-1;j>=0;j--) { v<<=8; v+=s[i*4+j]; }
		recv[i]=v;
		i++;
	}
	for(;i<((N_EXT/8)/4);i++) recv[i]=0;
	benes(recv , sk + IRR_BYTES, 1);  // secret permutation

#if defined(_SMALL_STACK_)
	scaling(temp, sk, recv );
#else
	scaling(temp, inv_g, recv);
#endif

	fft_tr_p8192_v256( s_priv, temp );  // calculate syndrome
	bm( locator , s_priv );
	fft_p127_v8192( temp , locator );

	// need to know the position only
	for(i=0;i<256;i++) error[i] = get_nonzero_mask_u32( temp[i] , GFBITS ) ^ 0xffffffff;


	// post processing:
	uint32_t check_synd;
	uint32_t check_weight;

	// re-encrypt
#if defined(_SMALL_STACK_)
	scaling(temp, sk, error );
#else
	scaling(temp, inv_g, error );
#endif
	fft_tr_p8192_v256(s_priv_cmp, temp);  // calculate syndrome

	// check if syndrome is equal ?
	uint32_t cmp_eq = 0;
	for(i=0;i<8;i++) for(int j=0;j<GFBITS;j++) cmp_eq |= s_priv_cmp[i][j]^s_priv[i][j];
	check_synd = is_zero( cmp_eq );


	// check if weight is equal ?
	benes(error, sk + IRR_BYTES, 0);
	check_weight = weight_6960(error) ^ SYS_T;


	// 6960 = 217x32 + 16
	// check [6960-8192] == 0
	check_weight |= error[(6960/32)]&0xffff0000;
	check_weight |= get_nonzero_mask_u32( &error[218] , 256-218 );
	check_weight = is_zero( check_weight );

	// output
	for (i = 0; i < (6960/32); i++)
		store4(e + i*4, error[i]);
	e[217*4]   = error[217]     &0xff;
	e[217*4+1] = (error[217]>>8)&0xff;



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

