
#include "sort_uint32.h"

#include <stdint.h>

typedef char bit;

static inline
bit is_smaller(uint32_t a, uint32_t b)
{
  uint32_t ret = 0;

  ret = a - b;
  ret >>= 31;

  return ret;
}

#define _NO_REPETITION_HIGH_

#if defined(_NO_REPETITION_HIGH_)
static inline
bit is_smaller_payload(uint32_t a1, uint32_t a0, uint32_t b1, uint32_t b0)
{
  (void) a0;
  (void) b0;
  uint32_t ret1 = a1-b1;
  ret1 >>= 31;
  return ret1;
}
#else
static inline
bit is_smaller_payload(uint32_t a1, uint32_t a0, uint32_t b1, uint32_t b0)
{
  uint32_t ret1 = a1-b1;
  uint32_t ret0 = a0-b0;

  uint32_t eq1 = (ret1&1)|(ret1>>1);
  eq1 -= 1;
  eq1 >>= 31;  // 1 if a1==b1, 0 else

  ret1 >>= 31;
  ret0 >>= 31;

  return ret1|(ret0&eq1);
}
#endif


static inline
void cswap(uint32_t *x,uint32_t *y,bit swap)
{
  uint32_t m;
  uint32_t d;

  m = swap; 
  m = 0 - m;

  d = (*x ^ *y);
  d &= m;
  *x ^= d;
  *y ^= d;
}


static inline
void cswap_u16(uint16_t *x,uint16_t *y,bit swap)
{
  uint16_t m;
  uint16_t d;

  m = swap; 
  m = 0 - m;

  d = (*x ^ *y);
  d &= m;
  *x ^= d;
  *y ^= d;
}

static inline
void cswap_mask(uint32_t *x,uint32_t *y,uint32_t m)
{
  uint32_t d = (*x ^ *y)&m;
  *x ^= d;
  *y ^= d;
}



/* output x = min(input x,input y) */
/* output y = max(input x,input y) */

static void minmax(uint32_t *x, uint32_t *y)
{
  bit m;

  m = is_smaller(*y, *x);
  cswap(x, y, m);
}

static void minmax_payload(uint32_t *x, uint32_t *y, uint32_t *xp, uint32_t *yp)
{
  bit m;

  m = is_smaller_payload(*y, *yp, *x, *xp);
  cswap(x, y, m);
  cswap(xp, yp, m);
}


/* merge first half of x[0],x[step],...,x[(2*n-1)*step] with second half */
/* requires n to be a power of 2 */

static void merge(int n,uint32_t x[],int step)
{
  int i;
  if (n == 1)
    minmax(&x[0],&x[step]);
  else {
    merge(n / 2,x,step * 2);
    merge(n / 2,x + step,step * 2);
    for (i = 1;i < 2*n-1;i += 2)
      minmax(&x[i * step],&x[(i + 1) * step]);
  }
}

static void merge_payload(int n,uint32_t x[],uint32_t pl[],int step)
{
  int i;
  if (n == 1)
    minmax_payload(&x[0],&x[step],&pl[0],&pl[step]);
  else {
    merge_payload(n / 2,x,pl,step * 2);
    merge_payload(n / 2,x + step,pl + step,step * 2);
    for (i = 1;i < 2*n-1;i += 2)
      minmax_payload(&x[i * step],&x[(i + 1) * step],&pl[i * step],&pl[(i + 1) * step]);
  }
}


/* sort x[0],x[1],...,x[n-1] in place */
/* requires n to be a power of 2 */

void sort_32b(int n, uint32_t x[])
{
  if (n <= 1) return;
  sort_32b(n/2,x);
  sort_32b(n/2,x + n/2);
  merge(n/2,x,1);
}

void sort_with_payload(int n, uint32_t x[], uint32_t pl[])
{
  if (n <= 1) return;
  sort_with_payload(n/2,x,pl);
  sort_with_payload(n/2,x + n/2,pl + n/2);
  merge_payload(n/2,x,pl,1);
}


/////////////

static void minmax_payload_2(uint16_t *x, uint32_t *pl, int w_p, int i0, int i1)
{
  bit m;

  m = is_smaller(x[i1], x[i0]);
  cswap_u16(&x[i0], &x[i1], m);

  uint32_t m32 = m; m32 = -m32;
  uint32_t *pl0 = pl+w_p*i0;
  uint32_t *pl1 = pl+w_p*i1;
  for(int i=0;i<w_p;i++) cswap_mask( pl0+i, pl1+i, m32);
}

static void merge_payload_2(int n,int len, uint16_t *x,uint32_t *pl,int w_p,int off,int step)
{
  int i;
  if (n == 1) {
    if(off + step < len)
      minmax_payload_2(x,pl, w_p, off, off+step);
  } else {
    merge_payload_2(n/2, len, x , pl, w_p, off,      step*2);
    merge_payload_2(n/2, len, x , pl, w_p, off+step, step*2);
    for (i = 1;i < 2*n-1 && off + (i+1)*step < len;i += 2)
      minmax_payload_2(x, pl, w_p, off + i*step, off + (i+1)*step );
  }
}


void sort_with_payload_2(int n, int len, uint16_t x[] , uint32_t pl[] , int w_p , int off)
{
  if (n <= 1) return;
  sort_with_payload_2(n/2,len,x,pl,w_p,off);
  sort_with_payload_2(n/2,len,x,pl,w_p,off + n/2);
  merge_payload_2(n/2,len,x,pl,w_p,off,1);
}

