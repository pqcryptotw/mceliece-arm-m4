

#include "test_keypair.h"

#include "run_config.h"

#if defined(_STM32F4_)

const uint32_t * get_test_pk()
{
  return 0;
}

#else

static
const uint32_t __pk[] =
{
#include "pk.txt"
};

const uint32_t * get_test_pk()
{
  return __pk;
}

#endif


static
const uint32_t __sk[] =
{
#include "sk.txt"
};

const uint32_t * get_test_sk()
{
  return __sk;
}



static
const uint32_t __c[] =
{
0x78234412,0x0c0c7b44,0x35325803,0x795c28f6, 0x15e2c9a2,0x8c394aa7,0xba0e960d,0xde92d693, 
0x80d17ae1,0xca2696a7,0x87dff98b,0x00592d14, 0x9fdf5743,0x126c1440,0x2a65dc56,0x98999dc5, 
0xd27a862d,0xe5f31a67,0x047c43ff,0x2ea31d78, 0xc0ee43d8,0xf86d3113,0xeaa328ce,0x9f2d740f, 
0x240a3ba9,0xf968e44f,0xd0a170e3,0x2d2b9112, 0xf081c10f,0x493f34f1,0x87fb409c,0x0c417ad9, 
0x3bb533f4,0x0540fcc7,0x38d22698,0x86865353, 0x18d76246,0xefb58b10,0x65376cd0,0xafc0f140, 
0x2260b6b5,0x8cb2048e,0x9f6ebc60,0x271706d6, 0xab61b1f5,0xf92f9476,0xe2907702,0xc0026eac, 
0x4661014f,0xc86f6e77,0x201b56ce,0x79a94957, 0x78fdf505,0x9dbcbbf8,0x7d6c1632,0x33b31f45, 
0x117abf17,0x7f65177d,0x69a8be1f,0x996490f0,
};

const uint8_t * get_test_ciphertext()
{
  return (const uint8_t *)__c;
}


static
const uint32_t __k[] =
{
0xb1c97612,0x1a34c923,0xef7c9a91,0x84797f2f, 0x710a8dda,0x7242d4b5,0x193e6841,0x12a568fa
};

const uint8_t * get_test_sharedkey()
{
  return (const uint8_t *)__k;
}
