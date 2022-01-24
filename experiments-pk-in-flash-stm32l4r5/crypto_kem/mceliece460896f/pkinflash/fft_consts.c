

#include "fft_consts.h"


const uint32_t consts_u32[ 256 ][ GFBITS ] =
{
#include "consts_u32.data"
};

const uint32_t power_128_u32[ 256 ][ GFBITS ] =
{
#include "powers_u32.data"
};

const uint32_t s_2x_u32[5][4][GFBITS] =
{
#include "scalars_2x_u32.data"
};

const uint32_t s_4x_u32[6][8][GFBITS] =
{
#include "scalars_4x_u32.data"
};

