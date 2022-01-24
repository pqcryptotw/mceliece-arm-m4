/*
  This file is for functions for field arithmetic
*/

#ifndef GF2_13_U32_H
#define GF2_13_U32_H


#include "stdint.h"

typedef uint32_t gfu32;



///////////////////////////
// arithmetics for gf(2^13)
///////////////////////////


gfu32 gfu32_2_13_mul( gfu32 a, gfu32 b);
//gfu32 gfu32_2_13_squ( gfu32 a );
gfu32 gfu32_2_13_inv( gfu32 a );

gfu32 gfu32_2_13_dot    ( const gfu32 * a , const gfu32 * b , unsigned len );

static inline
gfu32 gfu32_2_13_rev_dot( const gfu32 * a , const gfu32 * b , unsigned len ) { return gfu32_2_13_dot(&a[-((int)len-1)],&b[-((int)len-1)],len); }

void gfu32_2_13_smul( gfu32 * c , const gfu32 * a , gfu32 b , unsigned len );


#endif

