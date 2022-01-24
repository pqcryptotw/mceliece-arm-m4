/*
  matrix library for public-key generation
*/


#include "matrix_lup.h"




static inline uint32_t equal_mask(uint32_t x, uint32_t y)
{
  uint32_t mask = x ^ y;
  mask -= 1;
  mask >>= 31;
  mask = -mask;
  return mask;
}


static inline uint32_t bit_mask(uint32_t x, int i)
{
  uint32_t mask = (x>>i)&1;
  return -mask;
}







/////////////////////////////////////////////////////




int lup_decompose( uint32_t * mat , uint16_t * p , int n )
{
	int i, j, k;
	uint32_t mask;
	int row, c;
	int mat_width = (n+31)>>5;

	for(i=0;i<n;i++) p[i]=i;

	for (row = 0; row < n; row++) {
		i = row >> 5;
		j = row & 31;
		uint32_t target_bit = (1<<j);
		for (k = row + 1; k < n; k++) {
			mask = ((~mat[ row *mat_width+ i ]) & mat[k*mat_width+i] )&target_bit;
			mask >>= j;
			mask = -mask;

			int16_t r_diff = (p[row]^p[k])&(mask&0xffff);
			p[row] ^= r_diff;
			p[k] ^= r_diff;
			for (c = 0; c < mat_width; c++) {
				uint32_t diff = (mat[ row *mat_width+ c ] ^ mat[ k *mat_width+ c ]) & mask;
				mat[ row *mat_width+ c ] ^= diff;
				mat[ k *mat_width+ c ] ^= diff;
			}
		}

		if ( 0==(mat[ row *mat_width+ i ]&target_bit) ) // return if not invertible
			return -1;

		uint32_t mask_0 = (target_bit+target_bit)-1;
		mask_0 = ~mask_0;
//printf("j=%d. target_bit=%x, mask_0=%x\n", j , target_bit , mask_0 );
		for (k = row+1; k < n; k++) {
			mask = mat[ k *mat_width+ i ]&target_bit;
			mask >>= j;
			mask = -mask;
			mat[k*mat_width+i] ^= mat[ row *mat_width+ i ] & mask_0 & mask;  // if(31==j) -> mask_0=0, no effect
			for (c = i+1; c < mat_width; c++) {
				mat[ k *mat_width+ c ] ^= mat[ row *mat_width+ c ] & mask;
			}
		}
	}
	return 0;
}


static
int get_invitable_columns_32x64( uint64_t mat[] , uint16_t * idx_cols )
{
	for (int i = 0; i < 32; i++)
	{
		uint64_t t = mat[i];
		for (int j = i+1; j < 32; j++) { t |= mat[j]; }

		if (t == 0) return -1; // return if buf is not full rank

		idx_cols[i] = __builtin_ctzll(t);

		for (int j = i+1; j < 32; j++) { uint64_t mask = (mat[i] >> idx_cols[i]) & 1; mask -= 1;    mat[i] ^= mat[j] & mask; }
		for (int j = i+1; j < 32; j++) { uint64_t mask = (mat[j] >> idx_cols[i]) & 1; mask = -mask; mat[j] ^= mat[i] & mask; }
	}
	return 0;
}


