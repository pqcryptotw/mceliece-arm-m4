/*
  matrix library for public-key generation
*/


#include "matrix_mul.h"



/////////////////////////////////////////////////////



void matrix_mul_256( uint32_t *mat_c , const uint32_t* mat_a , int w_a, const uint32_t *mat_b , int w_b )
{
  int h_a = w_a*32;
  for(int i=0;i<h_a;i++) for(int j=0;j<w_b;j++) mat_c[i*w_b+j] = 0;

  for(int i=0;i<w_b;i+=(256/32)){
    for(int j=0;j<h_a;j+=256) {
      for(int k=0;k<w_a;k+=(256/32)) {
        gf2_submat256x256_madd( &mat_c[j*w_b+i], w_b, &mat_a[j*w_a+k], w_a, &mat_b[k*32*w_b+i], w_b);
      }
    }
  }
}


void matrix_mul_128( uint32_t *mat_c , const uint32_t* mat_a , int w_a, const uint32_t *mat_b , int w_b )
{
  int h_a = w_a*32;
  for(int i=0;i<h_a;i++) for(int j=0;j<w_b;j++) mat_c[i*w_b+j] = 0;

  for(int i=0;i<w_b;i+=(128/32)){
    for(int j=0;j<h_a;j+=128) {
      for(int k=0;k<w_a;k+=(128/32)) {
        gf2_submat128x128_madd( &mat_c[j*w_b+i], w_b, &mat_a[j*w_a+k], w_a, &mat_b[k*32*w_b+i], w_b);
      }
    }
  }
}


void matrix_mul_64( uint32_t *mat_c , const uint32_t* mat_a , int w_a, const uint32_t *mat_b , int w_b )
{
  int h_a = w_a*32;
  for(int i=0;i<h_a;i++) for(int j=0;j<w_b;j++) mat_c[i*w_b+j] = 0;

  for(int i=0;i<w_b;i+=(64/32)){
    for(int j=0;j<h_a;j+=64) {
      for(int k=0;k<w_a;k+=(64/32)) {
        gf2_submat64x64_madd( &mat_c[j*w_b+i], w_b, &mat_a[j*w_a+k], w_a, &mat_b[k*32*w_b+i], w_b);
      }
    }
  }
}


void matrix_mul_64_ext_buffer( uint32_t *mat_c , const uint32_t* mat_a , int w_a, const uint32_t *mat_b , int w_b , uint32_t *buffer_448u32)
{
  int h_a = w_a*32;
  for(int i=0;i<h_a;i++) for(int j=0;j<w_b;j++) mat_c[i*w_b+j] = 0;

  for(int i=0;i<w_b;i+=(64/32)){
    for(int j=0;j<h_a;j+=64) {
      for(int k=0;k<w_a;k+=(64/32)) {
        gf2_submat64x64_madd_ext_buffer( &mat_c[j*w_b+i], w_b, &mat_a[j*w_a+k], w_a, &mat_b[k*32*w_b+i], w_b, buffer_448u32 );
      }
    }
  }
}


void matrix_mul_32( uint32_t *mat_c , const uint32_t* mat_a , int w_a, const uint32_t *mat_b , int w_b )
{
  int h_a = w_a*32;
  for(int i=0;i<h_a;i++) for(int j=0;j<w_b;j++) mat_c[i*w_b+j] = 0;

  for(int i=0;i<w_b;i++){
    for(int j=0;j<h_a;j+=32) {
      for(int k=0;k<w_a;k++) {
        gf2_submat32x32_madd( &mat_c[j*w_b+i], w_b, &mat_a[j*w_a+k], w_a, &mat_b[k*32*w_b+i], w_b);
      }
    }
  }
}


void matrix_mul( uint32_t *mat_c , const uint32_t* mat_a , int w_a , const uint32_t *mat_b , int w_b )
{
	int h_a = w_a*32;
	for(int i=0;i<h_a;i++) {
		const uint32_t *row_a = mat_a+w_a*i;
		uint32_t mask = row_a[0];
		mask &= 1;
		mask = -mask;
		for(int k=0;k<w_b;k++) mat_c[i*w_b+k] = mask&mat_b[0*w_b + k];
		for(int j=1;j<h_a;j++) {
			mask = row_a[j>>5]>>(j&31);
			mask &= 1;
			mask = -mask;
			for(int k=0;k<w_b;k++) mat_c[i*w_b+k] ^= mask&mat_b[j*w_b + k];
		}
	}
}

///////////////////////////////////////////////////////////////






