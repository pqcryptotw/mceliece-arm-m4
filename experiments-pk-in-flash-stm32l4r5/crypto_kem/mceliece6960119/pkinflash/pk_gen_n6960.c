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

// 6960 = 32 x 217 + 16
//static const int nblocks_H = (SYS_N + 31) / 32;  //  218

// GFBITS x SYS_T = 1547 = 32x48+11
static const int nblocks_I = (GFBITS * SYS_T + 31) / 32;  //  49

// 6960 - 1547 = 5413 = 32 x 169 + 5
static const int nblocks_PKMAT = (PK_NCOLS + 31)/32;  // 170

// 11
#define PK_NROWS_MOD32   (PK_NROWS%32)


#include "sk_util_n8192_t119.h"

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

	// align pk-mat part
#if 0 != PK_NROWS_MOD32

#define PKMAT_SHR     (32-PK_NROWS_MOD32)
	for(int i=nblocks_PKMAT-1;i>=0;i--) {
		for(int j=0;j<GFBITS;j++) { prod32[nblocks_I+i][j] = (prod32[nblocks_I+i][j]<<PKMAT_SHR)| (prod32[nblocks_I+i-1][j]>>PK_NROWS_MOD32);}
		for(int j=0;j<GFBITS;j++) { consts32[nblocks_I+i][j] = (consts32[nblocks_I+i][j]<<PKMAT_SHR)| (consts32[nblocks_I+i-1][j]>>PK_NROWS_MOD32);}
	}
	for(int j=0;j<GFBITS;j++) { prod32[nblocks_I+nblocks_PKMAT-1][j] &= (1<<(PK_NCOLS%32))-1; }
	for(int j=0;j<GFBITS;j++) { consts32[nblocks_I+nblocks_PKMAT-1][j] &= (1<<(PK_NCOLS%32))-1; }
#endif

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

	for(int i=0;i<GFBITS;i++) { prod32[nblocks_I-1][i] = (prod32[nblocks_I-1][i]<<PKMAT_SHR)| (prod32[nblocks_I-2][i]>>PK_NROWS_MOD32);  }
	for(int i=0;i<GFBITS;i++) { consts32[nblocks_I-1][i] = (consts32[nblocks_I-1][i]<<PKMAT_SHR)| (consts32[nblocks_I-2][i]>>PK_NROWS_MOD32);  }

	adjust_cols( &prod32[nblocks_I-1] , &consts32[nblocks_I-1] , idx );
	}
#else  // defined(_F_PARAM_)

	r = lup_decompose( inv_M , p_op , PK_NROWS );
	if( r ) return r;

	// output pivot
	*pivot = 0xffffffff;
#endif   // defined(_F_PARAM_)


//static const int nblocks_H = (SYS_N + 31) / 32;  //  218
//static const int nblocks_I = (GFBITS * SYS_T + 31) / 32;  //  49
// GFBITS x SYS_T = 1547 = 32x48+11
// 6960 = 32 x 217 + 16
// 6960 - 1547 = 5413 = 32 x 169 + 5



#if 768==PK_NROWS
    #define PK_NR_2POW 1024
#elif 1248==PK_NROWS || 1547==PK_NROWS || 1664==PK_NROWS
    #define PK_NR_2POW 2048
#else
    error: no defined PK_NR_2POW  !!!!
#endif
	// generate pk
	//const int width_pkmat = nblocks_H - nblocks_I + 1;  //  218-49+1 = 170
	pi_to_weight( p_op , PK_NROWS );

#if defined(GEN_PK_T_WIDTH)
#if defined(_PK_IN_FLASH_)
	flashpk_open_and_erase( (PK_NROWS*PK_ROW_BYTES+4095)>>12 );

	uint64_t flashpk_start[PK_NROWS];
	uint64_t flashpk_remain[PK_NROWS];

#endif  // defined(_PK_IN_FLASH_)

	// generate pk with segmantation
	const int width_submat = GEN_PK_T_WIDTH;  // 22
	uint32_t pkmat[PK_NROWS*GEN_PK_T_WIDTH];    // 147*22*4=136,136

    for(int l=0;l<nblocks_PKMAT;l+=width_submat) {
	int last_block = (nblocks_PKMAT<=l+width_submat)?1:0;
	int len = (last_block)? (nblocks_PKMAT-l):width_submat;

	fill_in_matrix( pkmat , &prod32[nblocks_I+l] , &consts32[nblocks_I+l] , len );

	apply_P_by_sort( pkmat , len , p_op , PK_NROWS , PK_NR_2POW );
	apply_invL_with_L( pkmat , len,  inv_M , PK_NROWS );
	apply_invU_with_U( pkmat , len,  inv_M , PK_NROWS );

	// write pk
#if !defined(_PK_IN_FLASH_)
	if( last_block ) {
		for (int row = 0; row < PK_NROWS; row++) {
			uint8_t * pk_row = (uint8_t*) &pkmat[row*len];
			uint8_t * pkdest_row = pk + row * PK_ROW_BYTES + l*4;
			int k;
			for(k=0;k<PK_ROW_BYTES-l*4;k++) pkdest_row[k] = pk_row[k];
		}
	} else {
		for (int row = 0; row < PK_NROWS; row++) {
			uint8_t * pk_row = (uint8_t*) &pkmat[row*len];
			uint8_t * pkdest_row = pk + row * PK_ROW_BYTES + l*4;
			for(int k=0;k<len*4;k++) pkdest_row[k] = pk_row[k];
		}
	}
    }
