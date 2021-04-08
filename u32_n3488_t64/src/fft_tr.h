/*
  This file is for transpose of the Gao-Mateer FFT
*/

#ifndef FFT_TR_H
#define FFT_TR_H

#include "params.h"
//#include "vec.h"
#include "stdint.h"

void fft_tr_vec(uint64_t [][GFBITS], uint64_t [][ GFBITS ]);

void fft_tr_p4096_v128(uint64_t [][GFBITS], uint64_t [][ GFBITS ]);

void fft_tr_p4096_v128_u32(uint32_t out32[4][GFBITS], uint32_t in32[128][GFBITS]);

//#define fft_tr fft_tr_p4096_v128
#define fft_tr fft_tr_p4096_v128_u32

#endif

