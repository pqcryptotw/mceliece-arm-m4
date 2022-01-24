#ifndef int32_sort_h
#define int32_sort_h

//#define int32_sort CRYPTO_NAMESPACE(int32_sort)

#include <stdint.h>


#define int32_MINMAX(a,b) \
do { \
  int32_t ab = b ^ a; \
  int32_t c = b - a; \
  c ^= ab & (c ^ b); \
  c >>= 31; \
  c &= ab; \
  a ^= c; \
  b ^= c; \
} while(0)

static inline void int32_sort(int32_t *x,long long n)
{
  long long top,p,q,r,i;

  if (n < 2) return;
  top = 1;
  while (top < n - top) top += top;

  for (p = top;p > 0;p >>= 1) {
    for (i = 0;i < n - p;++i)
      if (!(i & p))
        int32_MINMAX(x[i],x[i+p]);
    i = 0;
    for (q = top;q > p;q >>= 1) {
      for (;i < n - q;++i) {
        if (!(i & p)) {
          int32_t a = x[i + p];
          for (r = q;r > p;r >>= 1)
            int32_MINMAX(a,x[i+r]);
          x[i + p] = a;
        }
      }
    }
  }
}



// XXX: use conditional execution

#define uint32_MINMAX(a,b) \
do { \
  uint32_t c = ((uint64_t)b - (uint64_t)a)>>32; \
  c &= a ^ b; \
  a ^= c; \
  b ^= c; \
} while(0)

static inline void uint32_sort(uint32_t *x,long long n)
{
  long long top,p,q,r,i;

  if (n < 2) return;
  top = 1;
  while (top < n - top) top += top;

  for (p = top;p > 0;p >>= 1) {
    for (i = 0;i < n - p;++i)
      if (!(i & p))
        uint32_MINMAX(x[i],x[i+p]);
    i = 0;
    for (q = top;q > p;q >>= 1) {
      for (;i < n - q;++i) {
        if (!(i & p)) {
          uint32_t a = x[i + p];
          for (r = q;r > p;r >>= 1)
            uint32_MINMAX(a,x[i+r]);
          x[i + p] = a;
        }
      }
    }
  }
}


// XXX: use conditional execution

#define uint32_MINMAX_payload(a,b,pa,pb) \
do { \
  uint32_t c = ((uint64_t)b - (uint64_t)a)>>32; \
  uint32_t pc = c; \
  c &= a ^ b; \
  pc &= pa ^ pb; \
  a ^= c; \
  pa ^= pc; \
  b ^= c; \
  pb ^= pc; \
} while(0)

static inline void uint32_sort_payload(uint32_t *x,uint32_t *y, long long n)
{
  long long top,p,q,r,i;

  if (n < 2) return;
  top = 1;
  while (top < n - top) top += top;

  for (p = top;p > 0;p >>= 1) {
    for (i = 0;i < n - p;++i)
      if (!(i & p))
        uint32_MINMAX_payload(x[i],x[i+p],y[i],y[i+p]);
    i = 0;
    for (q = top;q > p;q >>= 1) {
      for (;i < n - q;++i) {
        if (!(i & p)) {
          uint32_t a = x[i + p];
          uint32_t ya = y[i + p];
          for (r = q;r > p;r >>= 1)
            uint32_MINMAX_payload(a,x[i+r],ya,y[i+r]);
          x[i + p] = a;
          y[i + p] = ya;
        }
      }
    }
  }
}



#endif