static
void gf2_submat32x4_madd(uint32_t *mat_c, int w_c, const uint32_t *mat_a_32x4, int w_a, const uint32_t *mat_b_32x32, int w_b)
{
  uint32_t rc0 = mat_c[0];
  uint32_t rc1 = mat_c[w_c];
  uint32_t rc2 = mat_c[2*w_c];
  uint32_t rc3 = mat_c[3*w_c];
  uint32_t a0 = mat_a_32x4[0];
  uint32_t a1 = mat_a_32x4[w_a];
  uint32_t a2 = mat_a_32x4[2*w_a];
  uint32_t a3 = mat_a_32x4[3*w_a];

  for(int i=0;i<32;i++) {
    uint32_t bi = mat_b_32x32[0];
    mat_b_32x32 += w_b;
    rc0 ^= bi*((a0>>i)&1);
    rc1 ^= bi*((a1>>i)&1);
    rc2 ^= bi*((a2>>i)&1);
    rc3 ^= bi*((a3>>i)&1);
  }
  mat_c[0] = rc0;
  mat_c[w_c] = rc1;
  mat_c[2*w_c] = rc2;
  mat_c[3*w_c] = rc3;
}




void gf2_submat32x32_madd(uint32_t *mat_c, int w_c, const uint32_t *mat_a, int w_a, const uint32_t *mat_b, int w_b)
{
  for(int i=0;i<32;i+=4) {
    gf2_submat32x4_madd(mat_c+i*w_c, w_c, mat_a+i*w_a, w_a, mat_b, w_b);
  }
}


static inline
void matmadd_32x4(uint32_t *c, const uint32_t *a, const uint32_t *b) {
  uint32_t c0=c[0];
  uint32_t c1=c[1];
  uint32_t c2=c[2];
  uint32_t c3=c[3];
  uint32_t a0=a[0];
  uint32_t a1=a[1];
  uint32_t a2=a[2];
  uint32_t a3=a[3];
  for(int i=0;i<32;i++) {
    uint32_t bi=b[i];
    c0 ^= bi*((a0>>i)&1);
    c1 ^= bi*((a1>>i)&1);
    c2 ^= bi*((a2>>i)&1);
    c3 ^= bi*((a3>>i)&1);
  }
  c[0] = c0;
  c[1] = c1;
  c[2] = c2;
  c[3] = c3;
}


static inline
void matmadd_32x32(uint32_t *c, const uint32_t *a, const uint32_t *b) {
  for(int i=0;i<32;i+=4) matmadd_32x4(c+i,a+i,b);
}

/////

static inline
void matmul_32x4(uint32_t *c, const uint32_t *a, const uint32_t *b) {
  uint32_t c0;
  uint32_t c1;
  uint32_t c2;
  uint32_t c3;
  uint32_t a0=a[0];
  uint32_t a1=a[1];
  uint32_t a2=a[2];
  uint32_t a3=a[3];
  {
    uint32_t bi=b[0];
    c0 = bi*(a0&1);
    c1 = bi*(a1&1);
    c2 = bi*(a2&1);
    c3 = bi*(a3&1);
  }
  for(int i=1;i<32;i++) {
    uint32_t bi=b[i];
    c0 ^= bi*((a0>>i)&1);
    c1 ^= bi*((a1>>i)&1);
    c2 ^= bi*((a2>>i)&1);
    c3 ^= bi*((a3>>i)&1);
  }
  c[0] = c0;
  c[1] = c1;
  c[2] = c2;
  c[3] = c3;
}


static inline
void matmul_32x32(uint32_t *c, const uint32_t *a, const uint32_t *b) {
  for(int i=0;i<32;i+=4) matmul_32x4(c+i,a+i,b);
}

/////

// strassen matrix multiplication
//
// [ a11  a12 ]  x  [ b11  b12 ]
// [ a21  a22 ]     [ b21  b22 ]
//
// s1 = a21 + a22               t1 = b12 - b11
// s2 = (s1:a21+a22) - a11      t2 = b22 - (t1:b12-b11)
// s3 = a11 - a21               t3 = b22 - b12
// s4 = a12 - (s2:a21+a22-a11)  t4 = (t2:b22-(b12-b11)) - b21
//
// p1 = a11 b11        <-- order: 1
// p2 = a12 b21        <-- order: 2
// p3 = s4 b22
// p4 = a22 t4
// p5 = s1 t1
// p6 = s2 t2          <-- order: 3
// p7 = s3 t3          <-- order: 4
//
// u1 = p1+p2  // release p2    // write back u1
// u2 = p1+p6  // release p1, p6
// u3 = u2+p7  // release p7
// u4 = u2+p5  // release u2
// u5 = u4+p3  // release u4, p3  // write back u5
// u6 = u3-p4  // release p4      // write back u6
// u7 = u3+p5  // release u3, p5  // write back u7
//
// return [ u1  u5 ]
//        [ u6  u7 ]

