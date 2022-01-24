#ifndef _SK_4096_64_UTIL_H_
#define _SK_4096_64_UTIL_H_



#include "params.h"
#include "util.h"
#include "fft.h"
#include "vec32.h"

#include "run_config.h"


//
// montgomery's trick
//
static inline void batch_inversion( uint32_t inv_in[][GFBITS], const uint32_t in[][GFBITS], int n_batch )
{
    uint32_t tmp[ GFBITS ];

    for(int i=0;i<GFBITS;i++) inv_in[0][i] = in[0][i];
    for(int i=1;i<n_batch;i++) vec32_mul( inv_in[i] , inv_in[i-1] , in[i] );

    vec32_inv( tmp , inv_in[n_batch-1] );

    for(int i=n_batch-1;i>0;i--) {
        vec32_mul( inv_in[i] , tmp , inv_in[i-1] );
        vec32_mul( tmp , tmp , in[i] );
    }
    for(int i=0;i<GFBITS;i++) inv_in[0][i] = tmp[i];
}



//
// Calculate 1/ g(\alpha) , ....
// 4096 = 32*128 terms
//
static inline void calc_1_over_g( uint32_t values[][GFBITS], const unsigned char * poly_g )
{
    uint32_t irr_bs[2][ GFBITS ];  // bit-sliced form
    uint32_t tmp[128][GFBITS]; // 8 KB...... used for calculating 1/g(). could be reduced if calling more vec32_inv().

    irr_load_32x(irr_bs, poly_g , 64 );
    fft_p64_v4096_u32( tmp , irr_bs );

    batch_inversion( values , tmp , 128 );
}



//
// Calculate 1/ g(\alpha)^2 , ....
// 4096 = 32*128 terms
//
static inline void calc_1_over_g_sq( uint32_t inv_g2[][GFBITS], const unsigned char * poly_g )
{

    uint32_t irr_bs[2][ GFBITS ];  // bit-sliced form
    uint32_t tmp32[128][GFBITS]; // 8 KB...... used for calculating 1/g()^2. could be reduced if calling more vec32_inv().

    irr_load_32x(irr_bs, poly_g , 64 );
    fft_p64_v4096_u32( tmp32 , irr_bs );
    for (int i = 0; i < 128; i++) vec32_sq( tmp32[i], tmp32[i] );
    batch_inversion( inv_g2 , tmp32 , 128 );
}




#endif
