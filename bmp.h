#ifndef BMP_H_
#define BMP_H_

#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>

typedef struct 
{
	uint32_t width;
	uint32_t height;
	uint32_t file_size;
	uint32_t data_offset;
	uint16_t bits_per_pixel;
	uint32_t compression;
	uint32_t data_size;
} bmp_info_t;

bmp_info_t* read_bmp(FILE* fp);

uint8_t* copy_pixel_data_bmp(FILE* fp, bmp_info_t* bi);

void add_red_frame_bmp(bmp_info_t* ti, uint8_t* pixel_data);

void put_pixel_data_bmp(FILE* fp, 
	char* outfile, 
	bmp_info_t* bi, 
	uint8_t* pixel_data);

void bmp_info_free(bmp_info_t* bi);

#endif
