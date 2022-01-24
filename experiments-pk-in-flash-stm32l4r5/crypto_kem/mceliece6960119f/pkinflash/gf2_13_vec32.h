#ifndef _GF2_13_VEC32_H_
#define _GF2_13_VEC32_H_

#include <stdint.h>

// gf_2_13 = gf_2[x]/x^13 + x^4 + x^3 + x + 1


void vec32_2_13_mul(uint32_t * h, const uint32_t * f, const uint32_t * g);

void vec32_2_13_accu_mul(uint32_t * h, const uint32_t * f, const uint32_t * g, unsigned limb);

void vec32_2_13_sq(uint32_t * out, const uint32_t * in);

void vec32_2_13_inv(uint32_t * out, const uint32_t * in);



#endif
