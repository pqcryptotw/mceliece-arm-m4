
#ifndef _GF212_64_H_
#define _GF212_64_H_

#include "stdint.h"
typedef uint16_t gf;


// input: in0, in1 in GF((2^12)^64)
// GF(2^12)^64 := GF(2^12)[x]/ 877 X^9 + 2888 X^7 + 1781 X^5 + 373
// output: out = in0*in1

void GF212_64_mul(gf *out, gf *in0, gf *in1);




#endif
