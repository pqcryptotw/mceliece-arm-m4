/*
  This file is for Nieddereiter decryption
*/

#ifndef DECRYPT_H
#define DECRYPT_H

int decrypt_vec(unsigned char *, const unsigned char *, const unsigned char *);

int decrypt_n3488_t64(unsigned char *, const unsigned char *, const unsigned char *);



//#define decrypt decrypt_vec
#define decrypt decrypt_n3488_t64

#endif

