#include "api.h"
#include "randombytes.h"
#include "hal.h"

#include <string.h>

#define NTESTS 1

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

const uint8_t canary[8] = {
  0x01, 0x23, 0x45, 0x67, 0x89, 0xAB, 0xCD, 0xEF
};

/* allocate a bit more for all keys and messages and
 * make sure it is not touched by the implementations.
 */
static void write_canary(uint8_t *d) {
  for (size_t i = 0; i < 8; i++) {
    d[i] = canary[i];
  }
}

static int check_canary(const uint8_t *d) {
  for (size_t i = 0; i < 8; i++) {
    if (d[i] != canary[i]) {
      return -1;
    }
  }
  return 0;
}

static int test_keys(unsigned char *sk_a, unsigned char *pk)
{
  unsigned char key_a[MUPQ_CRYPTO_BYTES+16], key_b[MUPQ_CRYPTO_BYTES+16];
  unsigned char sendb[MUPQ_CRYPTO_CIPHERTEXTBYTES+16];

  write_canary(key_a); write_canary(key_a+sizeof(key_a)-8);
  write_canary(key_b); write_canary(key_b+sizeof(key_b)-8);
  write_canary(sendb); write_canary(sendb+sizeof(sendb)-8);


  int i;

  for(i=0; i<NTESTS; i++)
  {
    //Alice generates a public key
    MUPQ_crypto_kem_keypair(pk, sk_a);
    hal_send_str("DONE key pair generation!");

    //Bob derives a secret key and creates a response
    MUPQ_crypto_kem_enc(sendb+8, key_b+8, pk);
    hal_send_str("DONE encapsulation!");

    //Alice uses Bobs response to get her secret key
    MUPQ_crypto_kem_dec(key_a+8, sendb+8, sk_a);
    hal_send_str("DONE decapsulation!");

    if(memcmp(key_a+8, key_b+8, MUPQ_CRYPTO_BYTES))
    {
      hal_send_str("ERROR KEYS\n");
    }
    else if(check_canary(key_a) || check_canary(key_a+sizeof(key_a)-8) ||
            check_canary(key_b) || check_canary(key_b+sizeof(key_b)-8) ||
            check_canary(sendb) || check_canary(sendb+sizeof(sendb)-8) )
    {
      hal_send_str("ERROR canary overwritten\n");
    }
    else
    {
      hal_send_str("OK KEYS\n");
    }
  }

  return 0;
}


static int test_invalid_ciphertext(const unsigned char *sk_a, const unsigned char *pk)
{
  unsigned char key_a[MUPQ_CRYPTO_BYTES], key_b[MUPQ_CRYPTO_BYTES];
  unsigned char sendb[MUPQ_CRYPTO_CIPHERTEXTBYTES];
  int i;
  size_t pos;

  for(i=0; i<NTESTS; i++)
  {
    randombytes((unsigned char *)&pos, sizeof(size_t));

    //Alice generates a public key
    //MUPQ_crypto_kem_keypair(pk, sk_a);

    //Bob derives a secret key and creates a response
    MUPQ_crypto_kem_enc(sendb, key_b, pk);

    // Change ciphertext to random value
    randombytes(sendb, sizeof(sendb));

    //Alice uses Bobs response to get her secret key
    MUPQ_crypto_kem_dec(key_a, sendb, sk_a);

    if(!memcmp(key_a, key_b, MUPQ_CRYPTO_BYTES))
    {
      hal_send_str("ERROR invalid ciphertext\n");
    }
    else
    {
      hal_send_str("OK invalid ciphertext\n");
    }
  }

  return 0;
}

#include "flashpk_stm32l4r5.h"
#include "sendfn.h"

int main(void)
{
  hal_setup(CLOCK_FAST);

  unsigned char sk_a[MUPQ_CRYPTO_SECRETKEYBYTES];
  // marker for automated testing
  hal_send_str("============ pk in flash test ==============");
  unsigned char *pk = flashpk_address();
  send_unsigned("pk address:", pk );
  //unsigned char pk[MUPQ_CRYPTO_PUBLICKEYBYTES];

  test_keys(sk_a,pk);
  test_invalid_ciphertext(sk_a,pk);
  hal_send_str("#");

  return 0;
}