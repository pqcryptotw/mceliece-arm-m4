
#ifndef BM_GFU32_H
#define BM_GFU32_H

#include "stdint.h"

void bm_gfu32( uint16_t poly[], uint16_t seq256[] , unsigned seq_len );



#include "params.h"

void bm_64_gfu32_bs( uint32_t poly_128 [][GFBITS], uint32_t seq_256 [][ GFBITS ] );

void bm_96_gfu32_bs( uint32_t poly_96 [][GFBITS], uint32_t seq_192 [][ GFBITS ] );

void bm_119_gfu32_bs( uint32_t poly_128 [][GFBITS], uint32_t seq_256 [][ GFBITS ] );

void bm_128_gfu32_bs( uint32_t poly_128 [][GFBITS], uint32_t seq_256 [][ GFBITS ] );



#endif

