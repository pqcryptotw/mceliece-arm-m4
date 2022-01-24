/*
  This file is for the inversion-free Berlekamp-Massey algorithm
  see https://ieeexplore.ieee.org/document/87857
*/

#include "bm_gfu32.h"
#include "gfu32.h"



static inline uint32_t mask_gfu32_nonzero( uint32_t e )
{
	uint32_t is_zero = ((e-1)>>31);
	return (is_zero-1);
}

static inline uint32_t mask_leq(uint32_t a, uint32_t b)
{
	uint32_t a_tmp = a;
	uint32_t b_tmp = b;
	uint32_t ret = b_tmp - a_tmp;

	ret >>= 31;
	ret -= 1;

	return ret;
}






#define min(a, b) ((a < b) ? a : b)


// Slower. Only for comparision
//#define _INV_FREE_


#if !defined(_INV_FREE_)


/* input: seq_256, sequence of field elements of length 256 */
/* output: poly_128, minimal polynomial of in of degree < 128 */
void bm_gfu32( uint16_t poly[], uint16_t seq256[] , unsigned seq_len )
{
   #define MAXSTO  (128)
   gfu32 _B[(MAXSTO+2)*2];
   gfu32 C[MAXSTO] = {0};
   gfu32 coefs[MAXSTO*2];
   gfu32 D[MAXSTO];

   uint32_t N = seq_len;
   int max_deg = seq_len/2;
   uint32_t L = 0;

   // initialize
   for(int i=0;i<(int)((seq_len+3)/4);i++) {
      gf_to_gfu32_x4( &coefs[i*4] , &seq256[4*i] );
   }

   gfu32 b, d, f, tmp;
   uint32_t update_mask;
   b = 1;  // last  discrepancy, d

   gfu32 *B = &_B[-(max_deg-1)];
   B[max_deg-1] = 1;
   B[max_deg] = 0;
   //C[max_deg] = 1; // this term is out of range
   gfu32 c0 = 1;  // c0 is alwasy 1

   for(int i=0;i<(int)N;i++) {
      int len = min(i,max_deg) + 1;

      d = gfu32_rev_dot( &C[max_deg-1] , &coefs[i-1] , len-1 );
      d ^= coefs[i]; // contribution of c0

      tmp = gfu32_inv( b );
      f = gfu32_mul( d , tmp );

      uint32_t mnz = mask_gfu32_nonzero(d);
      update_mask = mnz & mask_leq(L*2, i);

      uint32_t L_update = (i+1-L);
      L ^= update_mask & (L^L_update);
      b ^= update_mask & (d^b);

      B[max_deg] ^= update_mask & (B[max_deg]^c0);
      int l = (len == max_deg+1)? max_deg:len;
      int st = max_deg-l;
      gfu32_smul( D , &B[st] , f , l );
      for(int j=0;j<l;j++) B[st+j] ^= update_mask & (B[st+j]^C[st+j]);
      for(int j=0;j<l;j++) C[st+j] ^= mnz & D[j];

      B++;  // B[] = B[] * x
      B[max_deg] = 0;
   }

/*
#if 119 == SYS_T
   C[119] = 1;
#endif
for(int i=0;i<128;i++) {
  printf("%X,",C[i]);
}
printf("\n");
*/



   for(int j=0;j<((max_deg+3)/4);j++) {
      gfu32_to_gf_x4( &poly[j*4] ,  &C[j*4] );
   }

#if 119 == SYS_T
   poly[119] = 1;
   for(int i=120;i<128;i++) poly[i]=0;
#endif

}


#else

// Slower. Only for comparision

