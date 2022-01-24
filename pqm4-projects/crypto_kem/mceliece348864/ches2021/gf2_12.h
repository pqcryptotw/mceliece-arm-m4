
#ifndef _GF_2_12_H_
#define _GF_2_12_H_

#include "stdint.h"
typedef uint16_t gf;


// gf_2_12 = gf_2[x]/x^12 + x^3 + 1


gf gf2_12_mul(gf in0, gf in1);

gf gf2_12_inv(gf in);


/* input: field element den, num */
/* return: (num/den) */
gf gf2_12_frac(gf den, gf num);



#endif
