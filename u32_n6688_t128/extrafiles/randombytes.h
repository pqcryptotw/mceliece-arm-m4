#ifndef _RANDOMBYTES_H_
#define _RANDOMBYTES_H_

#if !defined( STM32F4 )

#include "stdlib.h"

static inline void randombytes(unsigned char *out,unsigned len) {
	for(unsigned i=0;i<len;i++) out[i]=rand()&0xff;
	//for(unsigned i=0;i<s;i++) out[i] = 0;
}

#endif

#endif