int lup_decompose_2( uint32_t * mat , uint32_t * extra_mat , uint16_t * p , uint16_t * idx_cols, int n )
{
	int i, j, k;
	uint32_t mask;
	int row, c;
	int mat_width = (n+31)>>5;

	for(i=0;i<n;i++) p[i]=i;

	for (row = 0; row < n-32; row++) {
		i = row >> 5;
		j = row & 31;
		uint32_t target_bit = (1<<j);
		for (k = row + 1; k < n; k++) {
			mask = ((~mat[ row *mat_width+ i ]) & mat[k*mat_width+i] )&target_bit;
			mask >>= j;
			mask = -mask;

			int16_t r_diff = (p[row]^p[k])&(mask&0xffff);
			p[row] ^= r_diff;
			p[k] ^= r_diff;
			for (c = 0; c < mat_width; c++) {
				uint32_t diff = (mat[ row *mat_width+ c ] ^ mat[ k *mat_width+ c ]) & mask;
				mat[ row *mat_width+ c ] ^= diff;
				mat[ k *mat_width+ c ] ^= diff;
			}
			{
				uint32_t diff = (extra_mat[row]^extra_mat[k])&mask;
				extra_mat[row] ^= diff;
				extra_mat[k] ^= diff;
			}
		}

		if ( 0==(mat[ row *mat_width+ i ]&target_bit) ) // return if not invertible
			return -1;

		uint32_t mask_0 = (target_bit+target_bit)-1;
		mask_0 = ~mask_0;
//printf("j=%d. target_bit=%x, mask_0=%x\n", j , target_bit , mask_0 );
		for (k = row+1; k < n; k++) {
			mask = mat[ k *mat_width+ i ]&target_bit;
			mask >>= j;
			mask = -mask;
			mat[k*mat_width+i] ^= mat[ row *mat_width+ i ] & mask_0 & mask;  // if(31==j) -> mask_0=0, no effect
			for (c = i+1; c < mat_width; c++) {
				mat[ k *mat_width+ c ] ^= mat[ row *mat_width+ c ] & mask;
			}
			extra_mat[k] ^= extra_mat[row]&mask;
		}
	}

	// get invertible columns
	uint64_t mat2[32];
	for(i=0;i<32;i++) { mat2[i] = ((uint64_t)mat[(n-32+i+1)*mat_width-1])|(((uint64_t)extra_mat[n-32+i])<<32); }
	for(i=0;i<32;i++) idx_cols[i] = i;
	int r = get_invitable_columns_32x64( mat2 , idx_cols );
	if( r ) return r;

	// collect columns
	for(row = 0; row < n; row ++ ) {
		uint64_t t = ((uint64_t)mat[(row+1)*mat_width-1])|(((uint64_t)extra_mat[row])<<32);
		uint32_t d = 0;
		for(int i=0;i<32;i++) {
			uint32_t t1 = (t>>idx_cols[i])&1;
			d |= t1<<i;
		}
		mat[(row+1)*mat_width-1] = d;
	}


	for (row = n-32; row < n-1; row++) {
		i = row >> 5;
		j = row & 31;
		uint32_t target_bit = (1<<j);
		for (k = row + 1; k < n; k++) {
			mask = ((~mat[ row *mat_width+ i ]) & mat[k*mat_width+i] )&target_bit;
			mask >>= j;
			mask = -mask;

			int16_t r_diff = (p[row]^p[k])&(mask&0xffff);
			p[row] ^= r_diff;
			p[k] ^= r_diff;
			for (c = 0; c < mat_width; c++) {
				uint32_t diff = (mat[ row *mat_width+ c ] ^ mat[ k *mat_width+ c ]) & mask;
				mat[ row *mat_width+ c ] ^= diff;
				mat[ k *mat_width+ c ] ^= diff;
			}
		}

		//if ( 0==(mat[ row *mat_width+ i ]&target_bit) ) // must be invertible

		uint32_t mask_0 = (target_bit+target_bit)-1;
		mask_0 = ~mask_0;
//printf("j=%d. target_bit=%x, mask_0=%x\n", j , target_bit , mask_0 );
		for (k = row+1; k < n; k++) {
			mask = mat[ k *mat_width+ i ]&target_bit;
			mask >>= j;
			mask = -mask;
			mat[k*mat_width+i] ^= mat[ row *mat_width+ i ] & mask_0 & mask;  // if(31==j) -> mask_0=0, no effect
			for (c = i+1; c < mat_width; c++) {
				mat[ k *mat_width+ c ] ^= mat[ row *mat_width+ c ] & mask;
			}
		}
	}
	return 0;
}




////////////////////////////////////////////////////


void L_to_invL( uint32_t *mat, int n )
{
	int row, c, i, j, k;
	uint32_t mask;
	int mat_width = (n+31)>>5;

	for (row = n-2; row > 0; row--) {
		i = row >> 5;
		j = row & 31;
		uint32_t target_bit = (1<<j);
		uint32_t mask_tail = (1<<j)-1;
		for (k = row + 1; k < n; k++) {
			mask = (mat[k*mat_width+i] & target_bit)>>j;
			mask = -mask;
			for (c = 0; c < i; c++) { mat[ k *mat_width+ c ] ^= mat[ row*mat_width+c]&mask; }
			mat[k*mat_width+i] ^= mat[row*mat_width+i]&mask&mask_tail;
		}
	}
}




