#ifndef _FFT_64_4096_CONSTS_H_
#define _FFT_64_4096_CONSTS_H_

#include "stdint.h"

#include "params.h"


// for fft_tr raidx conversion
extern const uint32_t s_2x_u32[5][4][GFBITS];

// for fft raidx conversion
extern const uint32_t s_u32[5][2][GFBITS];

// for fft and fft_tr butterflies
extern const uint32_t consts_u32[ 126 ][ GFBITS ];

// values of x^64 for all elements in GF(2^12)
extern const uint32_t power_64_u32[ 128 ][ GFBITS ];


#endif
