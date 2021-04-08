#ifndef uint16_qsort_h
#define uint16_qsort_h

//#define uint16_qsort CRYPTO_NAMESPACE(uint16_qsort)
//#include <inttypes.h>
#include <stdint.h>


#define SWAP(a,b) do { uint16_t t=a; a=b; b=t; } while(0)


static inline
int partition(uint16_t arr[], int l, int h) {
    int x = arr[h];
    int i = (l - 1);
    for (int j = l; j <= h - 1; j++) {
        if (arr[j] <= x) {
            i++;
            SWAP(arr[i],arr[j]);
        }
    }
    SWAP(arr[i+1],arr[h]);
    return (i + 1);
}

static inline
void uint16_qsort(uint16_t arr[], int l, int h)
{
    int stack[h - l + 1];

    int top = -1;

    stack[++top] = l;
    stack[++top] = h;

    while (top >= 0) {
        h = stack[top--];
        l = stack[top--];

        int p = partition(arr, l, h);

        if (p - 1 > l) {
            stack[++top] = l;
            stack[++top] = p - 1;
        }

        if (p + 1 < h) {
            stack[++top] = p + 1;
            stack[++top] = h;
        }
    }
}


#endif

