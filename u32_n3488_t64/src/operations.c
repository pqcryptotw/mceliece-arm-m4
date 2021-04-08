#include "operations.h"
#include "run_config.h"
#include "controlbits.h"
#include "randombytes.h"
#include "hash_shake256.h"
#include "encrypt.h"
#include "decrypt.h"
#include "params.h"
#include "sk_gen.h"
#include "pk_gen.h"
#include "util.h"

#include <stdint.h>
#include <string.h>

int crypto_kem_enc(
       unsigned char *c,
       unsigned char *key,
       const unsigned char *pk
)
{
	uint32_t _buffer[ 1 + (SYS_N+31)/32 + (SYND_BYTES+3)/4 + 32/4 ];
	uint32_t _buf_c[(SYND_BYTES+3)/4];

	unsigned char *two_e  = ((uint8_t*)_buffer) + 3;
	unsigned char *e      = ((uint8_t*)_buffer) + 4; // making e 32-bit aligned
	unsigned char *one_ec = ((uint8_t*)_buffer) + 3;

	//

	gen_e(e);

	two_e[0] = 2;
	crypto_hash_32b(c + SYND_BYTES, two_e, 1 + SYS_N/8 );

	encrypt((uint8_t*)_buf_c, pk, e);
	memcpy(c, _buf_c, SYND_BYTES );
	memcpy(one_ec + 1 + SYS_N/8, c, SYND_BYTES + 32);

	one_ec[0] = 1;
	crypto_hash_32b(key, one_ec, 1 + SYS_N/8 + SYND_BYTES + 32 );

	return 0;
}

int crypto_kem_dec(
       unsigned char *key,
       const unsigned char *c,
       const unsigned char *sk
)
{
	int i;

	unsigned char ret_confirm = 0;
	unsigned char ret_decrypt = 0;

	uint16_t m;

	unsigned char conf[32];
	unsigned char two_e[ 1 + SYS_N/8 ] = {2};
	unsigned char *e = two_e + 1;
	unsigned char preimage[ 1 + SYS_N/8 + (SYND_BYTES + 32) ];
	unsigned char *x = preimage;
	const unsigned char *s = sk + 40 + IRR_BYTES + COND_BYTES;

	//

	ret_decrypt = decrypt(e, sk + 40, c);

	crypto_hash_32b(conf, two_e, sizeof(two_e)); 

	for (i = 0; i < 32; i++) ret_confirm |= conf[i] ^ c[SYND_BYTES + i];

	m = ret_decrypt | ret_confirm;
	m -= 1;
	m >>= 8;

	*x++ = (m &    1);
	for (i = 0; i < SYS_N/8;         i++) *x++ = (~m & s[i]) | (m & e[i]);
	for (i = 0; i < SYND_BYTES + 32; i++) *x++ = c[i];

	crypto_hash_32b(key, preimage, sizeof(preimage)); 

	return 0;
}


int crypto_kem_keypair
(
       unsigned char *pk,
       unsigned char *sk 
)
{
	unsigned char r[ SYS_N/8 + (1 << GFBITS)*sizeof(uint32_t) + SYS_T*2 + 32 ]; // 16980 bytes
	// sk: [ 32 + pivot:8 + irr_poly:2*SYS_t + COND_BYTES + SYS_N/8 ]  // COND_BYTES:5888 , SYS_N/8:436

	unsigned char seed[ 33 ] = {64};

	//gf f[ SYS_T ]; // element in GF(2^mt)
	//gf irr[ SYS_T ]; // Goppa polynomial
	//uint32_t perm[ 1 << GFBITS ]; // random permutation
	uint32_t *perm = (uint32_t*)&r[SYS_N/8];

	randombytes(seed + 1, 32);

	while (1)
	{
		// expanding and update the seed
		shake256(r, sizeof(r), seed, 33);
		memcpy(sk, seed+1, 32);
		memcpy(seed+1, &r[ sizeof(r)-32 ], 32);

		// generating irreducible polynomial
		{
		gf f[ SYS_T ]; // element in GF(2^mt)
		gf irr[ SYS_T ]; // Goppa polynomial

		for (int i = 0; i < SYS_T; i++) { f[i] = load_gf( r + sizeof(r)-32-SYS_T*2 + i*2); }
		if (genpoly_gen(irr, f)) continue;
		for (int i = 0; i < SYS_T; i++) store_gf(sk+40 + i*2, irr[i]);
		}

		// generating permutation
		for (int i = 0; i < (1 << GFBITS); i++) { perm[i] = load4((uint8_t*)&perm[i]); }

		// generating public key
		if (pk_gen(pk, sk + 40, perm, (uint64_t*)(sk+40+SYS_T*2) )) continue;

		// storing positions of the 32 pivots
		store8(sk + 32, *(uint64_t*)(sk+40+SYS_T*2) );

		// last part
		memcpy(sk + 40 + SYS_T*2 + COND_BYTES, r, SYS_N/8);  // XXX: use store4()

		// generating controlbits from permuation(pi)
		controlbitsfrompermutation(sk + 40 + SYS_T*2, (int16_t*)perm, GFBITS, 1 << GFBITS); // output are in 16-bits form

		break;
	}
	return 0;
}