void erase_U( uint32_t *mat , int n )
{
	int mat_width = (n+31)>>5;
	for(int i=0;i<n-1;i++) {
		int idx = i+1;
		int st = idx>>5;
		int j = idx&31;
		uint32_t head_mask = (1<<j)-1;
		mat[i*mat_width+st] &= head_mask;
		for(int k=st+1;k<mat_width;k++) mat[i*mat_width+k] = 0;
	}
}


void U_to_invU( uint32_t *mat, int n )
{
	int row, c, i, j, k;
	uint32_t mask;
	int mat_width = (n+31)>>5;

	for (row = 1; row < n-1; row++) {
		i = row >> 5;
		j = row & 31;
		uint32_t mask_head = (~((1<<j)-1))<<1;
		for (k = 0; k < row; k++) {
			mask = bit_mask( mat[k*mat_width+i] , j );
			mat[k*mat_width+i] ^= mat[row*mat_width+i]&mask&mask_head;
			for (c = i+1; c < mat_width; c++) { mat[ k *mat_width+ c ] ^= mat[ row*mat_width+c]&mask; }
		}
	}
}



static inline
void __matrix_madd_32x32(const uint32_t *L, int width_L , uint32_t *mat, int width_mat, int row, int k, int c)
{
	int i, x, y;

	i = k >> 5;
	for (x = 0; x < 32; x += 4)
	{
		uint32_t a0 = L[ (row + x + 0 )*width_L + i ];
		uint32_t a1 = L[ (row + x + 1 )*width_L + i ];
		uint32_t a2 = L[ (row + x + 2 )*width_L + i ];
		uint32_t a3 = L[ (row + x + 3 )*width_L + i ];
		uint32_t c0 = mat[ (row + x + 0 )*width_mat + c ];
		uint32_t c1 = mat[ (row + x + 1 )*width_mat + c ];
		uint32_t c2 = mat[ (row + x + 2 )*width_mat + c ];
		uint32_t c3 = mat[ (row + x + 3 )*width_mat + c ];
		for(y=0;y<32;y++) {
			c0 ^= mat[ (k + y )*width_mat + c ] * ((a0 >> y) & 1);
			c1 ^= mat[ (k + y )*width_mat + c ] * ((a1 >> y) & 1);
			c2 ^= mat[ (k + y )*width_mat + c ] * ((a2 >> y) & 1);
			c3 ^= mat[ (k + y )*width_mat + c ] * ((a3 >> y) & 1);
		}

		mat[ (row + x + 0 )*width_mat + c ]=c0;
		mat[ (row + x + 1 )*width_mat + c ]=c1;
		mat[ (row + x + 2 )*width_mat + c ]=c2;
		mat[ (row + x + 3 )*width_mat + c ]=c3;
	}
}


static
void apply_invL_768x640(const uint32_t L[], uint32_t mat[])
{
	int i, k, c, row=0, x=0, y=0;
	uint32_t mask;

	for (row = 0; row < 768; row += 32) {
		for (k = 0; k < row; k += 32) {
			for (c = 0; c < 20; c++)
				__matrix_madd_32x32((const uint32_t*)L, 24, (uint32_t*)mat, 20, row, k, c);
		}

		i = k>>5;
		for (x = 0; x < 32; x++) {
			for (y = 0; y <  x; y++) {
				mask = bit_mask( L[ (row + x )*24+ i ] , y );
				for (c = 0; c < 20; c++) {
	 				mat[ (row + x )*20+ c ] ^= mat[ (k + y )*20+ c ] & mask;
			 	}
			}
		}
	}
}


