/*
  This file is for functions for field arithmetic
*/

#ifndef GF2_12_U32_H
#define GF2_12_U32_H


#include "stdint.h"

typedef uint32_t gfu32;


///////////////////////////
// arithmetics for gf(2^13)
///////////////////////////


gfu32 gfu32_2_12_mul( gfu32 a, gfu32 b);
//gfu32 gfu32_2_12_squ( gfu32 a );
gfu32 gfu32_2_12_inv( gfu32 a );

gfu32 gfu32_2_12_dot    ( const gfu32 * a , const gfu32 * b , unsigned len );

static inline
gfu32 gfu32_2_12_rev_dot( const gfu32 * a , const gfu32 * b , unsigned len ) { return gfu32_2_12_dot(&a[-((int)len-1)],&b[-((int)len-1)],len); }


void gfu32_2_12_smul( gfu32 * c , const gfu32 * a , gfu32 b , unsigned len );


#endif

