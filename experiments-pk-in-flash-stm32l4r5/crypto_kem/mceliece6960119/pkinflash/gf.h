/*
  This file is for functions for field arithmetic
*/

#ifndef GF_H
#define GF_H

#include <stdint.h>


typedef uint16_t gf;



/* check if a == 0 */
static inline
gf gf_iszero(gf a)
{
        uint32_t t = a;

        t -= 1;
        t >>= 19;

        return (gf) t;
}


/////////////////////


#include "params.h"


#if 13 == GFBITS

#include "gf2_13.h"

static inline
gf gf_mul(gf a, gf b) { return gf2_13_mul(a,b); }

static inline
gf gf_squ(gf a) { return gf2_13_squ(a); }

static inline
gf gf_frac(gf den, gf num) { return gf2_13_frac(den,num); }

static inline
gf gf_inv(gf a) { return gf2_13_inv(a); }



/////////////////////

#if 96 == SYS_T

#include "gf213_96.h"

static inline
void GF_mul(gf *c, gf *a, gf *b) { return GF213_96_mul(c,a,b); }


#elif 119 == SYS_T

#include "gf213_119.h"

static inline
void GF_mul(gf *c, gf *a, gf *b) { return GF213_119_mul(c,a,b); }

#elif 128 == SYS_T

#include "gf213_128.h"

static inline
void GF_mul(gf *c, gf *a, gf *b) { return GF213_128_mul(c,a,b); }

#else
error: un-supported SYS_T
#endif


#elif 12 == GFBITS


#include "gf2_12.h"

static inline
gf gf_mul(gf a, gf b) { return gf2_12_mul(a,b); }

static inline
gf gf_frac(gf den, gf num) { return gf2_12_frac(den,num); }

static inline
gf gf_inv(gf a) { return gf2_12_inv(a); }


////////////////////////

#include "gf212_64.h"

static inline
void GF_mul(gf *c, gf *a, gf *b) { GF212_64_mul(c,a,b); }




#else
error: un-supported GFBITS
#endif



#endif

