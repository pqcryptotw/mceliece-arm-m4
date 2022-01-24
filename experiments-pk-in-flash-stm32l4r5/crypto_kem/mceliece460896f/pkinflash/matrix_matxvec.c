/// @file matrix_matxvec.c
/// @brief The standard implementations for matrix multiplication and addition on GF(2)
///

#include "matrix_matxvec.h"

#include "stdint.h"

///
/// @brief matrix multiplication and addition on GF(2). c += mat * a
///
/// @param[out] c        - the output vector c.
/// @param[in]  mat      - the input matrix mat, row major.
/// @param[in]  w_mat    - width of the mat, number of byte.
/// @param[in]  h_mat    - height of the mat.
/// @param[in]  w_mat    - the input vecotr a.
///

//static void gf2_mat_madd_x32(unsigned char *c, const unsigned char *mat, unsigned w_mat, unsigned h_mat, const unsigned char *a);
static void gf2_mat_madd_x4(unsigned char *c, const unsigned char *mat, unsigned w_mat, unsigned h_mat, const unsigned char *a);
static void gf2_mat_madd_x2(unsigned char *c, const unsigned char *mat, unsigned w_mat, unsigned h_mat, const unsigned char *a);


#include "run_config.h"

#ifdef _M4_ASM_
void matxvec_madd_768x2720_asm(unsigned char *c, const unsigned char *mat, const unsigned char *a);
void matxvec_madd_1248x3360_asm(unsigned char *c, const unsigned char *mat, const unsigned char *a);
#endif

void gf2_mat_madd(unsigned char *c, const unsigned char *mat, unsigned w_mat, unsigned h_mat, const unsigned char *a)
{
#ifdef _M4_ASM_
	if( (768==h_mat) && (340==w_mat) ) return matxvec_madd_768x2720_asm(c,mat,a);
	if( (1248==h_mat) && (420==w_mat) ) return matxvec_madd_1248x3360_asm(c,mat,a);
#endif

	if( 0 == (h_mat&3) ) return gf2_mat_madd_x4(c,mat,w_mat,h_mat,a);
	if( 0 == (h_mat&1) ) return gf2_mat_madd_x2(c,mat,w_mat,h_mat,a);

	uint32_t b;
	const uint32_t * ptr_a = (const uint32_t *)a;
	uint32_t * ptr_row;
	unsigned row_byte = w_mat;
	unsigned row_n_u32 = row_byte>>2;

	for (unsigned i = 0; i < h_mat; i++)
	{
		ptr_row = (uint32_t *) (mat + row_byte * i);

		b = 0;
		for (unsigned j = 0; j < row_n_u32; j++)
			b ^= (ptr_row[j] & ptr_a[j]);

		b ^= b >> 16;
		b ^= b >> 8;
		b ^= b >> 4;
		b ^= b >> 2;
		b ^= b >> 1;
		b &= 1;

		c[ i/8 ] ^= (b << (i&7));
	}
}



/// 802119  for  h=768  w=2720
static
void gf2_mat_madd_x2(unsigned char *c, const unsigned char *mat, unsigned w_mat, unsigned h_mat, const unsigned char *a)
{
	uint32_t b0;
	uint32_t b1;
	const uint32_t * ptr_a = (const uint32_t *)a;
	uint32_t * ptr_row0;
	uint32_t * ptr_row1;
	unsigned row_byte = w_mat;
	unsigned row_n_u32 = row_byte>>2;

	for (unsigned i = 0; i < h_mat; i+=2)
	{
		ptr_row0 = (uint32_t *) (mat + row_byte * i);
		ptr_row1 = (uint32_t *) (mat + row_byte * (i+1));

		b0 = 0;
		b1 = 0;
		for (unsigned j = 0; j < row_n_u32; j++) {
			b0 ^= (ptr_row0[j] & ptr_a[j]);
			b1 ^= (ptr_row1[j] & ptr_a[j]);
		}

		b0 ^= b0 >> 16;
		b1 ^= b1 >> 16;
		b0 ^= b0 >> 8;
		b1 ^= b1 >> 8;
		b0 ^= b0 >> 4;
		b1 ^= b1 >> 4;
		b0 ^= b0 >> 2;
		b1 ^= b1 >> 2;
		b0 ^= b0 >> 1;
		b1 ^= b1 >> 1;
		b0 &= 1;
		b1 &= 1;
		b0 ^= b1<<1;
		c[ i/8 ] ^= (b0 << (i&7));
	}
}


static inline
uint32_t parallel_reduce_x4(uint32_t b0, uint32_t b1, uint32_t b2, uint32_t b3 )
{
		b0 ^= b0 >> 2;
		b1 ^= b1 >> 2;
		b2 ^= b2 >> 2;
		b3 ^= b3 >> 2;
		b0 ^= b0 >> 1;
		b1 ^= b1 >> 1;
		b2 ^= b2 >> 1;
		b3 ^= b3 >> 1;

		b0 &= 0x11111111;
		b1 &= 0x11111111;
		b2 &= 0x11111111;
		b3 &= 0x11111111;
		b0 ^= (b1<<1)^(b2<<2)^(b3<<3);
		b0 ^= b0>>4;
		b0 ^= b0>>8;
		b0 ^= b0>>16;
		return b0&0xf;
}


/// 607332  for  h=768  w=2720
static
void gf2_mat_madd_x4(unsigned char *c, const unsigned char *mat, unsigned w_mat, unsigned h_mat, const unsigned char *a)
{
	uint32_t b0;
	uint32_t b1;
	uint32_t b2;
	uint32_t b3;
	const uint32_t * ptr_a = (const uint32_t *)a;
	uint32_t * ptr_row0;
	uint32_t * ptr_row1;
	uint32_t * ptr_row2;
	uint32_t * ptr_row3;
	unsigned row_byte = w_mat;
	unsigned row_n_u32 = row_byte>>2;

	for (unsigned i = 0; i < h_mat; i+=4)
	{
		ptr_row0 = (uint32_t *) (mat + row_byte * i);
		ptr_row1 = (uint32_t *) (mat + row_byte * (i+1));
		ptr_row2 = (uint32_t *) (mat + row_byte * (i+2));
		ptr_row3 = (uint32_t *) (mat + row_byte * (i+3));

		b0 = 0;
		b1 = 0;
		b2 = 0;
		b3 = 0;
		for (unsigned j = 0; j < row_n_u32; j++) {
			b0 ^= (ptr_row0[j] & ptr_a[j]);
			b1 ^= (ptr_row1[j] & ptr_a[j]);
			b2 ^= (ptr_row2[j] & ptr_a[j]);
			b3 ^= (ptr_row3[j] & ptr_a[j]);
		}

		b0 = parallel_reduce_x4(b0,b1,b2,b3);

		c[ i/8 ] ^= (b0 << (i&7));
	}
}





