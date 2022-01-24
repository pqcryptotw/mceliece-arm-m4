
#ifndef _SORT_32B_H
#define _SORT_32B_H

#include <stdint.h>

void sort_32b(int, uint32_t []);
void sort_with_payload(int len, uint32_t to_sort[] , uint32_t payload []);

void sort_with_payload_2(int len_2_power, int len , uint16_t to_sort[] , uint32_t payload_mat [] , int mat_width, int off);


#endif

