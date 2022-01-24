/*
  This file is for functions for field arithmetic
*/

#ifndef GF_U32_H
#define GF_U32_H

#include <stdint.h>



typedef uint32_t gfu32;


#include "params.h"


#if 13 == GFBITS


#include "gf2_13_u32.h"

#define gfu32_mul       gfu32_2_13_mul
#define gfu32_rev_dot   gfu32_2_13_rev_dot
#define gfu32_dot       gfu32_2_13_dot
#define gfu32_inv       gfu32_2_13_inv
#define gfu32_smul      gfu32_2_13_smul

#elif 12 == GFBITS

#include "gf2_12_u32.h"

#define gfu32_mul       gfu32_2_12_mul
#define gfu32_rev_dot   gfu32_2_12_rev_dot
#define gfu32_dot       gfu32_2_12_dot
#define gfu32_inv       gfu32_2_12_inv
#define gfu32_smul      gfu32_2_12_smul

#else
error: un-supported GFBITS
#endif





/////////////////////////////
// converter to/from normal gf <-- uint16_t
////////////////////////////


#include "transpose.h"


static inline gfu32 gf_to_gfu32( uint16_t e )
{
  uint32_t e32 = e;
  uint32_t r32 = transpose_4x16_half(e32);
  return r32;
}

static inline gfu32 gf_to_gfu32_x2( uint16_t e0 , uint16_t e1 )
{
  uint32_t e32 = e0 ^ (((uint32_t)e1)<<16);
  uint32_t r32 = transpose_4x16_half(e32);
  return r32;
}

static inline uint16_t gfu32_to_gf( gfu32 e32 )
{
  return transpose_16x4_half(e32);
}


static inline void gf_to_gfu32_x4( uint32_t * gfs , const uint16_t * e )
{
  uint32_t mask = 0x11111111;
  union{
  uint32_t v32[2];
  uint16_t v16[4];
  } i;
  i.v16[0] = e[0];
  i.v16[1] = e[1];
  i.v16[2] = e[2];
  i.v16[3] = e[3];

  transpose_4x16_in(i.v32);

  gfs[0] = (i.v32[0] & mask)     |((i.v32[1] & mask)<<1);
  gfs[1] = ((i.v32[0]>>1) & mask)| (i.v32[1] &(mask<<1));
  gfs[2] = ((i.v32[0]>>2) & mask)|(((i.v32[1]>>2) & mask)<<1);
  gfs[3] = ((i.v32[0]>>3) & mask)|(((i.v32[1]>>3) & mask)<<1);
}


static inline void gfu32_to_gf_x4( uint16_t * gfs , const uint32_t * egfs )
{
  uint32_t mask = 0x11111111;

  union{
  uint32_t mat[2];
  uint16_t m16[4];
  } t;

  uint32_t e0 = egfs[0]&mask;
  uint32_t e1 = egfs[1]&mask;
  uint32_t e2 = egfs[2]&mask;
  uint32_t e3 = egfs[3]&mask;

  t.mat[0] = (e0)  ^ (e1<<1) ^ (e2<<2) ^ (e3<<3);
  t.mat[1] = ((egfs[0]^e0)>>1) ^ (egfs[1]^e1) ^ ((egfs[2]^e2)<<1) ^ ((egfs[3]^e3)<<2);

  transpose_16x4_in(t.mat);
  gfs[0] = t.m16[0];
  gfs[1] = t.m16[1];
  gfs[2] = t.m16[2];
  gfs[3] = t.m16[3];
}




///////////////////////////////





#endif

