#ifndef _SK_UTIL_8192_96_H_
#define _SK_UTIL_8192_96_H_



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

    irr_load_32x(irr_bs, poly_g , 96 );
    irr_bs[3][0] = 1;
    for(int i=1;i<GFBITS;i++) irr_bs[3][i] = 0;
    fft_p127_v8192( tmp , irr_bs );

    batch_inversion( values , tmp , 256 );
}



//
// Calculate 1/ g(\alpha)^2 , ....
// 8192 = 32*256 terms
//
#if defined(_SMALL_STACK_)
static inline void calc_1_over_g_sq( uint32_t values[][GFBITS], const unsigned char * poly_g )
{
    int i;
    uint32_t irr_bs[4][ GFBITS ];  // bit-sliced form
    uint32_t tmp[128][GFBITS];     // 8 KB...... used for calculating 1/g()^2. could be reduced if calling more vec32_inv().

    irr_load_32x(irr_bs, poly_g , 96 );
    irr_bs[3][0] = 1;
    for(i=1;i<GFBITS;i++) irr_bs[3][i] = 0;
    fft_p127_v8192( values , irr_bs );

    for (i = 0; i < 128; i++) vec32_sq( tmp[i], values[i] );
    batch_inversion( values , tmp , 128 );
    for (i = 0; i < 128; i++) vec32_sq( tmp[i], values[128+i] );
    batch_inversion( &values[128] , tmp , 128 );
}
#else
static inline void calc_1_over_g_sq( uint32_t values[][GFBITS], const unsigned char * poly_g )
{
    int i;
    uint32_t irr_bs[4][ GFBITS ];  // bit-sliced form
    uint32_t tmp[256][GFBITS]; // 16 KB...... used for calculating 1/g()^2. could be reduced if calling more vec32_inv().

    irr_load_32x(irr_bs, poly_g , 96 );
    irr_bs[3][0] = 1;
    for(i=1;i<GFBITS;i++) irr_bs[3][i] = 0;
    fft_p127_v8192( tmp, irr_bs );

    for (i = 0; i < 256; i++) vec32_sq( tmp[i], tmp[i] );
    batch_inversion( values , tmp , 256 );
}
#endif






#endif
