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


/* check if the padding bits of pk are all zero */
static int check_pk_padding(const unsigned char * pk)
{
	unsigned char b;  
	int i, ret;

	b = 0;
	for (i = 0; i < PK_NROWS; i++)
		b |= pk[i*PK_ROW_BYTES + PK_ROW_BYTES-1];

	b >>= (PK_NCOLS % 8);
	b -= 1;
	b >>= 7;
	ret = b;

	return ret-1;
}


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

	int padding_ok = check_pk_padding(pk);

        // clear outputs (set to all 0's) if padding bits are not all zero
	unsigned char mask = padding_ok;
	mask ^= 0xFF;

	for (int i = 0; i < SYND_BYTES + 32; i++)
		c[i] &= mask;

	for (int i = 0; i < 32; i++)
		key[i] &= mask;

	return padding_ok;
}


/* check if the padding bits of c are all zero */
static int check_c_padding(const unsigned char * c)
{
	unsigned char b;
	int ret;

	b = c[ SYND_BYTES-1 ] >> (PK_NROWS % 8);
	b -= 1;
	b >>= 7;
	ret = b;

	return ret-1;
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

	int padding_ok = check_c_padding(c);
	// clear outputs (set to all 1's) if padding bits are not all zero

	unsigned char mask = padding_ok;

	for (i = 0; i < 32; i++)
		key[i] |= mask;

	return padding_ok;
}


int crypto_kem_keypair
(
       unsigned char *pk,
       unsigned char *sk 
)
{
	unsigned char r[ SYS_N/8 + (1 << GFBITS)*sizeof(uint32_t) + SYS_T*2 + 32 ];
	// SYS_N/8=4608/8=576; (1<<GFBITS)*4=32768; SYS_T=96
	uint8_t * r_perm      = r + SYS_N/8;
	uint32_t* r_perm_u32  = (uint32_t*)r_perm;
	uint8_t * r_poly_f    = r + sizeof(r)-32-SYS_T*2;
        uint8_t * r_next_seed = r + sizeof(r)-32;

	// sk: [ sk_seed:32 + pivot:8 + irr_poly:2*SYS_t + COND_BYTES + SYS_N/8 ]  // COND_BYTES:12800 , SYS_N/8:576
	uint8_t * sk_pivot        = sk + 32;
	uint8_t * sk_irr_poly     = sk + 40;
	uint8_t * sk_cond_bytes   = sk + 40 + SYS_T*2;
	uint8_t * sk_error        = sk + 40 + SYS_T*2 + COND_BYTES;  // secret for decrypt fails.

	unsigned char seed[ 33 ] = {64};
	randombytes(seed + 1, 32);

	while (1)
	{
		memcpy(sk, seed+1, 32);

		// expand and update the seed
		shake256(r, sizeof(r), seed, 33);
		memcpy(seed+1, r_next_seed, 32);

		// generate the irreducible polynomial (Goppa polynomial)
		{
		gf f[ SYS_T ]; // element in GF(2^mt)
		gf irr[ SYS_T ]; // Goppa polynomial
		for (int i = 0; i < SYS_T; i++) { f[i] = load_gf( r_poly_f + i*2); }
		if (genpoly_gen(irr, f)) continue;
		for (int i = 0; i < SYS_T; i++) store_gf( sk_irr_poly + i*2, irr[i]);
		}

		// generate the permutation
		for (int i = 0; i < (1 << GFBITS); i++) { r_perm_u32[i] = load4( r_perm+i*4 ); }

		{
		uint64_t pivot;
		// generate public key
		if (pk_gen(pk, sk_irr_poly, r_perm_u32, &pivot )) continue;
		// store positions of the 32 pivots
		store8(sk_pivot, pivot );
		}

		// generate controlbits from permuation(pi)
		// perm from the output of pk_gen() is in 16-bit format
		controlbitsfrompermutation(sk_cond_bytes, (int16_t*)r_perm_u32, GFBITS, 1 << GFBITS);

		// last part
		memcpy(sk_error, r, SYS_N/8);  // the decrypt() uses this byte by byte

		break;
	}
	return 0;
}