/* input: seq_256, sequence of field elements of length 256 */
/* output: poly_128, minimal polynomial of in of degree < 128 */
void bm_gfu32( uint16_t poly[], uint16_t seq256[] , unsigned seq_len )
{
   #define MAXSTO  (128)
   gfu32 _B[(MAXSTO+2)*2];
   gfu32 C[MAXSTO] = {0};
   gfu32 coefs[MAXSTO*2];
   gfu32 B_temp[MAXSTO];
   gfu32 C_temp[MAXSTO];

   uint32_t N = seq_len;
   int max_deg = seq_len/2;
   uint32_t L = 0;

   // initialize
   for(int i=0;i<(int)((seq_len+3)/4);i++) {
      gf_to_gfu32_x4( &coefs[i*4] , &seq256[4*i] );
   }

   gfu32 b, d, f, tmp;
   uint32_t update_mask;
   b = 1;  // last  discrepancy, d

   gfu32 *B = &_B[-(max_deg-1)];
   B[max_deg-1] = 1;
   B[max_deg] = 0;
   //C[max_deg] = 1; // this term is out of range
   gfu32 c0 = 1;  // c0 is alwasy 1

   for(int i=0;i<(int)N;i++) {
      int len = min(i,max_deg) + 1;

      d = gfu32_rev_dot( &C[max_deg-1] , &coefs[i-1] , len-1 );
      d ^= gfu32_mul( c0 , coefs[i] ); // contribution of c0

      //tmp = gfu32_inv( b );
      //f = gfu32_mul( d , tmp );

      uint32_t mnz = mask_gfu32_nonzero(d);
      update_mask = mnz & mask_leq(L*2, i);

      int l = (len == max_deg+1)? max_deg:len;
      int st = max_deg-l;
      gfu32_smul( B_temp , &B[st] , d , l );
      gfu32_smul( C_temp , &C[st] , b , l );

      for(int j=0;j<l;j++) B[st+j] ^= update_mask & (B[st+j]^C[st+j]);
      B[max_deg] ^= update_mask & (B[max_deg]^c0);

      //gfu32 next_c0 = gf32_mul( c0 , b );
      for(int j=0;j<l;j++) C[st+j] = B_temp[j]^C_temp[j];
      c0 = gfu32_mul( c0 , b );

      uint32_t L_update = (i+1-L);
      L ^= update_mask & (L^L_update);
      b ^= update_mask & (d^b);

      B++;  // B[] = B[] * x
      B[max_deg] = 0;
   }


#if 119 == SYS_T
   C[max_deg] = c0;
#else
    gfu32 c0_inv = gfu32_inv(c0);
    gfu32_smul( C , C , c0_inv , max_deg );
#endif

   for(int j=0;j<((max_deg+3)/4);j++) {
      gfu32_to_gf_x4( &poly[j*4] ,  &C[j*4] );
   }
   //poly[max_deg] = 1;
#if 119 == SYS_T
   for(int i=max_deg+1;i<128;i++) poly[i]=0;
#endif
}


#endif



///////////////////////////////////////////////

#include "util.h"

#include "params.h"

#if 128 == SYS_T

void bm_128_gfu32_bs( uint32_t poly_128 [][GFBITS], uint32_t seq_256 [][ GFBITS ] )
{
  uint32_t seq[128];
  uint32_t poly[64];

  uint16_t *seq16 = (uint16_t *)&seq[0];
  for(int i=0;i<8;i++) bs32_to_gf( seq16+i*32 , seq_256[i] );

  bm_gfu32( (uint16_t*)poly , (uint16_t*)seq , 256 );

  uint16_t *poly16 = (uint16_t *)&poly[0];
  for(int i=0;i<4;i++) gf_to_bs32( poly_128[i] , poly16+i*32 );
}

#elif 119 == SYS_T

void bm_119_gfu32_bs( uint32_t poly_128 [][GFBITS], uint32_t seq_256 [][ GFBITS ] )
{
  uint32_t seq[128];
  uint32_t poly[64] = {0};

  uint16_t *seq16 = (uint16_t *)&seq[0];
  for(int i=0;i<8;i++) bs32_to_gf( seq16+i*32 , seq_256[i] );

  bm_gfu32( (uint16_t*)poly , (uint16_t*)seq , 119*2 );

  uint16_t *poly16 = (uint16_t *)&poly[0];
  for(int i=0;i<4;i++) gf_to_bs32( poly_128[i] , poly16+i*32 );

}


#elif 96 == SYS_T

void bm_96_gfu32_bs( uint32_t poly_96 [][GFBITS], uint32_t seq_192 [][ GFBITS ] )
{
  uint32_t seq[96];
  uint32_t poly[48];

  uint16_t *seq16 = (uint16_t *)&seq[0];
  for(int i=0;i<6;i++) bs32_to_gf( seq16+i*32 , seq_192[i] );

  bm_gfu32( (uint16_t*)poly , (uint16_t*)seq , 192 );

  uint16_t *poly16 = (uint16_t *)&poly[0];
  for(int i=0;i<3;i++) gf_to_bs32( poly_96[i] , poly16+i*32 );
}


#elif 64 == SYS_T

void bm_64_gfu32_bs( uint32_t poly_128 [][GFBITS], uint32_t seq_256 [][ GFBITS ] )
{
  uint32_t seq[64];
  uint32_t poly[32];

  uint16_t *seq16 = (uint16_t *)&seq[0];
  for(int i=0;i<4;i++) bs32_to_gf( seq16+i*32 , seq_256[i] );

  bm_gfu32( (uint16_t*)poly , (uint16_t*)seq , 128 );

  uint16_t *poly16 = (uint16_t *)&poly[0];
  for(int i=0;i<2;i++) gf_to_bs32( poly_128[i] , poly16+i*32 );
}

#else
error: un-supported SYS_T
#endif
