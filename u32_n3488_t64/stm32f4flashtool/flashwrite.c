
#include "flashwrite.h"
#include "stdint.h"


#include "params.h"
#include "run_config.h"



#define FLASH_SECTOR_SIZE (1<<17)
#define FLASH_SIZE    (3*(FLASH_SECTOR_SIZE))
#define FLASH_WRONG_DATA_WRITTEN 0x80


#if defined(STM32F4)

#include <libopencm3/stm32/flash.h>


// assert( PK_BYTES <= FLASH_SIZE )
__attribute__((__section__(".flash_pk"))) const unsigned char __data0[FLASH_SIZE];  // 256+128KB


uint32_t * flash_address(void) {
	return &__data0[0];
}

uint32_t flash_open(void) {
	flash_unlock();
	flash_erase_sector( 9, 2);
	flash_erase_sector(10, 2);
	//flash_erase_sector(11, 2);
	return flash_address();
}


uint32_t * flash_stack_address(void) {
	return &__data0[FLASH_SIZE-FLASH_SECTOR_SIZE];
}

uint32_t flash_stack_erase()
{
	flash_erase_sector(11, 2);
	return flash_stack_address();
}

uint32_t flash_stack_program_32bits(uint32_t address, uint32_t input_data )
{
	// assert( address <= (FLASH_SECTOR_SIZE-4) );
	flash_program_word( &__data0[FLASH_SIZE-FLASH_SECTOR_SIZE+address] , input_data );
	return *((uint32_t*)(&__data0[FLASH_SIZE-FLASH_SECTOR_SIZE+address]));
}


uint32_t flash_program_32bits(uint32_t address, uint32_t input_data )
{
	// assert( address <= (FLASH_SIZE-4) );
	flash_program_word( &__data0[address] , input_data );
	//if(*((uint32_t*)(&__data0[address])) != input_data )
	//	return FLASH_WRONG_DATA_WRITTEN;
	//return 0;
	return *((uint32_t*)(&__data0[address]));
}

uint32_t flash_close(void) {
	flash_lock();
	return flash_address();
}


#endif  //defined(STM32F4)
