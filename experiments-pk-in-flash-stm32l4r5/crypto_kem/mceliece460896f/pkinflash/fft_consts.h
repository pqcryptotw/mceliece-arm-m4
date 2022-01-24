#ifndef _FFT_CONSTS_H_
#define _FFT_CONSTS_H_

#include "stdint.h"

#include "params.h"


// for fft raidx conversion
extern const uint32_t s_2x_u32[5][4][GFBITS];

// for fft_tr raidx conversion
extern const uint32_t s_4x_u32[6][8][GFBITS];

// for fft and fft_tr butterflies
extern const uint32_t consts_u32[ 256 ][ GFBITS ];

// values of x^128 for all elements in GF(2^13)
extern const uint32_t power_128_u32[ 256 ][ GFBITS ];


#endif
