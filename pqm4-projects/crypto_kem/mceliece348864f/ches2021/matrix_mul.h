

#ifndef _MATRIX_LIB_MUL_H_
#define _MATRIX_LIB_MUL_H_


#include <stdint.h>


//
//  mat_c = mat_a * mat_b
//  mat_a: square matrix
//
void matrix_mul( uint32_t *mat_c , const uint32_t* mat_a , int w_a , const uint32_t *mat_b , int w_b );

void matrix_mul_32( uint32_t *mat_c , const uint32_t* mat_a , int w_a, const uint32_t *mat_b , int w_b );

void matrix_mul_64( uint32_t *mat_c , const uint32_t* mat_a , int w_a, const uint32_t *mat_b , int w_b );

void matrix_mul_64_ext_buffer( uint32_t *mat_c , const uint32_t* mat_a , int w_a, const uint32_t *mat_b , int w_b , uint32_t *buffer);

void matrix_mul_128( uint32_t *mat_c , const uint32_t* mat_a , int w_a , const uint32_t *mat_b , int w_b );

void matrix_mul_256( uint32_t *mat_c , const uint32_t* mat_a , int w_a , const uint32_t *mat_b , int w_b );






///
/// @brief matrix multiplication and addition on GF(2). mat_c += mat_a * mat_b
///
void gf2_submat32x32_madd(uint32_t *mat_c, int w_c, const uint32_t *mat_a, int w_a, const uint32_t *mat_b, int w_b);

void gf2_submat64x64_madd(uint32_t *mat_c, int w_c, const uint32_t *mat_a, int w_a, const uint32_t *mat_b, int w_b);

void gf2_submat128x128_madd(uint32_t *mat_c, int w_c, const uint32_t *mat_a, int w_a, const uint32_t *mat_b, int w_b);

void gf2_submat256x256_madd(uint32_t *mat_c, int w_c, const uint32_t *mat_a, int w_a, const uint32_t *mat_b, int w_b);


#define BUFFERSIZE_64X64_U32 (32*14)
void gf2_submat64x64_madd_ext_buffer(uint32_t *mat_c, int w_c, const uint32_t *mat_a, int w_a, const uint32_t *mat_b, int w_b, uint32_t *buffer_448u32);


#define SIZE_INPUT_64X64_U32 (32*7)
void gf2mat_input_64x64_s(uint32_t *s, const uint32_t *mat_a, int w_a);
void gf2mat_input_64x64_t(uint32_t *t, const uint32_t *mat_b, int w_b);
void gf2mat_madd_64x64_with_transformed_inputs(uint32_t *mat_c, int w_c, const uint32_t *inp_a, const uint32_t *inp_b );




#endif