#if 0

void gf2_submat64x64_madd(uint32_t *mat_c, int w_c, const uint32_t *mat_a, int w_a, const uint32_t *mat_b, int w_b)
{
  uint32_t buffer[32*6];
// s3 = a11 - a21               t3 = b22 - b12
  uint32_t *s3=buffer; for(int i=0;i<32;i++) s3[i] = mat_a[i*w_a]       ^mat_a[32*w_a+i*w_a];
  uint32_t *t3=&buffer[32]; for(int i=0;i<32;i++) t3[i] = mat_b[32*w_b+i*w_b+1]^mat_b[i*w_b+1];
// p7 = s3 t3
  uint32_t *p7=&buffer[32*2]; for(int i=0;i<32;i++) p7[i]=0; gf2_submat32x32_madd(p7,1,s3,1,t3,1);

// s1 = a21 + a22               t1 = b12 - b11
// s2 = (s1:a21+a22) - a11      t2 = b22 - (t1:b12-b11)
  uint32_t *s1=s3; for(int i=0;i<32;i++) s1[i] = mat_a[32*w_a+i*w_a]^mat_a[32*w_a+i*w_a+1];
  uint32_t *t1=t3; for(int i=0;i<32;i++) t1[i] = mat_b[i*w_b]         ^mat_b[i*w_b+1];
  uint32_t *s2=&buffer[32*3]; for(int i=0;i<32;i++) s2[i] = mat_a[i*w_a]       ^s1[i];
  uint32_t *t2=&buffer[32*4]; for(int i=0;i<32;i++) t2[i] = mat_b[32*w_b+i*w_b+1]^t1[i];
// p5 = s1 t1
  uint32_t *p5=&buffer[32*5]; for(int i=0;i<32;i++) p5[i]=0; gf2_submat32x32_madd(p5,1,s1,1,t1,1);
// p6 = s2 t2
  uint32_t *p6=s1; for(int i=0;i<32;i++) p6[i]=0; gf2_submat32x32_madd(p6,1,s2,1,t2,1);
// s4 = a12 - (s2:a21+a22-a11)  t4 = (t2:b22-(b12-b11)) - b21
  uint32_t *s4=t1; for(int i=0;i<32;i++) s4[i] = mat_a[i*w_a+1]     ^s2[i];
  uint32_t *t4=s2; for(int i=0;i<32;i++) t4[i] = mat_b[32*w_b+i*w_b]  ^t2[i];

// p4 = a22 t4
  uint32_t* p4 = t2; for(int i=0;i<32;i++) p4[i]=0; gf2_submat32x32_madd(p4,1,mat_a+32*w_a+1,w_a,t4,1);
// p3 = s4 b22
  uint32_t* p3 = t4; for(int i=0;i<32;i++) p3[i]=0; gf2_submat32x32_madd(p3,1,s4,1,mat_b+32*w_b+1,w_b);
// p1 = a11 b11
  uint32_t* p1 = s4; for(int i=0;i<32;i++) p1[i]=0; gf2_submat32x32_madd(p1,1,mat_a,w_a,mat_b,w_b);

// u2 = p1+p6
// u3 = u2+p7
// u4 = u2+p5
// u5 = u4+p3
// u6 = u3-p4
// u7 = u3+p5
  uint32_t* u2 = p6;
  for(int i=0;i<32;i++) { u2[i] ^= p1[i]; }
  uint32_t* u3 = p7;
  uint32_t* u4 = u2;
  for(int i=0;i<32;i++) { u3[i] ^= u2[i]; u4[i] ^= p5[i]; }
  uint32_t* u5 = p3;
  for(int i=0;i<32;i++) u5[i] ^= u4[i];
  uint32_t* u6 = p4;
  uint32_t* u7 = p5;
  for(int i=0;i<32;i++) { u6[i] ^= u3[i]; u7[i] ^= u3[i]; }

// p2 = a12 b21
  uint32_t* p2 = u4; for(int i=0;i<32;i++) p2[i]=0; gf2_submat32x32_madd(p2,1,mat_a+1,w_a,mat_b+32*w_b,w_b);
// u1 = p1+p2
  uint32_t* u1 = p2;
  for(int i=0;i<32;i++) { u1[i] ^= p1[i]; }

  for(int i=0;i<32;i++) { mat_c[i*w_c] ^= u1[i]; mat_c[i*w_c+1] ^= u5[i]; }
  for(int i=0;i<32;i++) { mat_c[32*w_c+i*w_c] ^= u6[i]; mat_c[32*w_c+i*w_c+1] ^= u7[i]; }
}


#else