static
void apply_invU_768x640(const uint32_t U[], uint32_t mat[])
{
	int i, k, c, row=0, x=0, y=0;
	uint32_t mask;

	for (row = 768-32; row >= 0; row -= 32)
	{

		for (k = row+32; k < 768; k += 32)
			for (c = 0; c < 20; c++)
				__matrix_madd_32x32((const uint32_t*)U, 24, (uint32_t*)mat, 20, row, k, c);

		for (x = 31; x >=0; x--) {
			int row_x = row+x;
			i = row >> 5;
			for (y = x+1; y <  32; y++) {
				mask = bit_mask( U[ (row_x )*24+ i ] , y );
				for (c = 0; c < 20; c++) // should be unrolled
				{
	 				mat[ (row_x )*20+ c ] ^= mat[ (row+y )*20+ c ] & mask;
			 	}
			}
		}
	}
}


void apply_invU_with_U( uint32_t mat[] , int width_mat,  const uint32_t U[] , int n )
{
	if( 768==n && 20==width_mat ) {
		return apply_invU_768x640(U, mat);
	}

	int i, k;
	uint32_t mask;
	int row, c;

	int ops_width = (n+31)>>5;
#if 1
	int x=0, y=0;
	row=0;

	for (row = n-32; row >= 0; row -= 32) {

		for (k = row+32; k < n; k += 32)
			for (c = 0; c < width_mat; c++)
				__matrix_madd_32x32((uint32_t*)U, ops_width, (uint32_t*)mat, width_mat, row, k, c);

		for (x = 31; x >=0; x--) {
			int row_x = row+x;
			i = row>>5;
			for (y = x+1; y <  32; y++) {
				mask = bit_mask( U[ (row_x )*ops_width+ i ] , y );
				for (c = 0; c < width_mat; c++) {
	 				mat[ (row_x )*width_mat+ c ] ^= mat[ (row+y )*width_mat+ c ] & mask;
			 	}
			}
		}


	}

#elif 0
	for (row = n-2; row >= 0; row--) {
		for (c = 0; c < (width_mat&(~7)); c+=8) {
			uint32_t c0 = mat[ row *width_mat+ c ];
			uint32_t c1 = mat[ row *width_mat+ c+1 ];
			uint32_t c2 = mat[ row *width_mat+ c+2 ];
			uint32_t c3 = mat[ row *width_mat+ c+3 ];
			uint32_t c4 = mat[ row *width_mat+ c+4 ];
			uint32_t c5 = mat[ row *width_mat+ c+5 ];
			uint32_t c6 = mat[ row *width_mat+ c+6 ];
			uint32_t c7 = mat[ row *width_mat+ c+7 ];
			for (k = row+1; k < n; k++) {
				mask = bit_mask( U[ row *ops_width + (k>>5) ] , k&31 );
				c0 ^= mat[ k *width_mat+ c ] & mask;
				c1 ^= mat[ k *width_mat+ c+1 ] & mask;
				c2 ^= mat[ k *width_mat+ c+2 ] & mask;
				c3 ^= mat[ k *width_mat+ c+3 ] & mask;
				c4 ^= mat[ k *width_mat+ c+4 ] & mask;
				c5 ^= mat[ k *width_mat+ c+5 ] & mask;
				c6 ^= mat[ k *width_mat+ c+6 ] & mask;
				c7 ^= mat[ k *width_mat+ c+7 ] & mask;
			}
			mat[ row *width_mat+ c ] = c0;
			mat[ row *width_mat+ c+1 ] = c1;
			mat[ row *width_mat+ c+2 ] = c2;
			mat[ row *width_mat+ c+3 ] = c3;
			mat[ row *width_mat+ c+4 ] = c4;
			mat[ row *width_mat+ c+5 ] = c5;
			mat[ row *width_mat+ c+6 ] = c6;
			mat[ row *width_mat+ c+7 ] = c7;
		}
		for (c = (width_mat&(~7)); c < (width_mat&(~3)); c+=4) {
			uint32_t c0 = mat[ row *width_mat+ c ];
			uint32_t c1 = mat[ row *width_mat+ c+1 ];
			uint32_t c2 = mat[ row *width_mat+ c+2 ];
			uint32_t c3 = mat[ row *width_mat+ c+3 ];
			for (k = row+1; k < n; k++) {
				mask = bit_mask( U[ row *ops_width + (k>>5) ] , k&31 );
				c0 ^= mat[ k *width_mat+ c ] & mask;
				c1 ^= mat[ k *width_mat+ c+1 ] & mask;
				c2 ^= mat[ k *width_mat+ c+2 ] & mask;
				c3 ^= mat[ k *width_mat+ c+3 ] & mask;
			}
			mat[ row *width_mat+ c ] = c0;
			mat[ row *width_mat+ c+1 ] = c1;
			mat[ row *width_mat+ c+2 ] = c2;
			mat[ row *width_mat+ c+3 ] = c3;

		}
		for (c = width_mat&(~3); c < width_mat; c++) {
			uint32_t c0 = mat[ row *width_mat+ c ];
			for (k = row+1; k < n; k++) {
				mask = bit_mask( U[ row *ops_width + (k>>5) ] , k&31 );
				c0 ^= mat[ k *width_mat+ c ] & mask;
			}
			mat[ row *width_mat+ c ] = c0;
		}

	}
#elif 0
// 67773929
	for (row = n-2; row >= 0; row--) {
		for (k = row+1; k < n; k++) {
			mask = bit_mask( U[ row *ops_width + (k>>5) ] , k&31 );
			for (c = 0; c < width_mat; c++) { mat[ row *width_mat+ c ] ^= mat[ k *width_mat+ c ] & mask; }
		}
	}
#else
// 69257317
	for (row = n-1; row >= 0; row--) {
		i = row >> 5;
		j = row & 31;

		for (k = 0; k < row; k++) {
			mask = bit_mask( U[ k *ops_width + i ] , j ); // >> (row&31);
			for (c = 0; c < width_mat; c++) { mat[ k *width_mat+ c ] ^= mat[ row *width_mat+ c ] & mask; }
		}
	}
#endif
}