#else  // defined(_PK_IN_FLASH_)
	if( 0==l ) {  // first block
		for (int row = 0; row < PK_NROWS; row++) {
			uint8_t * pk_row = (uint8_t*) &pkmat[row*len];
			int n = len*4;
			uint32_t dest_addr = row*PK_ROW_BYTES;

			uint8_t * ptr_dest = (uint8_t*) &flashpk_start[row-1];
			while( dest_addr&7 ) {
				ptr_dest[dest_addr&7] = pk_row[0];
				pk_row ++;
				n --;
				dest_addr++;
			}
			ptr_dest = (uint8_t*) &flashpk_remain[row];
			while( n >= 8 ) {
				for(int k=0;k<8;k++) ptr_dest[k] = pk_row[k];
				flashpk_program_2words(dest_addr,flashpk_remain[row]);
				pk_row += 8;
				n -= 8;
				dest_addr += 8;
			}
			if ( n ) {
				for(int k=0;k<n;k++) ptr_dest[k] = pk_row[k];
			}
		}
	}
	else if( last_block ) {
		for (int row = 0; row < PK_NROWS; row++) {
			uint8_t * pk_row = (uint8_t*) &pkmat[row*len];
			int n = PK_ROW_BYTES - l*4;
			uint32_t dest_addr = row*PK_ROW_BYTES + l*4;

			uint8_t * ptr_dest = (uint8_t*) &flashpk_remain[row];
			int write_flash = 0;
			while( dest_addr&7 ) {		// XXX: what if n < 8 - (dest_addr&7)
				write_flash = 1;
				ptr_dest[dest_addr&7] = pk_row[0];
				pk_row ++;
				n --;
				dest_addr++;
			}
			if(write_flash) flashpk_program_2words(dest_addr-8,flashpk_remain[row]);
			while( n >= 8 ) {
				for(int k=0;k<8;k++) ptr_dest[k] = pk_row[k];
				flashpk_program_2words(dest_addr,flashpk_remain[row]);
				pk_row += 8;
				n -= 8;
				dest_addr += 8;
			}
			ptr_dest = (uint8_t*) &flashpk_start[row];
			write_flash = 0;
			if( n ) {
				write_flash = 1;
				for(int k=0;k<n;k++) ptr_dest[k] = pk_row[k];
			}
			if(write_flash) flashpk_program_2words(dest_addr, flashpk_start[row] );
		}
	} else {
		for (int row = 0; row < PK_NROWS; row++) {
			uint8_t * pk_row = (uint8_t*) &pkmat[row*len];
			int n = len*4;
			uint32_t dest_addr = row*PK_ROW_BYTES + l*4;

			uint8_t * ptr_dest = (uint8_t*) &flashpk_remain[row];
			int write_flash = 0;
			while( dest_addr&7  ) {
				write_flash = 1;
				ptr_dest[dest_addr&7] = pk_row[0];
				pk_row ++;
				n --;
				dest_addr++;
			}
			if(write_flash) flashpk_program_2words(dest_addr-8,flashpk_remain[row]);
			while( n >= 8 ) {
				for(int k=0;k<8;k++) ptr_dest[k] = pk_row[k];
				flashpk_program_2words(dest_addr,flashpk_remain[row]);
				pk_row += 8;
				n -= 8;
				dest_addr += 8;
			}
			if( n ) {
				for(int k=0;k<n;k++) ptr_dest[k] = pk_row[k];
			}
		}
	}
    }
	flashpk_close();
#endif  // defined(_PK_IN_FLASH_)

#else   // defined(GFN_PK_T_WIDTH)
	uint32_t pkmat[PK_NROWS*nblocks_PKMAT];
	fill_in_matrix( pkmat , &prod32[nblocks_I] , &consts32[nblocks_I] , nblocks_PKMAT );

	apply_P_by_sort( pkmat , nblocks_PKMAT , p_op , PK_NROWS , PK_NR_2POW );
	apply_invL_with_L( pkmat , nblocks_PKMAT , inv_M , PK_NROWS );
	apply_invU_with_U( pkmat , nblocks_PKMAT , inv_M , PK_NROWS );

	// output pk
	for(int i=0;i<PK_NROWS;i++) {
		uint8_t * pk_row = (uint8_t*) &pkmat[i*nblocks_PKMAT];
		uint8_t * pkdest_row = pk + i * PK_ROW_BYTES;
		for(int k=0;k<PK_ROW_BYTES;k++) pkdest_row[k] = pk_row[k];
	}
#endif  // defined(GEN_PK_T_WIDTH)
	return 0;
}


