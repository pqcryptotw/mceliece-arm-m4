/*
  This file is for the Gao-Mateer FFT
  sse http://www.math.clemson.edu/~sgao/papers/GM10.pdf
*/

#ifndef FFT_H
#define FFT_H

#include <stdint.h>

#include "params.h"

void fft_u64(uint64_t [][GFBITS], uint64_t [][GFBITS]);

void fft_p127_v8192(uint32_t values [][GFBITS], uint32_t poly [][GFBITS]);

#define fft fft_p127_v8192

#endif

