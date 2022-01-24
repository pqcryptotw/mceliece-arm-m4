/*
  This file is for the Gao-Mateer FFT
  sse http://www.math.clemson.edu/~sgao/papers/GM10.pdf
*/

#ifndef FFT_H
#define FFT_H

#include <stdint.h>
#include "params.h"
//#include "vec.h"


void fft_vec(uint64_t [][ GFBITS ], uint64_t *);

void fft_p64_v4096( uint64_t [][ GFBITS ], uint64_t * );

void fft_p64_v4096_u32(uint32_t values [][GFBITS], uint32_t poly [][GFBITS]);

//#define fft fft_vec
#define fft fft_p64_v4096_u32

#endif

