//#include "operations.h"

#include "gf.h"
#include "randombytes.h"
#include "params.h"

#include "benes.h"
#include "util.h"
#include "sk_gen.h"
#include "pk_gen.h"
#include "bm.h"
#include "operations.h"
#include "controlbits.h"
#include "encrypt.h"

#include <stdio.h>
#include <assert.h>
#include <string.h>

void pt()
{
	printf("print ");
}

void nl()
{
	printf("\n");
}

void file_head()
{
	printf("load(\"synd.sage\")\n");
	printf("load(\"bm.sage\")\n\n");

	printf("F2 = GF(2)\n");
	printf("P2.<t> = F2[]\n");
	printf("F.<a> = GF(2^13, modulus=t^13 + t^4 + t^3 + t + 1)\n");
	printf("P.<x> = F[]\n");
}

//#define _TEST_BENES_

#define TEST_RUN  30

int main()
{
	unsigned char pk[ PK_NROWS * PK_ROW_BYTES ];
	unsigned char sk[ SK_BYTES ];

	printf("print \"=== enc/dec test ===\"\n");

	unsigned char key[ 32 ];
	unsigned char key_cmp[ 32 ];
	unsigned char c[ SYND_BYTES + 32 ];

#if defined(_TEST_BENES_)
	{
		int i, j, pos, shift;
		unsigned char cond[ COND_BYTES ];
		unsigned char s[ (1 << GFBITS)/8 ];
		uint32_t perm[ 1 << GFBITS ], t;

		for (shift = 0; shift < (1 << GFBITS); shift++)
		{
			printf("shift = %d\n", shift);

			for (i = 0; i < (1 << GFBITS); i++) perm[i] = (i + shift) % (1 << GFBITS);
	
			controlbits(cond, perm);
	
			for (pos = 0; pos < 8; pos++)
			{
				for (i = 0; i < sizeof(s); i++)
				{
					s[i] = 0;
		
					for (j = 0; j < 8; j++)
					{
						t = i*8+j;
						s[i] |= ((t >> pos) & 1) << j;
					}
				}
		
				benes(s, cond, 0);
		
				for (i = 0; i < sizeof(s); i++)
					for (j = 0; j < 8; j++)
						if (((s[i] >> j) & 1) != ((perm[i*8+j] >> pos) & 1))
							{ printf("something wrong %d\n", pos); getchar();}
			}
		}

		printf("test done\n");
	}	
#endif

	int iter = 1, i;

	int kk = 0;
	while (1)
	{
		crypto_kem_keypair(pk, sk);

		for (i = 0; i < 100; i++)
		{
			crypto_kem_enc(c, key, pk);
			crypto_kem_dec(key_cmp, c, sk);

			assert(memcmp(key, key_cmp, sizeof(key)) == 0);
		}	

		fprintf(stderr, "%d-th key pair tested\n", iter++);

		kk++;
		if( kk >= TEST_RUN ) break;
	}

	//

	return 0;
}

