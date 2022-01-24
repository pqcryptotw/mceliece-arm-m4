/*
  This file is for functions for field arithmetic
*/

#ifndef GF_2_13_H
#define GF_2_13_H

#include <stdint.h>


typedef uint16_t gf;


gf gf2_13_mul(gf, gf);
gf gf2_13_squ(gf);
gf gf2_13_inv(gf);
gf gf2_13_frac(gf den,gf num);

gf gf2_13_inv_fermat(gf);
gf gf2_13_inv_extgcd(gf);



#endif

