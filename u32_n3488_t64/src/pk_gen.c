/*
  This file is for public-key generation
*/

#include "pk_gen.h"

#include "params.h"
#include "util.h"
#include "vec32.h"

#include "run_config.h"

#include <stdint.h>



/////////////////////


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


static void to_bitslicing_2x(uint32_t prod32[][GFBITS], uint32_t const32[][GFBITS], const uint32_t * in)
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
    for(int j=0;j<GFBITS;j++) prod32[i][j] = mat0[j];
    for(int j=0;j<GFBITS;j++) const32[i][GFBITS-1-j] = mat1[j];  // !!!!
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




static inline uint32_t equal_mask(int x, int y)
{
  uint32_t mask;
  mask = x ^ y;
  mask -= 1;
  mask >>= 31;
  mask = -mask;
  return mask;
}


static inline
void generate_identity( uint32_t *mat , uint16_t * idx , int n )
{
  int mat_width = (n+31)>>5;
  for(int i=0;i<n;i++) {
    uint32_t mask = ((uint32_t)1)<<(idx[i]&31);
    int c = idx[i]>>5;
    for(int j=0;j<mat_width;j++) { mat[i*mat_width+j] = mask&equal_mask(c,j); }
  }
}


static const int nblocks_H = (SYS_N + 31) / 32;  // (3488+31)/32 = 109
static const int nblocks_I = (GFBITS * SYS_T + 31) / 32;  // 12*64/32 = 24



#include "matrix_lup.h"
#include "matrix_mul.h"


///////////////////////////////////////////////////////////////


#include "sk_util_n4096_t64.h"

#include "sort_uint32.h"

