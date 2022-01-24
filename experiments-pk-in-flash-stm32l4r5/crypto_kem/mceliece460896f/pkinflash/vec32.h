#ifndef _VEC_32_H
#define _VEC_32_H


#include <stdint.h>


#include "params.h"


#if 13 == GFBITS

#include "gf2_13_vec32.h"

static inline
void vec32_mul(uint32_t *c, const uint32_t *a, const uint32_t *b) { vec32_2_13_mul(c,a,b); }

static inline
void vec32_accu_mul(uint32_t *c, const uint32_t *a, const uint32_t *b, unsigned limb) { vec32_2_13_accu_mul(c,a,b,limb); }

static inline
void vec32_sq (uint32_t *c, const uint32_t *a) { vec32_2_13_sq(c,a); }

static inline
void vec32_inv(uint32_t *c, const uint32_t *a) { vec32_2_13_inv(c,a); }

#elif 12 == GFBITS

#include "gf2_12_vec32.h"

static inline
void vec32_mul(uint32_t *c, const uint32_t *a, const uint32_t *b) { vec32_2_12_mul(c,a,b); }

static inline
void vec32_accu_mul(uint32_t *c, const uint32_t *a, const uint32_t *b, unsigned limb) { vec32_2_12_accu_mul(c,a,b,limb); }

static inline
void vec32_sq (uint32_t *c, const uint32_t *a) { vec32_2_12_sq(c,a); }

static inline
void vec32_inv(uint32_t *c, const uint32_t *a) { vec32_2_12_inv(c,a); }


#else
error: un-supported GFBITS
#endif


#endif

