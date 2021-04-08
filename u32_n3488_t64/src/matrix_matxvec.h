/// @file matrix_matxvec.h
/// @brief The standard implementations for matrix operations.
///


#ifndef _GF2_MAT_LIB_H_
#define _GF2_MAT_LIB_H_

#include "stdint.h"



///
/// @brief matrix multiplication and addition on GF(2). c += mat * a
///
/// @param[out] c        - the output vector c.
/// @param[in]  mat      - the input matrix mat, row major.
/// @param[in]  w_mat    - width of the mat, number of byte.
/// @param[in]  h_mat    - height of the mat.
/// @param[in]  w_mat    - the input vecotr a.
/// @param[in]  a        - the input column vector a.
///
void gf2_mat_madd(unsigned char *c, const unsigned char *mat, unsigned w_mat, unsigned h_mat, const unsigned char *a);



#endif
