#ifndef _FLASHPK_H_
#define _FLASHPK_H_


#include "stdint.h"

uint32_t * flashpk_address(void);

uint32_t flashpk_open_and_erase(int n_4kBpage);

uint32_t flashpk_program_2words(uint32_t address, uint64_t data );

uint32_t flashpk_program_n_2words(uint32_t address, const uint32_t *data , unsigned n );

// for debug only
uint32_t flashpk_program_n_words(uint32_t address, const uint32_t *data , unsigned n );



uint32_t flashpk_close(void);


#endif