static inline void cpy_32x32( uint32_t *d , const uint32_t *s, int w_s ) { for(int i=0;i<32;i++) d[i] = s[w_s*i]; }

void gf2_submat64x64_madd(uint32_t *mat_c, int w_c, const uint32_t *mat_a, int w_a, const uint32_t *mat_b, int w_b)
{
  uint32_t buf_a[32];
  uint32_t buf_b[32];
  uint32_t buffer[32*6];
// s3 = a11 - a21               t3 = b22 - b12
  uint32_t *s3=buffer; for(int i=0;i<32;i++) s3[i] = mat_a[i*w_a]       ^mat_a[32*w_a+i*w_a];
  uint32_t *t3=&buffer[32]; for(int i=0;i<32;i++) t3[i] = mat_b[32*w_b+i*w_b+1]^mat_b[i*w_b+1];
// p7 = s3 t3
  uint32_t *p7=&buffer[32*2]; for(int i=0;i<32;i++) p7[i]=0;
  matmadd_32x32(p7,s3,t3);
  //gf2_submat32x32_madd(p7,1,s3,1,t3,1);

// s1 = a21 + a22               t1 = b12 - b11
// s2 = (s1:a21+a22) - a11      t2 = b22 - (t1:b12-b11)
  uint32_t *s1=s3; for(int i=0;i<32;i++) s1[i] = mat_a[32*w_a+i*w_a]^mat_a[32*w_a+i*w_a+1];
  uint32_t *t1=t3; for(int i=0;i<32;i++) t1[i] = mat_b[i*w_b]         ^mat_b[i*w_b+1];
  uint32_t *s2=&buffer[32*3]; for(int i=0;i<32;i++) s2[i] = mat_a[i*w_a]       ^s1[i];
  uint32_t *t2=&buffer[32*4]; for(int i=0;i<32;i++) t2[i] = mat_b[32*w_b+i*w_b+1]^t1[i];
// p5 = s1 t1
  uint32_t *p5=&buffer[32*5]; for(int i=0;i<32;i++) p5[i]=0;
  matmadd_32x32(p5,s1,t1);
  //gf2_submat32x32_madd(p5,1,s1,1,t1,1);
// p6 = s2 t2
  uint32_t *p6=s1; for(int i=0;i<32;i++) p6[i]=0;
  matmadd_32x32(p6,s2,t2);
  //gf2_submat32x32_madd(p6,1,s2,1,t2,1);
// s4 = a12 - (s2:a21+a22-a11)  t4 = (t2:b22-(b12-b11)) - b21
  uint32_t *s4=t1; for(int i=0;i<32;i++) s4[i] = mat_a[i*w_a+1]     ^s2[i];
  uint32_t *t4=s2; for(int i=0;i<32;i++) t4[i] = mat_b[32*w_b+i*w_b]  ^t2[i];

// p4 = a22 t4
  uint32_t* p4 = t2; for(int i=0;i<32;i++) p4[i]=0;
  cpy_32x32(buf_a,mat_a+32*w_a+1,w_a); matmadd_32x32(p4,buf_a,t4);
  //gf2_submat32x32_madd(p4,1,mat_a+32*w_a+1,w_a,t4,1);
// p3 = s4 b22
  uint32_t* p3 = t4; for(int i=0;i<32;i++) p3[i]=0;
  cpy_32x32(buf_b,mat_b+32*w_b+1,w_b); matmadd_32x32(p3,s4,buf_b);
  //gf2_submat32x32_madd(p3,1,s4,1,mat_b+32*w_b+1,w_b);
// p1 = a11 b11
  uint32_t* p1 = s4; for(int i=0;i<32;i++) p1[i]=0;
  cpy_32x32(buf_a,mat_a,w_a); cpy_32x32(buf_b,mat_b,w_b); matmadd_32x32(p1,buf_a,buf_b);
  //gf2_submat32x32_madd(p1,1,mat_a,w_a,mat_b,w_b);

// u2 = p1+p6
// u3 = u2+p7
// u4 = u2+p5
// u5 = u4+p3
// u6 = u3-p4
// u7 = u3+p5
  uint32_t* u2 = p6;
  for(int i=0;i<32;i++) { u2[i] ^= p1[i]; }
  uint32_t* u3 = p7;
  uint32_t* u4 = u2;
  for(int i=0;i<32;i++) { u3[i] ^= u2[i]; u4[i] ^= p5[i]; }
  uint32_t* u5 = p3;
  for(int i=0;i<32;i++) u5[i] ^= u4[i];
  uint32_t* u6 = p4;
  uint32_t* u7 = p5;
  for(int i=0;i<32;i++) { u6[i] ^= u3[i]; u7[i] ^= u3[i]; }

// p2 = a12 b21
  uint32_t* p2 = u4; for(int i=0;i<32;i++) p2[i]=0; gf2_submat32x32_madd(p2,1,mat_a+1,w_a,mat_b+32*w_b,w_b);
// u1 = p1+p2
  uint32_t* u1 = p2;
  for(int i=0;i<32;i++) { u1[i] ^= p1[i]; }

  for(int i=0;i<32;i++) { mat_c[i*w_c] ^= u1[i]; mat_c[i*w_c+1] ^= u5[i]; }
  for(int i=0;i<32;i++) { mat_c[32*w_c+i*w_c] ^= u6[i]; mat_c[32*w_c+i*w_c+1] ^= u7[i]; }
}


