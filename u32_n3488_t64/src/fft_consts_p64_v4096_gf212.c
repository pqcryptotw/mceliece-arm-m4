

#include "fft_consts_p64_v4096_gf212.h"



const uint32_t consts_u32[ 126 ][ GFBITS ] =
{
#include "consts_u32.data"
};

const uint32_t power_64_u32[ 128 ][ GFBITS ] =
{
#include "powers_64_u32.data"
};

const uint32_t s_u32[5][2][GFBITS] =
{
#include "scalars_u32.data"
};


const uint32_t s_2x_u32[5][4][GFBITS] =
{
#include "scalars_2x_u32.data"
};

