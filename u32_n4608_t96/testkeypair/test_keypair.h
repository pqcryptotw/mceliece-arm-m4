#ifndef _TEST_KEYPAIR_H_
#define _TEST_KEYPAIR_H_

#include "stdint.h"

extern const uint32_t * get_test_pk();

extern const uint32_t * get_test_sk();


const unsigned char * get_test_e();

const unsigned char * get_test_ciphertext();

const unsigned char * get_test_sharedkey();


#endif
