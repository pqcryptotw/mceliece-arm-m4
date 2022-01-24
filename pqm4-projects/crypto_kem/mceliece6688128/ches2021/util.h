/*
  This file is for loading/storing data in a little-endian fashion
*/

#ifndef UTIL_H
#define UTIL_H

#include <stdint.h>

#include "params.h"


static inline void store_gf(unsigned char *dest, uint16_t a)
{
	dest[0] = a & 0xFF;
	dest[1] = a >> 8;
}

static inline uint16_t load_gf(const unsigned char *src)
{
	uint16_t a;

	a = src[1];
	a = src[0] | (a<<8);

	return a & GFMASK;
}

static inline uint32_t load4(const unsigned char *src)
{
	uint32_t a;

	a = src[3];
	a = src[2] | (a<<8);
	a = src[1] | (a<<8);
	a = src[0] | (a<<8);
	return a;
}

static inline void store4(unsigned char *out, uint32_t in)
{
	out[0] = (in >> 0x00) & 0xFF;
	out[1] = (in >> 0x08) & 0xFF;
	out[2] = (in >> 0x10) & 0xFF;
	out[3] = (in >> 0x18) & 0xFF;
}

static inline void store8(unsigned char *out, uint64_t in)
{
	out[0] = (in >> 0x00) & 0xFF;
	out[1] = (in >> 0x08) & 0xFF;
	out[2] = (in >> 0x10) & 0xFF;
	out[3] = (in >> 0x18) & 0xFF;
	out[4] = (in >> 0x20) & 0xFF;
	out[5] = (in >> 0x28) & 0xFF;
	out[6] = (in >> 0x30) & 0xFF;
	out[7] = (in >> 0x38) & 0xFF;
}

static inline uint64_t load8(const unsigned char * in)
{
	uint64_t ret = load4(in);
	ret = load4(in+4) | (ret << 32);
	return ret;
}

//////////////////////////


#include "transpose.h"

static inline void bs16_to_bs32( uint32_t * out , const uint16_t * in_l, const uint16_t * in_h, unsigned len )
{
	unsigned i;
	union{
	uint16_t v16[2];
	uint32_t v;
	} t;
	for(i=0;i<len;i++) {
		t.v16[0] = in_l[i];
		t.v16[1] = in_h[i];
		out[i] = t.v;
	}
}

static inline void bs32_to_bs16( uint16_t * out_l , uint16_t * out_h , const uint32_t * in, unsigned len )
{
	unsigned i;
	union{
	uint16_t v16[2];
	uint32_t v;
	} t;
	for(i=0;i<len;i++) {
		t.v = in[i];
		out_l[i] = t.v16[0];
		out_h[i] = t.v16[1];
	}
}

static inline void gf_to_bs32( uint32_t *out, const uint16_t * gfs )
{
	uint32_t mat[16];
	uint16_t *mat16 = (uint16_t*)&mat[0];
	for(int i=0;i<32;i++) mat16[i] = gfs[i];

	transpose_16x16( mat16 , mat16 );
	transpose_16x16( (mat16+16) , (mat16+16) );

	bs16_to_bs32( out , mat16 , mat16+16 , GFBITS );
}

static inline void bs32_to_gf( uint16_t *out, const uint32_t * in )
{
	uint16_t *mat16 = out;
	bs32_to_bs16(mat16, mat16+16, in, GFBITS );
	for(int i=GFBITS;i<16;i++) mat16[i] = 0;
	for(int i=GFBITS;i<16;i++) mat16[16+i] = 0;

	transpose_16x16( mat16 , mat16 );
	transpose_16x16( (mat16+16) , (mat16+16) );
}



static inline void bs32_to_bs64( uint64_t * out , const uint32_t * in_l, const uint32_t * in_h, unsigned len )
{
	unsigned i;
	union{
	uint32_t v32[2];
	uint64_t v;
	} t;
	for(i=0;i<len;i++) {
		t.v32[0] = in_l[i];
		t.v32[1] = in_h[i];
		out[i] = t.v;
	}
}


static inline void bs64_to_bs32( uint32_t * out_l, uint32_t * out_h, const uint64_t * in , unsigned len )
{
	unsigned i;
	union{
	uint32_t v32[2];
	uint64_t v;
	} t;
	for(i=0;i<len;i++) {
		t.v = in[i];
		out_l[i] = t.v32[0];
		out_h[i] = t.v32[1];
	}
}

/////////////////////////////////////////

static inline void irr_load_32x(uint32_t out[][GFBITS], const unsigned char * in, int len )
{
	// assert(0==(len&31));
	int i, j;
	uint32_t mat[16];
	uint16_t *mat16 = (uint16_t*)&mat[0];
	for(i=0;i<len;i+=32) {
		for(j=0;j<16;j++) mat16[j] =    load_gf(in + (i+j)*2);
		for(j=0;j<16;j++) mat16[16+j] = load_gf(in + (i+j+16)*2);
		transpose_16x16( mat16 , mat16 );
		transpose_16x16( (mat16+16), (mat16+16) );
		bs16_to_bs32( out[i>>5] , mat16 , mat16+16 , GFBITS );
	}
}

/////////////////////////////////////////

#if 0
static inline void irr_load(uint64_t * out, const unsigned char * in)
{
        int i, j;
        uint16_t irr[ SYS_T + 1 ];      

        for (i = 0; i < SYS_T; i++) irr[i] = load_gf(in + i*2);
        irr[ SYS_T ] = 1;

        for (i = 0; i < GFBITS; i++) 
                out[i] = 0;

        for (i = SYS_T; i >= 0; i--)
        for (j = 0; j < GFBITS; j++) 
        {
                out[j] <<= 1;
                out[j] |= (irr[i] >> j) & 1;
        }
}
#endif

// warning: can only handle the cases which SYS_T = 64 and 128.
static inline void irr_load_bs(uint64_t out[][GFBITS], const unsigned char * in)
{
	int i, j;
	uint16_t irr[ SYS_T ];
	for (i = 0; i < SYS_T; i++) irr[i] = load_gf(in + i*2);

	uint16_t mat16[32];
	uint32_t mat32[2*(GFBITS)];
	for(i=0;i<SYS_T;i+=64) {
		for(j=0;j<16;j++) mat16[j] = irr[i+j];
		for(j=0;j<16;j++) mat16[16+j] = irr[i+16+j];
		transpose_16x16( mat16 , mat16 );
		transpose_16x16( (mat16+16), (mat16+16) );
		bs16_to_bs32( mat32 , mat16 , mat16+16 , GFBITS );

		for(j=0;j<16;j++) mat16[j] = irr[i+32+j];
		for(j=0;j<16;j++) mat16[16+j] = irr[i+48+j];
		transpose_16x16( mat16 , mat16 );
		transpose_16x16( (mat16+16) , (mat16+16) );
		bs16_to_bs32( mat32 + GFBITS , mat16 , mat16+16 , GFBITS );

		bs32_to_bs64( out[i>>6] , mat32 , mat32 + GFBITS , GFBITS );
	}
}




#endif

