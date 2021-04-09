#include "api.h"
#include "hal.h"

#include <stdio.h>
#include <stdint.h>
#include <string.h>

// https://stackoverflow.com/a/1489985/1711232
#define PASTER(x, y) x####y
#define EVALUATOR(x, y) PASTER(x, y)
#define NAMESPACE(fun) EVALUATOR(MUPQ_NAMESPACE, fun)

// use different names so we can have empty namespaces
#define MUPQ_CRYPTO_BYTES           NAMESPACE(CRYPTO_BYTES)
#define MUPQ_CRYPTO_PUBLICKEYBYTES  NAMESPACE(CRYPTO_PUBLICKEYBYTES)
#define MUPQ_CRYPTO_SECRETKEYBYTES  NAMESPACE(CRYPTO_SECRETKEYBYTES)
#define MUPQ_CRYPTO_CIPHERTEXTBYTES NAMESPACE(CRYPTO_CIPHERTEXTBYTES)
#define MUPQ_CRYPTO_ALGNAME NAMESPACE(CRYPTO_ALGNAME)

#define MUPQ_crypto_kem_keypair NAMESPACE(crypto_kem_keypair)
#define MUPQ_crypto_kem_enc NAMESPACE(crypto_kem_enc)
#define MUPQ_crypto_kem_dec NAMESPACE(crypto_kem_dec)

static void printcycles(const char *s, unsigned long long c)
{
  char outs[32];
  hal_send_str(s);
  snprintf(outs,sizeof(outs),"%llu\n",c);
  hal_send_str(outs);
}

#define _USE_FLASH_

#if defined(_USE_FLASH_)
#include "flashwrite.h"
#endif

#define NUM_KEYGEN  100

int main(void)
{
  unsigned char key_a[MUPQ_CRYPTO_BYTES], key_b[MUPQ_CRYPTO_BYTES];

  unsigned char sk[MUPQ_CRYPTO_SECRETKEYBYTES];
#if defined(_USE_FLASH_)
//  unsigned char pk[MUPQ_CRYPTO_PUBLICKEYBYTES];
  unsigned char *pk = flash_address();
#endif
  unsigned char ct[MUPQ_CRYPTO_CIPHERTEXTBYTES];
  unsigned long long t0, t1;

  //hal_setup(CLOCK_BENCHMARK);
  hal_setup(CLOCK_FAST);

  hal_send_str("==========================");

  // Key-pair generation
unsigned long long accu0 = 0;
for(int i=0;i<NUM_KEYGEN;i++) {
  t0 = hal_get_time();
  int r = MUPQ_crypto_kem_keypair(pk, sk);
  t1 = hal_get_time();
  accu0 += t1-t0;

  // Encapsulation
  MUPQ_crypto_kem_enc(ct, key_a, pk);

  // Decapsulation
  MUPQ_crypto_kem_dec(key_b, ct, sk);

  if (0==memcmp(key_a, key_b, MUPQ_CRYPTO_BYTES)) {
    printcycles("OKKEY,", t1-t0);
  } else {
    printcycles("ERRKEY,", t1-t0);
  }
  printcycles("keypair cycles of this iter:", t1-t0);
  printcycles("keypair avg  cycles:", accu0/(i+1));
}

  printcycles("keypair avg  cycles:", accu0/(NUM_KEYGEN));

  hal_send_str("#");
  while(1);
  return 0;
}
