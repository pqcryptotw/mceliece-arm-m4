#include "api.h"
#include "hal.h"
#include "sendfn.h"

#include <stdint.h>
#include <string.h>

// https://stackoverflow.com/a/1489985/1711232
#define PASTER(x, y) x##y
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

#define printcycles(S, U) send_unsignedll((S), (U))

#include "flashpk_stm32l4r5.h"

int main(void)
{
  unsigned char *pk = flashpk_address();
  unsigned long long pk_addr = pk;
  //unsigned char pk[MUPQ_CRYPTO_PUBLICKEYBYTES];

  unsigned char sk[MUPQ_CRYPTO_SECRETKEYBYTES];

  unsigned char key_a[MUPQ_CRYPTO_BYTES], key_b[MUPQ_CRYPTO_BYTES];
  unsigned char ct[MUPQ_CRYPTO_CIPHERTEXTBYTES];
  unsigned long long t0, t1;
  int i;

  hal_setup(CLOCK_BENCHMARK);
  //hal_setup(CLOCK_FAST);

  hal_send_str("========== pk in flash speed ================");
  printcycles("pk address:", pk_addr );

  for(i=0;i<MUPQ_ITERATIONS; i++)
  {
    // Key-pair generation
    t0 = hal_get_time();
    MUPQ_crypto_kem_keypair(pk, sk);
    t1 = hal_get_time();
    printcycles("keypair cycles:", t1-t0);

    // Encapsulation
    t0 = hal_get_time();
    MUPQ_crypto_kem_enc(ct, key_a, pk);
    t1 = hal_get_time();
    printcycles("encaps cycles:", t1-t0);

    // Decapsulation
    t0 = hal_get_time();
    MUPQ_crypto_kem_dec(key_b, ct, sk);
    t1 = hal_get_time();
    printcycles("decaps cycles:", t1-t0);

    if (memcmp(key_a, key_b, MUPQ_CRYPTO_BYTES)) {
      hal_send_str("ERROR KEYS\n");
    }
    else {
      hal_send_str("OK KEYS\n");
    }
    hal_send_str("+");
  }

  hal_send_str("#");

  return 0;
}