#endif


//////////////////////////////////////////////




/////////////////////////////////////////////////


// [ a11  a12 ]  x  [ b11  b12 ]
// [ a21  a22 ]     [ b21  b22 ]
//
// s1 = a21 + a22               t1 = b12 - b11
// s2 = (s1:a21+a22) - a11      t2 = b22 - (t1:b12-b11)
// s3 = a11 - a21               t3 = b22 - b12
// s4 = a12 - (s2:a21+a22-a11)  t4 = (t2:b22-(b12-b11)) - b21
//
// p1 = a11 b11        <-- order: 1
// p2 = a12 b21        <-- order: 2
// p6 = s2 t2          <-- order: 3
// p7 = s3 t3          <-- order: 4
// p5 = s1 t1
// p3 = s4 b22
// p4 = a22 t4

void gf2mat_input_64x64_s(uint32_t *s, const uint32_t *mat_a, int w_a)
{
  uint32_t * p1 = s;
  uint32_t * p2 = p1 + 32;
  uint32_t * p6 = p2 + 32;
  uint32_t * p7 = p6 + 32;
  uint32_t * p5 = p7 + 32;
  uint32_t * p3 = p5 + 32;
  uint32_t * p4 = p3 + 32;

  cpy_32x32(p1,mat_a,w_a);          // a11
  cpy_32x32(p2,mat_a+1,w_a);        // a12
  cpy_32x32(p5,mat_a+w_a*32,w_a);   // a21
  cpy_32x32(p4,mat_a+w_a*32+1,w_a); // a22

  for(int i=0;i<32;i++) p7[i] = p5[i]^p1[i];  // s3
  for(int i=0;i<32;i++) {
    p5[i] ^= p4[i];  // s1
    p6[i] = p5[i]^p1[i];  // s2
    p3[i] = p6[i]^p2[i]; // s4
  }
}

// [ a11  a12 ]  x  [ b11  b12 ]
// [ a21  a22 ]     [ b21  b22 ]
//
// s1 = a21 + a22               t1 = b12 - b11
// s2 = (s1:a21+a22) - a11      t2 = b22 - (t1:b12-b11)
// s3 = a11 - a21               t3 = b22 - b12
// s4 = a12 - (s2:a21+a22-a11)  t4 = (t2:b22-(b12-b11)) - b21
//
// p1 = a11 b11        <-- order: 1
// p2 = a12 b21        <-- order: 2
// p6 = s2 t2          <-- order: 3
// p7 = s3 t3          <-- order: 4
// p5 = s1 t1
// p3 = s4 b22
// p4 = a22 t4

void gf2mat_input_64x64_t(uint32_t *t, const uint32_t *mat_b, int w_b)
{
  uint32_t * p1 = t;
  uint32_t * p2 = p1 + 32;
  uint32_t * p6 = p2 + 32;
  uint32_t * p7 = p6 + 32;
  uint32_t * p5 = p7 + 32;
  uint32_t * p3 = p5 + 32;
  uint32_t * p4 = p3 + 32;

  cpy_32x32(p1,mat_b,w_b);          // b11
  cpy_32x32(p5,mat_b+1,w_b);        // b12
  cpy_32x32(p2,mat_b+w_b*32,w_b);   // b21
  cpy_32x32(p3,mat_b+w_b*32+1,w_b); // b22

  for(int i=0;i<32;i++) p7[i] = p5[i]^p3[i];  // t3
  for(int i=0;i<32;i++) {
    p5[i] ^= p1[i];  // t1
    p6[i] = p3[i]^p5[i];  // t2
    p4[i] = p6[i]^p2[i]; // t4
  }
}


//////////////////////////////////////


// p1 = a11 b11        <-- order: 1
// p2 = a12 b21        <-- order: 2
// p6 = s2 t2          <-- order: 3
// p7 = s3 t3          <-- order: 4
// p5 = s1 t1
// p3 = s4 b22
// p4 = a22 t4
//
// u1 = p1+p2  // release p2    // write back u1
// u2 = p1+p6  // release p1, p6
// u3 = u2+p7  // release p7
// u4 = u2+p5  // release u2
// u5 = u4+p3  // release u4, p3  // write back u5
// u6 = u3-p4  // release p4      // write back u6
// u7 = u3+p5  // release u3, p5  // write back u7
//
// return [ u1  u5 ]
//        [ u6  u7 ]

