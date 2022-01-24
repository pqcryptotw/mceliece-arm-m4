/*
  This file is for public-key generation
*/

#include "pk_gen.h"

#include "sort_uint32.h"
#include "sort_int32.h"
#include "params.h"

#include "util.h"
#include "vec32.h"

#include "run_config.h"

#include <stdint.h>


#if defined(_PK_IN_FLASH_)
#include "flashpk_stm32l4r5.h"
#endif


static void de_bitslicing(uint32_t * out, const uint32_t in[][GFBITS])
{
  const int len = ((1<<GFBITS)/32);

  uint32_t mat[32] = {0};
  for(int i=0;i<len;i++) {
    for(int j=0;j<GFBITS;j++) mat[j] = in[i][j];
    transpose_32x32(out, mat);
    out += 32;
  }
}


static void to_bitslicing_2x(uint32_t out0[][GFBITS], uint32_t out1[][GFBITS], const uint32_t * in)
{
  const int len = ((1<<GFBITS)/32);
  uint32_t mat0[32];
  uint32_t mat1[32];

  for(int i=0;i<len;i++) {
    for(int j=0;j<32;j++) {
      mat0[j] = in[i*32+j];
      mat1[j] = in[i*32+j]>>GFBITS;
    }
    transpose_32x32_in(mat0);
    transpose_32x32_in(mat1);
    for(int j=0;j<GFBITS;j++) out0[i][j] = mat0[j];
    for(int j=0;j<GFBITS;j++) out1[i][GFBITS-1-j] = mat1[j];  // !!!!
  }
}



#if defined(_F_PARAM_)

static inline uint16_t same_mask(uint16_t x, uint16_t y)
{
  uint32_t mask;
  mask = x ^ y;
  mask -= 1;
  return (mask>>16)&0xffff;
}


static
void adjust_perm( uint16_t * perm , uint16_t * idx , const uint16_t idx_cols[] )
{
  for(int i=0;i<64;i++) idx[i]=i;
  for (int j = 0;   j < 32; j++) {
    for (int k = j+1; k < 64-(31-j); k++) {
      uint16_t mask = same_mask(k, idx_cols[j]);
      uint16_t diff = (perm[ j ] ^ perm[ k ]) & mask;
      perm[j] ^= diff;
      perm[k] ^= diff;
      uint16_t d_idx = (idx[j]^idx[k]) & mask;
      idx[j] ^= d_idx;
      idx[k] ^= d_idx;
    }
  }
}


static
void adjust_cols( uint32_t prod32[][GFBITS], uint32_t const32[][GFBITS], uint16_t idx_cols[] )
{
  for(int i=0;i<GFBITS;i++) {
    uint64_t src0 = ((uint64_t)prod32[0][i])|(((uint64_t)prod32[1][i])<<32);
    uint64_t src1 = ((uint64_t)const32[0][i])|(((uint64_t)const32[1][i])<<32);
    uint32_t d0,d1;
    d0 = 0;
    d1 = 0;
    for(int j=0;j<32;j++) {
      d0 ^= ((src0>>idx_cols[32+j])&1)<<j;
      d1 ^= ((src1>>idx_cols[32+j])&1)<<j;
    }
    prod32[1][i] = d0;
    const32[1][i] = d1;
  }
}


#endif


static const int nblocks_H = (SYS_N + 31) / 32;  //
static const int nblocks_I = (GFBITS * SYS_T + 31) / 32;  //


#include "sk_util_n8192_t96.h"

#include "matrix_lup.h"


static
int gen_irr_perm_bs( uint32_t prod32[][GFBITS] , uint32_t consts32[][GFBITS] , uint32_t * perm , const unsigned char * irr )
{
	int i;
	// compute the inverses of the secret \alphas
	calc_1_over_g( prod32, irr );

	// reorder with the secret perm
	uint32_t payload[1<<GFBITS];

	de_bitslicing(payload, prod32);
	for (i = 0; i < (1 << GFBITS); i++) { payload[i] |= (i<<GFBITS); }
	uint32_sort_payload(perm,payload,1<<GFBITS);

	for (i = 1; i < (1<<GFBITS);i++) { if(perm[i-1]==perm[i]) { return -1; } }

	// output perm. turn perm output into 16-bits form
	uint16_t *pi = (uint16_t*) perm;
	for(i=0;i<(1<<GFBITS);i++) { pi[i] = (payload[i]>>GFBITS)&0xffff; }

	to_bitslicing_2x(prod32, consts32, payload);
	return 0;
}


static
void fill_in_matrix( uint32_t mat0[] , const uint32_t prod32[][GFBITS] , const uint32_t consts32[][GFBITS], int len_u32 )
{
	uint32_t tmp[GFBITS];
	for (int j = 0; j < len_u32; j++) {
		for (int k=0;k<GFBITS;k++) { tmp[k] = prod32[j][k]; }
		for (int k=0;k<GFBITS;k++) { mat0[ k *len_u32+ j ] = tmp[ k ]; }
		for (int i = 1; i < SYS_T; i++) {
			vec32_mul(tmp,tmp,consts32[j]);
			for (int k = 0; k < GFBITS; k++) { mat0[ (i*GFBITS + k) *len_u32+ j ] = tmp[ k ]; }
		}
	}
}