static
int gen_irr_perm_bs( uint32_t prod32[][GFBITS] , uint32_t consts32[][GFBITS] , uint32_t * perm , const unsigned char * irr )
{
	int i;
	// compute the inverses of the secret \alphas
	calc_1_over_g_sq( prod32, irr );

	// reorder with the secret perm
	uint32_t payload[1<<GFBITS];

	de_bitslicing(payload, prod32);
	for (i = 0; i < (1 << GFBITS); i++) { payload[i] |= (i<<GFBITS); }
	sort_with_payload(1<<GFBITS,perm,payload);

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



#if 768==PK_NROWS
    #define PK_NR_2POW 1024
#else
    error: no defined PK_NR_2POW  !!!!
#endif





#if defined(_PK_IN_FLASH_)
#include "flashwrite.h"
#endif

int pk_gen(unsigned char * pk, const unsigned char * irr, uint32_t * perm, uint64_t *pivot)
{
#if defined(_PK_IN_FLASH_)
	if( (uint32_t *)pk != flash_address() ) return -1;
#endif

	// compute the inverses of the secret \alphas
	uint32_t prod32[128][GFBITS];  // 6144 bytes
	// reorder with the secret perm
#if defined(_USE_SK_AS_STACK_)
	uint32_t (*consts32)[GFBITS] = (void*)(pivot+1);   // 6144 bytes
#else
	uint32_t consts32[128][GFBITS];   // 6144 bytes
#endif

	int r;
	r  = gen_irr_perm_bs( prod32 , consts32 , perm , irr );
	if(r) return r;

	// calcuate the inverse matrix for systematiclizing the public matrix
	uint32_t __mat[ GFBITS * SYS_T * nblocks_I ]; // 768*24*4 = 73,728 bytes
	uint32_t *inv_M = __mat;
	fill_in_matrix( inv_M , prod32, consts32 , nblocks_I );

	uint32_t *buffer_2048u32 = perm+(1<<(GFBITS-1));
	uint16_t *p_op = (uint16_t*)buffer_2048u32;
	uint32_t * buffer_1664u32 = (uint32_t*)(p_op + PK_NROWS);

#if defined(_F_PARAM_)
	do {

	uint32_t *extra_alpha = buffer_1664u32;  // 768 x uint32_t
	fill_in_matrix( extra_alpha , &prod32[nblocks_I], &consts32[nblocks_I] , 1 );

	//uint16_t idx_cols_last32[32];
	uint16_t *idx_cols_last32 = (uint16_t*)(extra_alpha+PK_NROWS);
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
	//uint16_t idx[64];
	uint16_t * idx = idx_cols_last32+32;
	uint16_t *pi = (uint16_t*)perm;
	adjust_perm( &pi[PK_NROWS-32] , idx , idx_cols_last32 );
	adjust_cols( &prod32[nblocks_I-1] , &consts32[nblocks_I-1] , idx );
	} while(0);
#else  // defined(_F_PARAM_)
	r = lup_decompose( inv_M , p_op , PK_NROWS );
	if( r ) return r;

	// output pivot
	*pivot = 0xffffffff;
#endif  // defined(_F_PARAM_)




#if defined( GEN_PK_T_WIDTH )


#if defined( _PK_IN_FLASH_ )
	flash_open();
#endif

	pi_to_weight_extbuffer( p_op, PK_NROWS , buffer_1664u32 );


#if defined(_USE_CCM_IF_STM32F4_)
	uint32_t * ccm = 0x10000000;   // use SRAM2  section: "ccm"
#else
	uint32_t __ccm[16384];   // simulate a 64KB CCM here
	uint32_t * ccm =  __ccm;
#endif


#if defined(_APPLY_LUP_)

	const int width_pkmat = nblocks_H - nblocks_I;
	const int width_submat = GEN_PK_T_WIDTH;  // 16384/768=21.333  ,i.e., 64 KB ~ 20x32 columns

#if 1 == GEN_PK_T_WIDTH
	(void) ccm;  // use ccm
	uint32_t * mat = buffer_1664u32+PK_NROWS/2;
	uint32_t * mat2 = mat;
#else
	uint32_t * mat = ccm;
	uint32_t * mat2 = mat;
#endif

     for(int l=nblocks_I;l<nblocks_H;l+=width_submat) {
	int len = width_submat;
	if( (nblocks_H-l) < width_submat ) len = (nblocks_H-l);

        fill_in_matrix( mat , &prod32[l] , &consts32[l] , len );

	apply_P_by_sort_extbuffer( mat , len , p_op , PK_NROWS , PK_NR_2POW , (uint16_t*)buffer_1664u32 );
	apply_invL_with_L( mat2 , len,  inv_M , PK_NROWS );
	apply_invU_with_U( mat2 , len,  inv_M , PK_NROWS );

	for (int row = 0; row < PK_NROWS; row++) {
		for(int k=0;k<len;k++) {
#if defined( _PK_IN_FLASH_ )
			flash_program_32bits( (row*width_pkmat+l-nblocks_I+k)*4 , mat2[row*len+k] );
#else
			store4(pk+(row*width_pkmat+l-nblocks_I+k)*4 , mat2[row*len+k] );
#endif
		}
	}
    }


#elif defined(_USE_P_)  // defined(_APPLY_LUP_)

	LU_to_invL_x_invU_extbuffer( inv_M , PK_NROWS , buffer_1664u32 );  // buffer size: one row

	const int width_pkmat = nblocks_H - nblocks_I;
	const int width_submat = GEN_PK_T_WIDTH;

	uint32_t * mat  = ccm;
	uint32_t * mat2 = mat + width_submat*PK_NROWS;
	//uint32_t one_col_u16[PK_NROWS/2];
	//uint32_t buffer_matmul[32*14];   // buffer for strassen

     for(int l=nblocks_I;l<nblocks_H;l+=width_submat) {
	int len = width_submat;
	if( (nblocks_H-l) < width_submat ) len = (nblocks_H-l);

        fill_in_matrix( mat , &prod32[l] , &consts32[l] , len );

	apply_P_by_sort_extbuffer( mat , len , p_op , PK_NROWS , PK_NR_2POW , buffer_1664u32 );

	if( 0 == (len&1) )
		matrix_mul_64_ext_buffer( mat2 , inv_M , nblocks_I , mat , len , buffer_1664u32 );
//		matrix_mul_64( mat2 , inv_M , nblocks_I , mat , len );
	else
		matrix_mul_32( mat2 , inv_M , nblocks_I , mat , len );

	for (int row = 0; row < PK_NROWS; row++) {
		for(int k=0;k<len;k++) {
#if defined( _PK_IN_FLASH_ )
			flash_program_32bits( (row*width_pkmat+l-nblocks_I+k)*4 , mat2[row*len+k] );
#else
			store4(pk+(row*width_pkmat+l-nblocks_I+k)*4 , mat2[row*len+k] );
#endif
		}
	}
    }

#else   // ! ( defined(_APPLY_LUP_) || defined(_USE_P_) )
error :  ! ( defined(_APPLY_LUP_) || defined(_USE_P_) )
#endif  //  defined(_APPLY_LUP_)


#if defined( _PK_IN_FLASH_ )
	flash_close();
#endif




#else
//#if defined( GEN_PK_T_WIDTH )
// generation pk without segmentation


	const int width_pkmat = nblocks_H - nblocks_I;
	uint32_t pkmat[PK_NROWS*width_pkmat];
        fill_in_matrix( pkmat , &prod32[nblocks_I] , &consts32[nblocks_I] , width_pkmat );

#if defined(_APPLY_LUP_)
	pi_to_weight( p_op , PK_NROWS );
	apply_P_by_sort( pkmat , width_pkmat , p_op , PK_NROWS , PK_NR_2POW );
	apply_invL_with_L( pkmat , width_pkmat , inv_M , PK_NROWS );
	apply_invU_with_U( pkmat , width_pkmat , inv_M , PK_NROWS );

	for(int i=0;i<PK_NROWS*width_pkmat;i++) store4(pk+i*4, pkmat[i]);
#elif defined(_USE_P_)

	// calcuate inverse matrix
	pi_to_weight( p_op , PK_NROWS );
	LU_to_invL_x_invU( inv_M , PK_NROWS );

	uint32_t mat00[PK_NROWS*width_pkmat];
	apply_P_by_sort( pkmat , width_pkmat , p_op , PK_NROWS , PK_NR_2POW );
	matrix_mul( mat00 , inv_M , nblocks_I , pkmat , width_pkmat );

	for(int i=0;i<PK_NROWS*width_pkmat;i++) store4(pk+i*4, mat00[i] );

#else
	// calcuate inverse matrix
	uint32_t mat00[PK_NROWS*width_pkmat];
	generate_identity( mat00 , p_op , PK_NROWS);
	apply_invL_with_L( mat00 , nblocks_I, inv_M , PK_NROWS );
	apply_invU_with_U( mat00 , nblocks_I, inv_M , PK_NROWS );
	for(int i=0;i< PK_NROWS * nblocks_I;i++) { inv_M[i] = mat00[i]; }

	matrix_mul( mat00 , inv_M , nblocks_I , pkmat , width_pkmat );

	for(int i=0;i<PK_NROWS*width_pkmat;i++) store4(pk+i*4, mat00[i] );

#endif  // defined(_APPLY_LUP_)


#endif  //#if defined( GEN_PK_T_WIDTH )


	return 0;
}




////////////////////////////////////////////////////////////////

