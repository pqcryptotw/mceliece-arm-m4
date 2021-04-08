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

#include "stdlib.h"

int cmpfunc (const void * a, const void * b) {
   return ( *(int*)a - *(int*)b );
}

#include "test_keypair.h"

#define NUM_TEST 100

int main(void)
{
  unsigned char key_a[MUPQ_CRYPTO_BYTES], key_b[MUPQ_CRYPTO_BYTES];

  //unsigned char sk[MUPQ_CRYPTO_SECRETKEYBYTES];
  //const unsigned char * mysk = get_test_sk();
  //memcpy( sk , mysk , MUPQ_CRYPTO_SECRETKEYBYTES );
  const unsigned char * sk = get_test_sk();
  const unsigned char * pk = get_test_pk();

  unsigned char ct[MUPQ_CRYPTO_CIPHERTEXTBYTES];
  unsigned long long t0, t1;

  unsigned enc[NUM_TEST];

  //hal_setup(CLOCK_BENCHMARK);
  hal_setup(CLOCK_FAST);

  hal_send_str("==========================");

  // Encapsulation
  int num_test = NUM_TEST;
  unsigned long long accu = 0;
  for(int i=0;i<num_test;i++) {
    t0 = hal_get_time();
    MUPQ_crypto_kem_enc(ct, key_a, pk);
    t1 = hal_get_time();
    accu += t1-t0;
    enc[i] = t1-t0;
  }
  printcycles("encaps avg(100) cycles: ", accu/num_test);
  qsort(enc, NUM_TEST, sizeof(unsigned), cmpfunc);
  printcycles("quartile: ", enc[NUM_TEST/4] );
  printcycles("median: "  , enc[NUM_TEST/2] );
  printcycles("quartile: ", enc[NUM_TEST-NUM_TEST/4] );

  // Decapsulation
  t0 = hal_get_time();
  MUPQ_crypto_kem_dec(key_b, ct, sk);
  t1 = hal_get_time();
  printcycles("decaps cycles: ", t1-t0);

  if (memcmp(key_a, key_b, MUPQ_CRYPTO_BYTES)) {
    hal_send_str("ERROR KEYS\n");
  }
  else {
    hal_send_str("OK KEYS\n");
  }

  hal_send_str("#");
  while(1);
  return 0;
}
