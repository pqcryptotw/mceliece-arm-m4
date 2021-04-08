#ifndef _GF2_12_VEC32_H_
#define _GF2_12_VEC32_H_

#include <stdint.h>

// gf_2_12 = gf_2[x]/x^12 + x^3 + 1


void vec32_2_12_mul(uint32_t * h, const uint32_t * f, const uint32_t * g);

void vec32_2_12_accu_mul(uint32_t * h, const uint32_t * f, const uint32_t * g, unsigned limb);

void vec32_2_12_sq(uint32_t * out, const uint32_t * in);

void vec32_2_12_inv(uint32_t * out, const uint32_t * in);



#endif
