#ifndef _SK_UTIL_8192_128_H_
#define _SK_UTIL_8192_128_H_



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
// 8192 = 32*256 terms
//
static inline void calc_1_over_g( uint32_t values[][GFBITS], const unsigned char * poly_g )
{
    uint32_t irr_bs[4][ GFBITS ];  // bit-sliced form
    uint32_t tmp[256][GFBITS]; // 16 KB...... used for calculating 1/g(). could be reduced if calling more vec32_inv().

    uint8_t gg[128*2];
    for(int i=0;i<119;i++) { gg[i*2]=poly_g[i*2]; gg[i*2+1]=poly_g[i*2+1]; }
    for(int i=119;i<128;i++) { gg[i*2]=0; gg[i*2+1]=0; }
    gg[119*2] = 1;

    irr_load_32x(irr_bs, gg , 128 );
    fft_p127_v8192( tmp , irr_bs );

    batch_inversion( values , tmp , 256 );
}



//
// Calculate 1/ g(\alpha)^2 , ....
// 8192 = 32*256 terms
//
static inline void calc_1_over_g_sq( uint32_t values[][GFBITS], const unsigned char * poly_g )
{
    int i;
    uint32_t irr_bs[4][ GFBITS ];  // bit-sliced form
    uint32_t tmp[256][GFBITS]; // 16 KB...... used for calculating 1/g()^2. could be reduced if calling more vec32_inv().

    uint8_t gg[128*2];
    for(int i=0;i<119;i++) { gg[i*2]=poly_g[i*2]; gg[i*2+1]=poly_g[i*2+1]; }
    for(int i=119;i<128;i++) { gg[i*2]=0; gg[i*2+1]=0; }
    gg[119*2] = 1;

    irr_load_32x(irr_bs, gg , 128 );
    fft_p127_v8192( tmp , irr_bs );

    for (i = 0; i < 256; i++) vec32_sq( tmp[i], tmp[i] );
    batch_inversion( values , tmp , 256 );
}






#endif
