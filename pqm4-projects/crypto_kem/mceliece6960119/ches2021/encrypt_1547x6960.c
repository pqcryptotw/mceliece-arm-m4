/*
  This file is for Niederreiter encryption
*/

#include "encrypt.h"

#include "randombytes.h"
#include "params.h"
#include "util.h"
//#include "uint16_sort.h"
#include "uint16_qsort.h"

#include <stdint.h>

#include "run_config.h"



#if !( defined( _M4_ASM_ ) && ( 3488==SYS_N || 4608==SYS_N )  )

static
void write_e(unsigned char *e, const uint16_t * index )
{

	uint32_t mask;
	uint32_t val[ SYS_T ];
	uint32_t e_int[ (SYS_N+31)/32 ];


	for (int j = 0; j < SYS_T; j++)
		val[j] = 1 << (index[j] & 31);

	for (int i = 0; i < (SYS_N+31)/32; i++) 
	{
		e_int[i] = 0;

		for (int j = 0; j < SYS_T; j++)
		{
			mask = i ^ (index[j] >> 5);
			mask -= 1;
			mask >>= 31;
			mask = -mask;

			e_int[i] |= val[j] & mask;
		}
	}

	// output
#if 0 == (SYS_N%32)
	for (int i = 0; i < SYS_N/32; i++) store4(e+i*4, e_int[i]);
#else
	// only parameter is SYS_N = 6960
	int i;
	for (i = 0; i < (SYS_N)/32; i++) {
		store4(e, e_int[i]);
		e += 4;
	}
	for (int j=0; j<(SYS_N&31); j+=8)
		e[j/8] = (e_int[i]>>j) & 0xff;
#endif

}

#endif



/* output: e, an error vector of weight SYS_T */
//static
void gen_e(unsigned char *e)
{
#if (1<<GFBITS) != SYS_N
#define TAU (2*SYS_T)
	int count;
#else
#define TAU (SYS_T)
#endif
	union {
		uint16_t nums[TAU];
		unsigned char bytes[TAU*sizeof(uint16_t)];
	} buf;

	uint16_t ind[ SYS_T];

	while (1)
	{
		randombytes(buf.bytes, sizeof(buf.bytes));

#if TAU == 2*SYS_T
		count = 0;
		for(int i=0; i<TAU && count < SYS_T ;i++) {
			buf.nums[i] = load_gf(buf.bytes + i*2);
			if(buf.nums[i] < SYS_N) ind[ count++ ] = buf.nums[i];
		}
		if( count < SYS_T ) continue;
#else
		for(int i=0; i<SYS_T; i++) ind[i] = load_gf(buf.bytes + i*2);
#endif

		// check for repetition

		//uint16_sort(ind, SYS_T);
		uint16_qsort(ind, 0, SYS_T-1);

		int eq = 0;
		for (int i = 1; i < SYS_T; i++)
			if (ind[i-1] == ind[i])
				eq = 1;

		if (eq == 0)
			break;
	}

#if defined( _M4_ASM_ ) && ( 3488==SYS_N )
	idx_to_vec_64_3488_asm( e , ind );
#elif defined( _M4_ASM_ ) && ( 4608==SYS_N )
	idx_to_vec_96_4608_asm( e , ind );
#else
	write_e( e , ind );
#endif
}


static
void _gf2_mat_madd( uint8_t * c , const uint8_t *row_mat , int vec_len , int n_vec , const uint8_t * col_vec )
{
	const uint32_t *col_vec32 = (const uint32_t*) col_vec;

	int vec_n_u32 = vec_len/4;
	int vec_rem = vec_len&3;

	for(int i=0;i<n_vec;i++) {
		uint32_t b = 0;
		const uint32_t *row_ptr = (const uint32_t*)(row_mat + vec_len*i);
		for(int j=0;j<vec_n_u32;j++) b ^= row_ptr[j]&col_vec32[j];
		for(int j=0;j<vec_rem;j++)   b ^=  ((const uint8_t*)(&row_ptr[vec_n_u32]))[j] & ((const uint8_t*)(&col_vec32[vec_n_u32]))[j];

		b ^= b>>16;
		b ^= b>>8;
		b ^= b>>4;
		b ^= b>>2;
		b ^= b>>1;
		b &= 1;;

		c[i/8] ^= (b<<(i%8));
	}
}


/* input: public key pk , error vector e */
/* output: syndrome s = pk * e */
void encrypt(unsigned char *s, const unsigned char *pk, const unsigned char *e)
{

#ifdef KAT
  {
    int k;
    printf("encrypt e: positions");
    for (k = 0;k < SYS_N;++k)
      if (e[k/8] & (1 << (k&7)))
        printf(" %d",k);
    printf("\n");
  }
#endif
	//syndrome(s, pk, e);

	for (int i = 0; i < SYND_BYTES; i++) s[i] = e[i];
	// 1547 = 32x48 + 11
	s[SYND_BYTES-1] &= 0x7;  // remainder: 3 bits

	// right shit 3 bits
	union {
	uint32_t v32[((6960-1547)+31)/32];  // 169x32+5 -> 170
	uint8_t v8[((6960-1547)+7)/8];      // 677
	} ee;
	ee.v32[169]=0;
	for(int i=SYND_BYTES-1;i<SYS_N/8-1;i++)
		ee.v8[i-(SYND_BYTES-1)] = (e[i]>>3)|(e[i+1]<<5);
	ee.v8[SYS_N/8 - SYND_BYTES] = e[SYS_N/8-1]>>3;

	_gf2_mat_madd( s , pk , (PK_NCOLS+7)/8 , PK_NROWS , ee.v32 );  // W:677 bytes  H:1547
}