/////////////////////////////////////////////////

static inline
void remove_row_U( uint32_t * row , uint32_t * U , int width , int i )
{
	int idx = i+1; // start from i+1 column. reserve '1' at i-th column.
	int st = idx>>5;
	int j = idx&31;
	uint32_t head_mask = (1<<j)-1;
	row[st] = U[i*width+st];
	U[i*width+st] &= head_mask;
	for(int k=st+1;k<width;k++) {
		row[k] = U[i*width+k];
		U[i*width+k] = 0;
	}
}


void LU_to_invL_x_invU( uint32_t *mat, int n)
{
	L_to_invL( mat , n );

	int width = (n+31)>>5;
	uint32_t reserved_row_U[width];

	for (int row = n-2; row >= 0; row--) {
		remove_row_U( reserved_row_U , mat , width , row );
		for (int k = row+1; k < n; k++) {
			uint32_t mask = bit_mask( reserved_row_U[ (k>>5) ] , k&31 );
			for (int c = 0; c < width; c++) { mat[ row *width + c ] ^= mat[ k *width + c ] & mask; }
		}
	}
}

void LU_to_invL_x_invU_extbuffer( uint32_t *mat, int n, uint32_t *reserved_row_U)
{
	L_to_invL( mat , n );
	int width = (n+31)>>5;

	for (int row = n-2; row >= 0; row--) {
		remove_row_U( reserved_row_U , mat , width , row );
		for (int k = row+1; k < n; k++) {
			uint32_t mask = bit_mask( reserved_row_U[ (k>>5) ] , k&31 );
			for (int c = 0; c < width; c++) { mat[ row *width + c ] ^= mat[ k *width + c ] & mask; }
		}
	}
}


/////////////////////////////////////////////////


