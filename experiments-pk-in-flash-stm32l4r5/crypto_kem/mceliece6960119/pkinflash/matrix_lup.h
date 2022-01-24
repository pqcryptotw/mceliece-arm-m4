

#ifndef _MATRIX_LIB_LUP_H_
#define _MATRIX_LIB_LUP_H_


#include <stdint.h>


int lup_decompose( uint32_t * mat , uint16_t * p , int n );

// lup decomposition for the f-parameter
int lup_decompose_2( uint32_t * mat , uint32_t * extra_cols , uint16_t * p , uint16_t * idx_cols_last32, int n );

/////////

void apply_invL_with_L( uint32_t mat[] , int width_mat,  const uint32_t L[] , int n );

void apply_invU_with_U( uint32_t mat[] , int width_mat,  const uint32_t U[] , int n );

////////

void LU_to_invL_x_invU( uint32_t *mat, int n);

void LU_to_invL_x_invU_extbuffer( uint32_t *mat, int n, uint32_t *buffer_one_row);

void L_to_invL( uint32_t *mat, int n );

void U_to_invU( uint32_t *mat, int n );

void erase_U( uint32_t *mat , int n );

/////////

void pi_to_weight( uint16_t *p , int n );

void pi_to_weight_extbuffer( uint16_t *p , int n , uint32_t * buffer_nxu32);

void apply_P_by_sort( uint32_t mat[] , int width_mat , const uint16_t p_weight[] , int n , int n_2pow);

void apply_P_by_sort_extbuffer( uint32_t mat[] , int width_mat , const uint16_t p_weight[] , int n , int n_2pow, uint16_t * buffer_p);

/////////


int to_invmat( uint32_t *mat , int n );

int to_invmat_2( uint32_t *mat , uint32_t * extra_cols , uint16_t * idx_cols_last32 , int n );




#endif



