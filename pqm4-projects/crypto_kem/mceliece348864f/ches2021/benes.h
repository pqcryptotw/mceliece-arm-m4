/*
  This file is for Benes network related functions
*/

#ifndef BENES_H
#define BENES_H

#include "gf.h"

void benes_4096(uint32_t *data, const unsigned char *control_bits, int rev);

static inline
void benes(uint8_t *data, const unsigned char *control_bits, int rev) { benes_4096((uint32_t*)data,control_bits,rev); }

#endif

