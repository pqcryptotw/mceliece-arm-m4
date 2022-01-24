
#include "stdint.h"

#include "transpose.h"


#include "run_config.h"


/* input: out, a 16x16 matrix over GF(2) */
/* output: out, transpose of out */
void transpose_16x16_in_u32(uint32_t * out )
{
	uint32_t x, y;
	uint32_t i0 = out[0];
	uint32_t i1 = out[1];
	uint32_t i2 = out[2];
	uint32_t i3 = out[3];
	uint32_t i4 = out[4];
	uint32_t i5 = out[5];
	uint32_t i6 = out[6];
	uint32_t i7 = out[7];

	x = (i0&0x00ff00ff) | ((i4&0x00ff00ff)<<8);
	y = ((i0&~0x00ff00ff)>>8) | (i4&~0x00ff00ff);
	i0 = x;
	i4 = y;
	x = (i1&0x00ff00ff) | ((i5&0x00ff00ff)<<8);
	y = ((i1&~0x00ff00ff)>>8) | (i5&~0x00ff00ff);
	i1 = x;
	i5 = y;
	x = (i2&0x00ff00ff) | ((i6&0x00ff00ff)<<8);
	y = ((i2&~0x00ff00ff)>>8) | (i6&~0x00ff00ff);
	i2 = x;
	i6 = y;
	x = (i3&0x00ff00ff) | ((i7&0x00ff00ff)<<8);
	y = ((i3&~0x00ff00ff)>>8) | (i7&~0x00ff00ff);
	i3 = x;
	i7 = y;

	x = (i0&0x0f0f0f0f) | ((i2&0x0f0f0f0f)<<4);
	y = ((i0&~0x0f0f0f0f)>>4) | (i2&~0x0f0f0f0f);
	i0 = x;
	i2 = y;
	x = (i1&0x0f0f0f0f) | ((i3&0x0f0f0f0f)<<4);
	y = ((i1&~0x0f0f0f0f)>>4) | (i3&~0x0f0f0f0f);
	i1 = x;
	i3 = y;
	x = (i4&0x0f0f0f0f) | ((i6&0x0f0f0f0f)<<4);
	y = ((i4&~0x0f0f0f0f)>>4) | (i6&~0x0f0f0f0f);
	i4 = x;
	i6 = y;
	x = (i5&0x0f0f0f0f) | ((i7&0x0f0f0f0f)<<4);
	y = ((i5&~0x0f0f0f0f)>>4) | (i7&~0x0f0f0f0f);
	i5 = x;
	i7 = y;

	x = (i0&0x33333333) | ((i1&0x33333333)<<2);
	y = ((i0&~0x33333333)>>2) | (i1&~0x33333333);
	i0 = x;
	i1 = y;
	x = (i2&0x33333333) | ((i3&0x33333333)<<2);
	y = ((i2&~0x33333333)>>2) | (i3&~0x33333333);
	i2 = x;
	i3 = y;
	x = (i4&0x33333333) | ((i5&0x33333333)<<2);
	y = ((i4&~0x33333333)>>2) | (i5&~0x33333333);
	i4 = x;
	i5 = y;
	x = (i6&0x33333333) | ((i7&0x33333333)<<2);
	y = ((i6&~0x33333333)>>2) | (i7&~0x33333333);
	i6 = x;
	i7 = y;

	i0 = (i0&~0x5555aaaa)|((i0&0xaaaa)<<15)|((i0&0x55550000)>>15);
	out[0] = i0;
	i1 = (i1&~0x5555aaaa)|((i1&0xaaaa)<<15)|((i1&0x55550000)>>15);
	out[1] = i1;
	i2 = (i2&~0x5555aaaa)|((i2&0xaaaa)<<15)|((i2&0x55550000)>>15);
	out[2] = i2;
	i3 = (i3&~0x5555aaaa)|((i3&0xaaaa)<<15)|((i3&0x55550000)>>15);
	out[3] = i3;
	i4 = (i4&~0x5555aaaa)|((i4&0xaaaa)<<15)|((i4&0x55550000)>>15);
	out[4] = i4;
	i5 = (i5&~0x5555aaaa)|((i5&0xaaaa)<<15)|((i5&0x55550000)>>15);
	out[5] = i5;
	i6 = (i6&~0x5555aaaa)|((i6&0xaaaa)<<15)|((i6&0x55550000)>>15);
	out[6] = i6;
	i7 = (i7&~0x5555aaaa)|((i7&0xaaaa)<<15)|((i7&0x55550000)>>15);
	out[7] = i7;
}


