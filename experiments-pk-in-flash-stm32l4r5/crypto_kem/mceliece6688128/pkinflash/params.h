#ifndef PARAMS_H
#define PARAMS_H

#define GFBITS 13
#define SYS_N 6688
#define SYS_T 128

#define COND_BYTES ((1 << (GFBITS-4))*(2*GFBITS - 1))
#define IRR_BYTES (SYS_T * 2)

#define PK_NROWS (SYS_T*GFBITS) 
#define PK_NCOLS (SYS_N - PK_NROWS)
#define PK_ROW_BYTES ((PK_NCOLS + 7)/8)
#define PK_BYTES (PK_NROWS * PK_ROW_BYTES)

#define SK_BYTES (40 + IRR_BYTES + COND_BYTES + SYS_N/8)
#define SYND_BYTES ((PK_NROWS + 7)/8)

#define GFMASK ((1 << GFBITS) - 1)

#define CIPHERTEXT_BYTES (SYND_BYTES+32)

#endif

