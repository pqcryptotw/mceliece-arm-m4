/*
  This file is for Benes network related functions
*/

#ifndef BENES_H
#define BENES_H

#include "gf.h"

//void benes(vec *, const unsigned char *, int);

void benes_8192(uint32_t *, const unsigned char *, int);

#define benes  benes_8192

#endif