int pk_gen(unsigned char * pk, const unsigned char * irr, uint32_t * perm, uint64_t *pivot)
{
	// compute the inverses of the secret \alphas
	uint32_t prod32[256][GFBITS];  //
	uint32_t consts32[256][GFBITS];   //

	int r;
        r  = gen_irr_perm_bs( prod32 , consts32 , perm , irr );
        if(r) return r;

	// calcuate the inverse matrix for systematiclizing the public matrix
	uint32_t __mat[ GFBITS * SYS_T * nblocks_I ];
	uint32_t *inv_M = __mat;
	fill_in_matrix( inv_M , prod32, consts32 , nblocks_I );

	uint16_t p_op[PK_NROWS];
#if defined(_F_PARAM_)
	{
	uint32_t extra_alpha[PK_NROWS];
	fill_in_matrix( extra_alpha , &prod32[nblocks_I], &consts32[nblocks_I] , 1 );

	uint16_t idx_cols_last32[32];
	r = lup_decompose_2( inv_M , extra_alpha , p_op ,idx_cols_last32 , PK_NROWS );
	if( r ) return r;

	// output pivot
	{
		uint64_t one = 1;
		uint64_t rp = 0;
		for(int i=0;i<32;i++) rp ^= one<<idx_cols_last32[i];
		*pivot = rp;
	}
	// adjust perm and prod32/consts32
	uint16_t idx[64];
	uint16_t *pi = (uint16_t*)perm;
	adjust_perm( &pi[PK_NROWS-32] , idx , idx_cols_last32 );
	adjust_cols( &prod32[nblocks_I-1] , &consts32[nblocks_I-1] , idx );
	}
#else  // defined(_F_PARAM_)

	r = lup_decompose( inv_M , p_op , PK_NROWS );
	if( r ) return r;

	// output pivot
	*pivot = 0xffffffff;
#endif


#if 768==PK_NROWS
    #define PK_NR_2POW 1024
#elif 1248==PK_NROWS
    #define PK_NR_2POW 2048
#else
    error: no defined PK_NR_2POW  !!!!
#endif
	// generate pk
	const int width_pkmat = nblocks_H - nblocks_I;  // 144 - 39 = 105
	pi_to_weight( p_op , PK_NROWS );

#if defined(GEN_PK_T_WIDTH)
#if defined(_PK_IN_FLASH_)
	flashpk_open_and_erase( (width_pkmat*PK_NROWS*4+4095)>>12 );

#define _PK_ROW_64BIT_ALIGNED_  0

#if 0 == _PK_ROW_64BIT_ALIGNED_
	uint32_t flashpk_start[PK_NROWS/2];
#endif
	uint32_t flashpk_remain[PK_NROWS];

#endif  // defined(_PK_IN_FLASH_)
	// generate pk with segmantation
	const int width_submat = GEN_PK_T_WIDTH;  // 21
	uint32_t pkmat[PK_NROWS*width_submat];    // 1248*21*4=104832

    for(int l=0;l<width_pkmat;l+=width_submat) {
	int len = width_submat;
	if( (width_pkmat-l) < width_submat ) len = (width_pkmat-l);

	fill_in_matrix( pkmat , &prod32[nblocks_I+l] , &consts32[nblocks_I+l] , len );

	apply_P_by_sort( pkmat , len , p_op , PK_NROWS , PK_NR_2POW );
	apply_invL_with_L( pkmat , len,  inv_M , PK_NROWS );
	apply_invU_with_U( pkmat , len,  inv_M , PK_NROWS );


#if defined(_PK_IN_FLASH_)

	for (int row = 0; row < PK_NROWS; row++) {
		uint32_t dest_address = (row*width_pkmat+l)*4;
		uint32_t row_idx = 0;
		uint32_t row_len = len;

		if( dest_address&4 ) {
#if 0 == _PK_ROW_64BIT_ALIGNED_
			if( (0==l) ) {
				flashpk_start[row/2] = pkmat[ row*len ];
			} else
#endif  // 0 == _PK_ROW_64BIT_ALIGNED_
			{
				uint64_t dd = flashpk_remain[row];
				dd |= ((uint64_t)pkmat[row*len])<<32;
				flashpk_program_2words(dest_address-4,dd);
			}
			dest_address += 4;
			row_idx += 1;
			row_len --;
		}
		flashpk_program_n_2words( dest_address , &pkmat[row*len+row_idx] , row_len>>1 );
		if( 1 == (row_len&1) ) flashpk_remain[row] = pkmat[row*len+len-1];
	}
    }
#if 0 == _PK_ROW_64BIT_ALIGNED_
	for(int row=0; row<PK_NROWS;row+=2) {
		uint32_t dest_address = (row*width_pkmat+width_pkmat-1)*4;
		uint64_t dd = flashpk_remain[row];
		dd |= ((uint64_t)flashpk_start[row>>1])<<32;
		flashpk_program_2words(dest_address,dd);
	}
#endif  // 0 == _PK_ROW_64BIT_ALIGNED_
	flashpk_close();
#else   // deined(_PK_IN_FLASH_)
	for (int row = 0; row < PK_NROWS; row++) {
		for(int k=0;k<len;k++) {
			store4(pk+(row*width_pkmat+l+k)*4 , pkmat[row*len+k] );
		}
	}
    }
#endif  // defined(_PK_IN_FLASH_)

#else   // defined(GFN_PK_T_WIDTH)
	uint32_t pkmat[PK_NROWS*width_pkmat];  // 1248*105*4=524160
	fill_in_matrix( pkmat , &prod32[nblocks_I] , &consts32[nblocks_I] , width_pkmat );

	apply_P_by_sort( pkmat , width_pkmat , p_op , PK_NROWS , PK_NR_2POW );
	apply_invL_with_L( pkmat , width_pkmat , inv_M , PK_NROWS );
	apply_invU_with_U( pkmat , width_pkmat , inv_M , PK_NROWS );

	// output pk
	for(int i=0;i<PK_NROWS*width_pkmat;i++) store4(pk+i*4, pkmat[i]);
#endif  // defined(GEN_PK_T_WIDTH)
	return 0;
}


