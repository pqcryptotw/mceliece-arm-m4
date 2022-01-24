/*
  This file is for functions for field arithmetic
*/

#ifndef GF_2_13_128_H
#define GF_2_13_128_H

#include <stdint.h>

typedef uint16_t gf;


void GF213_128_mul(gf *out, gf *in0, gf *in1);


#endif