void apply_invL_with_L( uint32_t mat[] , int width_mat,  const uint32_t L[] , int n )
{
	if( 768==n && 20==width_mat ) {
		return apply_invL_768x640( L, mat);
	}

	int i, k;
	uint32_t mask;
	int row, c;

	//int ops_width = (n+31)>>5;
#if 1
	int x=0, y=0;
	row=0;
	int width_L = (n+31)>>5;

	for (row = 0; row < n; row += 32)
	{
		for (k = 0; k < row; k += 32) {
			for (c = 0; c < width_mat; c++)
				__matrix_madd_32x32( L, width_L, mat, width_mat, row, k, c);
		}
		i = k>>5;
		for (x = 0; x < 32; x++) {
			for (y = 0; y <  x; y++) {
				//i = (y + k) >> 5;
				//j = (y + k) & 31;
				mask = bit_mask( L[ (row + x )*width_L + i ] , y );
				for (c = 0; c < width_mat; c++) {
	 				mat[ (row + x )*width_mat+ c ] ^= mat[ (k + y  )*width_mat+ c ] & mask; 
			 	}
			}
		}
	}

#elif 0
	for (row = 1; row < n; row++) {
		for (c = 0; c < (width_mat&(~7)); c+=8) {
			uint32_t c0 = mat[ row *width_mat+ c ];
			uint32_t c1 = mat[ row *width_mat+ c+1 ];
			uint32_t c2 = mat[ row *width_mat+ c+2 ];
			uint32_t c3 = mat[ row *width_mat+ c+3 ];
			uint32_t c4 = mat[ row *width_mat+ c+4 ];
			uint32_t c5 = mat[ row *width_mat+ c+5 ];
			uint32_t c6 = mat[ row *width_mat+ c+6 ];
			uint32_t c7 = mat[ row *width_mat+ c+7 ];
			for (k = 0; k < row; k++) {
				mask = bit_mask( L[ row *ops_width + (k>>5) ] , k&31 );
				c0 ^= mat[ k *width_mat+ c ] & mask;
				c1 ^= mat[ k *width_mat+ c+1 ] & mask;
				c2 ^= mat[ k *width_mat+ c+2 ] & mask;
				c3 ^= mat[ k *width_mat+ c+3 ] & mask;
				c4 ^= mat[ k *width_mat+ c+4 ] & mask;
				c5 ^= mat[ k *width_mat+ c+5 ] & mask;
				c6 ^= mat[ k *width_mat+ c+6 ] & mask;
				c7 ^= mat[ k *width_mat+ c+7 ] & mask;
			}
			mat[ row *width_mat+ c ] = c0;
			mat[ row *width_mat+ c+1 ] = c1;
			mat[ row *width_mat+ c+2 ] = c2;
			mat[ row *width_mat+ c+3 ] = c3;
			mat[ row *width_mat+ c+4 ] = c4;
			mat[ row *width_mat+ c+5 ] = c5;
			mat[ row *width_mat+ c+6 ] = c6;
			mat[ row *width_mat+ c+7 ] = c7;
		}
		for (c = (width_mat&(~7)); c < (width_mat&(~3)); c+=4) {
			uint32_t c0 = mat[ row *width_mat+ c ];
			uint32_t c1 = mat[ row *width_mat+ c+1 ];
			uint32_t c2 = mat[ row *width_mat+ c+2 ];
			uint32_t c3 = mat[ row *width_mat+ c+3 ];
			for (k = 0; k < row; k++) {
				mask = bit_mask( L[ row *ops_width + (k>>5) ] , k&31 );
				c0 ^= mat[ k *width_mat+ c ] & mask;
				c1 ^= mat[ k *width_mat+ c+1 ] & mask;
				c2 ^= mat[ k *width_mat+ c+2 ] & mask;
				c3 ^= mat[ k *width_mat+ c+3 ] & mask;
			}
			mat[ row *width_mat+ c ] = c0;
			mat[ row *width_mat+ c+1 ] = c1;
			mat[ row *width_mat+ c+2 ] = c2;
			mat[ row *width_mat+ c+3 ] = c3;
		}
		for (c = (width_mat&(~3)); c < width_mat ;c++) {
			uint32_t c0 = mat[ row *width_mat+ c ];
			for (k = 0; k < row; k++) {
				mask = bit_mask( L[ row *ops_width + (k>>5) ] , k&31 );
				c0 ^= mat[ k *width_mat+ c ] & mask;
			}
			mat[ row *width_mat+ c ] = c0;
		}
	}
#elif 0
// 67770861
	for (row = 1; row < n; row++) {
		for (k = 0; k < row; k++) {
			mask = bit_mask( L[ row *ops_width + (k>>5) ] , k&31 );
			for (c = 0; c < width_mat; c++) {
				mat[ row *width_mat+ c ] ^= mat[ k *width_mat+ c ] & mask;
			}
		}
	}
#else
// 56534760
	for (row = 0; row < n; row++) {
		i = row >> 5;
		j = row & 31;

		for (k = row+1; k < n; k++) {
			mask = bit_mask( L[ k *ops_width + i ] , j );
			for (c = 0; c < (width_mat&(~3)); c+=4) { mat[ k *width_mat+ c ] ^= mat[ row *width_mat+ c ] & mask;
				mat[ k *width_mat+ c +1] ^= mat[ row *width_mat+ c +1] & mask;
				mat[ k *width_mat+ c +2] ^= mat[ row *width_mat+ c +2] & mask;
				mat[ k *width_mat+ c +3] ^= mat[ row *width_mat+ c +3] & mask;  }
			for (c = width_mat&(~3); c < width_mat; c++) { mat[ k *width_mat+ c ] ^= mat[ row *width_mat+ c ] & mask; }
		}
	}
#endif
}