#if defined(_M4_ASM_)

extern void transpose_32x32_in_asm(uint32_t *);

void transpose_32x32_in(uint32_t * out)
{
  transpose_32x32_in_asm(out);
}

#else

static inline void transpose_8x8_x4_in(uint32_t * out)
{
  uint32_t i0=out[0];
  uint32_t i1=out[1];
  uint32_t i2=out[2];
  uint32_t i3=out[3];
  uint32_t i4=out[4];
  uint32_t i5=out[5];
  uint32_t i6=out[6];
  uint32_t i7=out[7];
  uint32_t x,y;
  uint32_t mask;
  // 4x4
  mask = 0x0f0f0f0f;
  x = (i0&mask)|((i4&mask)<<4);
  y = ((i0&~mask)>>4)|(i4&~mask);
  i0 = x; i4 = y;
  x = (i1&mask)|((i5&mask)<<4);
  y = ((i1&~mask)>>4)|(i5&~mask);
  i1 = x; i5 = y;
  x = (i2&mask)|((i6&mask)<<4);
  y = ((i2&~mask)>>4)|(i6&~mask);
  i2 = x; i6 = y;
  x = (i3&mask)|((i7&mask)<<4);
  y = ((i3&~mask)>>4)|(i7&~mask);
  i3 = x; i7 = y;
  // 2x2
  mask = 0x33333333;
  x = (i0&mask)|((i2&mask)<<2);
  y = ((i0&~mask)>>2)|(i2&~mask);
  i0 = x; i2 = y;
  x = (i1&mask)|((i3&mask)<<2);
  y = ((i1&~mask)>>2)|(i3&~mask);
  i1 = x; i3 = y;
  x = (i4&mask)|((i6&mask)<<2);
  y = ((i4&~mask)>>2)|(i6&~mask);
  i4 = x; i6 = y;
  x = (i5&mask)|((i7&mask)<<2);
  y = ((i5&~mask)>>2)|(i7&~mask);
  i5 = x; i7 = y;
  // 1x1
  mask = 0x55555555;
  x = (i0&mask)|((i1&mask)<<1);
  y = ((i0&~mask)>>1)|(i1&~mask);
  i0 = x; i1 = y;
  x = (i2&mask)|((i3&mask)<<1);
  y = ((i2&~mask)>>1)|(i3&~mask);
  i2 = x; i3 = y;
  x = (i4&mask)|((i5&mask)<<1);
  y = ((i4&~mask)>>1)|(i5&~mask);
  i4 = x; i5 = y;
  x = (i6&mask)|((i7&mask)<<1);
  y = ((i6&~mask)>>1)|(i7&~mask);
  i6 = x; i7 = y;
  out[0] = i0;
  out[1] = i1;
  out[2] = i2;
  out[3] = i3;
  out[4] = i4;
  out[5] = i5;
  out[6] = i6;
  out[7] = i7;
}


/* input: out, a 32x32 matrix over GF(2) */
/* output: out, transpose of out */
void transpose_32x32_in(uint32_t * out)
{
  int i;
  uint32_t x, y;
  uint32_t mask;
  transpose_8x8_x4_in(out);
  transpose_8x8_x4_in(out+8);
  transpose_8x8_x4_in(out+16);
  transpose_8x8_x4_in(out+24);
  mask = 0x00ff00ff;
  for(i=0;i<8;i++) {
    x = (out[i]&mask)|((out[8+i]&mask)<<8);
    y = ((out[i]&~mask)>>8)|((out[8+i]&~mask));
    out[i] = x; out[8+i] = y;
  }
  for(i=0;i<8;i++) {
    x = (out[16+i]&mask)|((out[16+8+i]&mask)<<8);
    y = ((out[16+i]&~mask)>>8)|((out[16+8+i]&~mask));
    out[16+i] = x; out[16+8+i] = y;
  }
  mask = 0x0000ffff;
  for(i=0;i<16;i++) {
    x = (out[i]&mask)|((out[16+i]&mask)<<16);
    y = ((out[i]&~mask)>>16)|((out[16+i]&~mask));
    out[i] = x; out[16+i] = y;
  }
}


#endif