void gf2mat_madd_64x64_with_transformed_inputs(uint32_t *mat_c, int w_c, const uint32_t *inp_a, const uint32_t *inp_b )
{
  uint32_t buffer[32*3];
  uint32_t *p1 = buffer;
  uint32_t *p2 = p1+32;
  matmul_32x32(p1,inp_a,inp_b);
  matmul_32x32(p2,inp_a+32,inp_b+32);
  for(int i=0;i<32;i++) p2[i] ^= p1[i];
  for(int i=0;i<32;i++) mat_c[i*w_c] ^= p2[i];  // wb u1
  mat_c += 1;

  uint32_t *u2 = p1;
  matmadd_32x32(p1,inp_a+32*2,inp_b+32*2); // p6

  uint32_t *u3 = p1+32;
  matmul_32x32(u3,inp_a+32*3,inp_b+32*3); // p7
  for(int i=0;i<32;i++) u3[i] ^= u2[i];

  uint32_t *u4 = u2;  // stored in p1
  uint32_t *p5 = p1 + 32*2;
  matmul_32x32(p5,inp_a+32*4,inp_b+32*4); // p5
  for(int i=0;i<32;i++) u4[i] ^= p5[i]; // u4 = u2+p5

  uint32_t *u5 = u4;  // stored in p1
  matmadd_32x32(u5,inp_a+32*5,inp_b+32*5); // u5 = u4+p3
  for(int i=0;i<32;i++) mat_c[i*w_c] ^= u5[i];  // wb u5
  mat_c += 32*w_c-1;

  uint32_t *p4 = p1;
  matmul_32x32(p4,inp_a+32*6,inp_b+32*6);
  for(int i=0;i<32;i++) mat_c[i*w_c] ^= u3[i]^p4[i]; // wb u6=u3+p1
  mat_c += 1;

  for(int i=0;i<32;i++) mat_c[i*w_c] ^= u3[i]^p5[i]; // wb u7=u3+p5
}




void gf2_submat64x64_madd_ext_buffer(uint32_t *mat_c, int w_c, const uint32_t *mat_a, int w_a, const uint32_t *mat_b, int w_b, uint32_t *buffer_448u32)
{

  uint32_t * inp_a = buffer_448u32;
  uint32_t * inp_b = buffer_448u32+32*7;

  gf2mat_input_64x64_s( inp_a , mat_a , w_a );
  gf2mat_input_64x64_t( inp_b , mat_b , w_b );

  gf2mat_madd_64x64_with_transformed_inputs( mat_c, w_c, inp_a, inp_b );

}







////////////////////////////////////////////////