////////////////////////////////////////////////////

#include "sort_int32.h"
#include "sort_uint32.h"


void pi_to_weight( uint16_t *p , int n )
{
    int32_t p32[n];
    for(int i=0;i<n;i++) p32[i] = (((int32_t)p[i])<<16)|i;
    int32_sort( p32 , n );
    for(int i=0;i<n;i++) p[i] = p32[i]&0xffff;
}

void pi_to_weight_extbuffer( uint16_t *p , int n , uint32_t * p32)
{
    //int32_t p32[n];
    for(int i=0;i<n;i++) p32[i] = (((int32_t)p[i])<<16)|i; // XXX: possible bug if p[i] is too large
    int32_sort( (int32_t*)p32 , n );
    for(int i=0;i<n;i++) p[i] = p32[i]&0xffff;
}


void apply_P_by_sort( uint32_t mat[] , int width_mat , const uint16_t p_weight[] , int n , int n_2pow )
{
  uint16_t p[n];
  for(int i=0;i<n;i++) p[i]=p_weight[i];
  sort_with_payload_2(n_2pow, n, p, mat, width_mat, 0 );
}

void apply_P_by_sort_extbuffer( uint32_t mat[] , int width_mat , const uint16_t p_weight[] , int n , int n_2pow, uint16_t * buffer_p)
{
  for(int i=0;i<n;i++) buffer_p[i]=p_weight[i];
  sort_with_payload_2(n_2pow, n, buffer_p, mat, width_mat, 0 );
}


/////////////////////////////////////////////////////


#if 0

int to_invmat( uint32_t *mat , int n )
{
	int16_t row_swap[n];
	int r = lup_decompose( mat , row_swap , n );
	if( r ) return r;  // early abort

	L_to_invL( mat , n );
	uint16_t *idx = (uint16_t*) row_swap;
	int mat_width = (n+31)>>5;

	uint32_t invL_U_mat[n*mat_width];
	for(int i=0;i<n*mat_width;i++) invL_U_mat[i] = mat[i];

	for(int i=0;i<n;i++) {
		uint32_t mask = ((uint32_t)1)<<(idx[i]&31);
		int c = idx[i]>>5;
		for(int j=0;j<mat_width;j++) { mat[i*mat_width+j] = mask&equal_mask(c,j); }
	}

	apply_invU_invL( mat , mat_width , invL_U_mat , n );
	return r;
}







int to_invmat_2( uint32_t *mat , uint32_t * extra_cols , uint16_t * idx_cols_last32 , int n )
{
	int16_t row_swap[n];
	int r = lup_decompose_2( mat , extra_cols , row_swap , idx_cols_last32 , n );
	if( r ) return r;  // early abort

	L_to_invL( mat , n );
	uint16_t *idx = (uint16_t*) row_swap;
	int mat_width = (n+31)>>5;

	uint32_t invL_U_mat[n*mat_width];
	for(int i=0;i<n*mat_width;i++) invL_U_mat[i] = mat[i];

	for(int i=0;i<n;i++) {
		uint32_t mask = ((uint32_t)1)<<(idx[i]&31);
		int c = idx[i]>>5;
		for(int j=0;j<mat_width;j++) { mat[i*mat_width+j] = mask&equal_mask(c,j); }
	}

	apply_invU_invL( mat , mat_width , invL_U_mat , n );
	return r;
}

#endif



