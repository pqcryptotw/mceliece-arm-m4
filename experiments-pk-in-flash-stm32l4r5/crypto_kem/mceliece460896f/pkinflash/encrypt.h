/*
  This file is for Niederreiter encryption
*/

#ifndef ENCRYPT_H
#define ENCRYPT_H

void gen_e(unsigned char *e);

void encrypt(unsigned char *, const unsigned char *, const unsigned char *);

#endif

