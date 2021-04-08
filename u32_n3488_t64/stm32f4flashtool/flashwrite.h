#ifndef _FLASHWRITE_H_
#define _FLASHWRITE_H_


#include "stdint.h"

uint32_t * flash_address(void);

uint32_t flash_open(void);

uint32_t flash_program_32bits(uint32_t address, uint32_t input_data );


uint32_t * flash_stack_address(void);

uint32_t flash_stack_erase();

uint32_t flash_stack_program_32bits(uint32_t address, uint32_t input_data );


uint32_t flash_close(void);


#endif
