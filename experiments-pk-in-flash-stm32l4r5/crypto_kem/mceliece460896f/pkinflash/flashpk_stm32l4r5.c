
#include "flashpk_stm32l4r5.h"
#include "stdint.h"





// 4 KB
#define FLASH_PAGE_SIZE  0x1000

#define FLASH_PAGE_ST_IDX  128

// 1328KB
#define FLASH_N_PAGES      332
// 512 KB
//#define FLASH_N_PAGES      128
// 1024KB
//#define FLASH_N_PAGES      256


#define FLASH_SIZE    ((FLASH_N_PAGES)*(FLASH_PAGE_SIZE))


#if defined(PQM4)

#include <libopencm3/stm32/flash.h>


// assert( PK_BYTES <= FLASH_SIZE )
__attribute__((__section__(".flash_pk"))) const unsigned char __data0[FLASH_SIZE];


uint32_t * flashpk_address(void) {
	return &__data0[0];
}

uint32_t flashpk_open_and_erase(int n_4kBpage) {
	flash_wait_for_last_operation();
	flash_unlock();
	flash_wait_for_last_operation();

	for(int i=0;i<n_4kBpage;i++) flash_erase_page(i+FLASH_PAGE_ST_IDX);

	flash_wait_for_last_operation();
	return flashpk_address();
}



uint32_t flashpk_program_2words(uint32_t address, uint64_t data )
{
	flash_wait_for_last_operation();
	flash_clear_status_flags();
	flash_wait_for_last_operation();

	flash_program_double_word((&__data0[0])+address,data);
	flash_wait_for_last_operation();

	return 0;
}

uint32_t flashpk_program_n_2words(uint32_t address, const uint32_t *data , unsigned n )
{
	flash_wait_for_last_operation();
	flash_clear_status_flags();
	flash_wait_for_last_operation();

	while(n) {
		uint64_t dd = data[0];
		dd |= (((uint64_t)data[1])<<32);

		flash_program_double_word( (&__data0[0])+address , dd );
		flash_wait_for_last_operation();
		address += 8;
		data += 2;
		n -= 1;
	}

	return 0;
}


uint32_t flashpk_close(void) {
	flash_wait_for_last_operation();
	flash_lock();
	flash_wait_for_last_operation();
	return flashpk_address();
}


#endif  //defined(PQM4)
