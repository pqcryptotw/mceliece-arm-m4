/*
  This file is for Nieddereiter decryption
*/

#ifndef DECRYPT_H
#define DECRYPT_H

int decrypt_u64(unsigned char *, const unsigned char *, const unsigned char *);

int decrypt_6688_128(unsigned char *, const unsigned char *, const unsigned char *);

#define decrypt decrypt_6688_128

#endif