void gf2_submat128x128_madd(uint32_t *mat_c, int w_c, const uint32_t *mat_a, int w_a, const uint32_t *mat_b, int w_b)
{
  uint32_t buffer[128*6];
// s3 = a11 - a21               t3 = b22 - b12
  uint32_t *s3=&buffer[128*0]; for(int i=0;i<64;i++){ s3[i*2] = mat_a[i*w_a]       ^mat_a[64*w_a+i*w_a];   s3[i*2+1] = mat_a[i*w_a+1]       ^mat_a[64*w_a+i*w_a+1]; }
  uint32_t *t3=&buffer[128*1]; for(int i=0;i<64;i++){ t3[i*2] = mat_b[64*w_b+i*w_b+2]^mat_b[i*w_b+2];      t3[i*2+1] = mat_b[64*w_b+i*w_b+3]^mat_b[i*w_b+3]; }
// p7 = s3 t3
  uint32_t *p7=&buffer[128*2]; for(int i=0;i<128;i++) p7[i]=0; gf2_submat64x64_madd(p7,2,s3,2,t3,2);

// s4 = a12 - (s2:a21+a22-a11)  t4 = (t2:b22-(b12-b11)) - b21
  uint32_t *s1=s3; for(int i=0;i<64;i++){ s1[i*2] = mat_a[64*w_a+i*w_a]^mat_a[64*w_a+i*w_a+2]; s1[i*2+1] = mat_a[64*w_a+i*w_a+1]^mat_a[64*w_a+i*w_a+3]; }
  uint32_t *t1=t3; for(int i=0;i<64;i++){ t1[i*2] = mat_b[i*w_b]         ^mat_b[i*w_b+2];      t1[i*2+1] = mat_b[i*w_b+1]       ^mat_b[i*w_b+3]; }
  uint32_t *s2=&buffer[128*3]; for(int i=0;i<64;i++){ s2[i*2] = mat_a[i*w_a]       ^s1[i*2];               s2[i*2+1] = mat_a[i*w_a+1]       ^s1[i*2+1]; }
  uint32_t *t2=&buffer[128*4]; for(int i=0;i<64;i++){ t2[i*2] = mat_b[64*w_b+i*w_b+2]^t1[i*2];             t2[i*2+1] = mat_b[64*w_b+i*w_b+3]^t1[i*2+1]; }

// p5 = s1 t1
  uint32_t *p5=&buffer[128*5]; for(int i=0;i<128;i++) p5[i]=0; gf2_submat64x64_madd(p5,2,s1,2,t1,2);

// p6 = s2 t2
  uint32_t* p6 = s1; for(int i=0;i<128;i++) p6[i]=0; gf2_submat64x64_madd(p6,2,s2,2,t2,2);
// s4 = a12 - (s2:a21+a22-a11)  t4 = (t2:b22-(b12-b11)) - b21
  uint32_t *s4=t1; for(int i=0;i<64;i++){ s4[i*2] = mat_a[i*w_a+2]     ^s2[i*2];               s4[i*2+1] = mat_a[i*w_a+3]       ^s2[i*2+1]; }
  uint32_t *t4=s2; for(int i=0;i<64;i++){ t4[i*2] = mat_b[64*w_b+i*w_b]  ^t2[i*2];             t4[i*2+1] = mat_b[64*w_b+i*w_b+1]^t2[i*2+1]; }

// p4 = a22 t4
  uint32_t* p4 = t2; for(int i=0;i<128;i++) p4[i]=0; gf2_submat64x64_madd(p4,2,mat_a+64*w_a+2,w_a,t4,2);
// p3 = s4 b22
  uint32_t* p3 = t4; for(int i=0;i<128;i++) p3[i]=0; gf2_submat64x64_madd(p3,2,s4,2,mat_b+64*w_b+2,w_b);
// p1 = a11 b11
  uint32_t* p1 = s4; for(int i=0;i<128;i++) p1[i]=0; gf2_submat64x64_madd(p1,2,mat_a,w_a,mat_b,w_b);

// u2 = p1+p6
// u3 = u2+p7
// u4 = u2+p5
// u5 = u4+p3
// u6 = u3-p4
// u7 = u3+p5
  uint32_t* u2 = p6;
  for(int i=0;i<128;i++) { u2[i] ^= p1[i]; }
  uint32_t* u3 = p7;
  uint32_t* u4 = u2;
  for(int i=0;i<128;i++) { u3[i] ^= u2[i]; u4[i] ^= p5[i]; }
  uint32_t* u5 = p3;
  for(int i=0;i<128;i++) u5[i] ^= u4[i];
  uint32_t* u6 = p4;
  uint32_t* u7 = p5;
  for(int i=0;i<128;i++) { u6[i] ^= u3[i]; u7[i] ^= u3[i]; }

// p2 = a12 b21
  uint32_t* p2 = u4; for(int i=0;i<128;i++) p2[i]=0; gf2_submat64x64_madd(p2,2,mat_a+2,w_a,mat_b+64*w_b,w_b);
// u1 = p1+p2
  uint32_t* u1 = p2;
  for(int i=0;i<128;i++) { u1[i] ^= p1[i]; }

  for(int i=0;i<64;i++) {
    mat_c[i*w_c] ^= u1[i*2]; mat_c[i*w_c+1] ^= u1[i*2+1];
    mat_c[i*w_c+2] ^= u5[i*2]; mat_c[i*w_c+3] ^= u5[i*2+1];
  }
  for(int i=0;i<64;i++) {
    mat_c[64*w_c+i*w_c] ^= u6[i*2]; mat_c[64*w_c+i*w_c+1] ^= u6[i*2+1];
    mat_c[64*w_c+i*w_c+2] ^= u7[i*2]; mat_c[64*w_c+i*w_c+3] ^= u7[i*2+1];
  }
}






