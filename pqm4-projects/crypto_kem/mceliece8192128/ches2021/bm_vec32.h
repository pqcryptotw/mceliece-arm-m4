
#ifndef BM_VEC32_H
#define BM_VEC32_H



#include <stdint.h>
#include "params.h"



void bm_vec32( uint32_t poly [][GFBITS], uint32_t seq [][ GFBITS ] , unsigned len_seq );


//////////////////////


static inline
void bm_64_vec32(uint32_t poly [][GFBITS], uint32_t seq [][ GFBITS ]) { bm_vec32(poly,seq,128); }

static inline
void bm_96_vec32(uint32_t poly [][GFBITS], uint32_t seq [][ GFBITS ]) { bm_vec32(poly,seq,192); }

static inline
void bm_128_vec32( uint32_t poly_128 [][GFBITS], uint32_t seq_256 [][ GFBITS ] ) { bm_vec32(poly_128,seq_256,256); }



#endif

