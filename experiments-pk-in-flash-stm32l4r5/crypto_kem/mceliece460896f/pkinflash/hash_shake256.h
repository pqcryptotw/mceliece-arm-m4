
#ifndef _CRYPTO_HASH_H_
#define _CRYPTO_HASH_H_


#if defined(PQM4)

#include "fips202.h"

#elif defined(_LIB_KECCAK_)

#include <libkeccak.a.headers/SimpleFIPS202.h>

#define SHARE256  shake256

#else   // openssl

#include <string.h>
#include <openssl/evp.h>

static inline
int shake256( unsigned char *out,unsigned long long outlen, const unsigned char *in,unsigned long long inlen )
{
  int ok = 1;
  EVP_MD_CTX *x;

  x = EVP_MD_CTX_create();
  if (!x) return -1;

  if (ok) ok = EVP_DigestInit_ex(x,EVP_shake256(),NULL);
  if (ok) ok = EVP_DigestUpdate(x,in,inlen);
  if (ok) ok = EVP_DigestFinalXOF(x,out,outlen);

  EVP_MD_CTX_destroy(x);
  if (!ok) return -1;
  return 0;
}

#endif

static inline
void crypto_hash_32b(unsigned char *output, const unsigned char* input, size_t inlen ) { shake256(output,32,input,inlen); }



#endif  //  _CRYPTO_HASH_H_