void gf2_submat256x256_madd(uint32_t *mat_c, int w_c, const uint32_t *mat_a, int w_a, const uint32_t *mat_b, int w_b)
{
  uint32_t buffer[512*6];
// s3 = a11 - a21               t3 = b22 - b12
  uint32_t *s3=&buffer[512*0]; for(int i=0;i<128;i++){ for(int j=0;j<4;j++) s3[i*4+j] = mat_a[i*w_a+j]       ^mat_a[128*w_a+i*w_a+j]; }
  uint32_t *t3=&buffer[512*1]; for(int i=0;i<128;i++){ for(int j=0;j<4;j++) t3[i*4+j] = mat_b[128*w_b+i*w_b+4+j]^mat_b[i*w_b+4+j]; }
// p7 = s3 t3
  uint32_t *p7=&buffer[512*2]; for(int i=0;i<512;i++) p7[i]=0; gf2_submat128x128_madd(p7,4,s3,4,t3,4);

// s1 = a21 + a22               t1 = b12 - b11
// s2 = (s1:a21+a22) - a11      t2 = b22 - (t1:b12-b11)
  uint32_t *s1=s3; for(int i=0;i<128;i++){ for(int j=0;j<4;j++) s1[i*4+j] = mat_a[128*w_a+i*w_a+j]^mat_a[128*w_a+i*w_a+4+j]; }
  uint32_t *t1=t3; for(int i=0;i<128;i++){ for(int j=0;j<4;j++) t1[i*4+j] = mat_b[i*w_b+j]         ^mat_b[i*w_b+4+j]; }
  uint32_t *s2=&buffer[512*3]; for(int i=0;i<128;i++){ for(int j=0;j<4;j++) s2[i*4+j] = mat_a[i*w_a+j]        ^s1[i*4+j]; }
  uint32_t *t2=&buffer[512*4]; for(int i=0;i<128;i++){ for(int j=0;j<4;j++) t2[i*4+j] = mat_b[128*w_b+i*w_b+4+j]^t1[i*4+j]; }
// p5 = s1 t1
  uint32_t *p5=&buffer[512*5]; for(int i=0;i<512;i++) p5[i]=0; gf2_submat128x128_madd(p5,4,s1,4,t1,4);
// p6 = s2 t2
  uint32_t* p6 = s1; for(int i=0;i<512;i++) p6[i]=0; gf2_submat128x128_madd(p6,4,s2,4,t2,4);

// s4 = a12 - (s2:a21+a22-a11)  t4 = (t2:b22-(b12-b11)) - b21
  uint32_t *s4=t1; for(int i=0;i<128;i++){ for(int j=0;j<4;j++) s4[i*4+j] = mat_a[i*w_a+4+j]     ^s2[i*4+j]; }
  uint32_t *t4=s2; for(int i=0;i<128;i++){ for(int j=0;j<4;j++) t4[i*4+j] = mat_b[128*w_b+i*w_b+j]  ^t2[i*4+j]; }

// p4 = a22 t4
  uint32_t* p4 = t2; for(int i=0;i<512;i++) p4[i]=0; gf2_submat128x128_madd(p4,4,mat_a+128*w_a+4,w_a,t4,4);
// p3 = s4 b22
  uint32_t* p3 = t4; for(int i=0;i<512;i++) p3[i]=0; gf2_submat128x128_madd(p3,4,s4,4,mat_b+128*w_b+4,w_b);
// p1 = a11 b11
  uint32_t* p1 = s4; for(int i=0;i<512;i++) p1[i]=0; gf2_submat128x128_madd(p1,4,mat_a,w_a,mat_b,w_b);

// u2 = p1+p6
// u3 = u2+p7
// u4 = u2+p5
// u5 = u4+p3
// u6 = u3-p4
// u7 = u3+p5
  uint32_t* u2 = p6;
  for(int i=0;i<512;i++) { u2[i] ^= p1[i]; }
  uint32_t* u3 = p7;
  uint32_t* u4 = u2;
  for(int i=0;i<512;i++) { u3[i] ^= u2[i]; u4[i] ^= p5[i]; }
  uint32_t* u5 = p3;
  for(int i=0;i<512;i++) u5[i] ^= u4[i];
  uint32_t* u6 = p4;
  uint32_t* u7 = p5;
  for(int i=0;i<512;i++) { u6[i] ^= u3[i]; u7[i] ^= u3[i]; }

// p2 = a12 b21
  uint32_t* p2 = u3; for(int i=0;i<512;i++) p2[i]=0; gf2_submat128x128_madd(p2,4,mat_a+4,w_a,mat_b+128*w_b,w_b);
// u1 = p1+p2
  uint32_t* u1 = p2;
  for(int i=0;i<512;i++) { u1[i] ^= p1[i]; }

  for(int i=0;i<128;i++) {
    for(int j=0;j<4;j++) mat_c[i*w_c+j] ^= u1[i*4+j];
    for(int j=0;j<4;j++) mat_c[i*w_c+4+j] ^= u5[i*4+j];
  }
  for(int i=0;i<128;i++) {
    for(int j=0;j<4;j++) mat_c[128*w_c+i*w_c+j] ^= u6[i*4+j];
    for(int j=0;j<4;j++) mat_c[128*w_c+i*w_c+4+j] ^= u7[i*4+j];
  }
}

