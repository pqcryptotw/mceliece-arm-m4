/*
  This file is for transpose of the Gao-Mateer FFT
*/

#ifndef FFT_TR_H
#define FFT_TR_H

#include <stdint.h>
#include "params.h"

void fft_tr_u64(uint64_t out[][GFBITS], uint64_t in[][ GFBITS ]);

void fft_tr_p8192_v256(uint32_t out[][GFBITS], uint32_t in[][ GFBITS ]);


#define fft_tr fft_tr_p8192_v256

#endif

