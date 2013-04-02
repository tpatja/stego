#ifndef TIFF_H_
#define TIFF_H_ 1
/* 
	tiff.h 

	functions for manipulating TIFF files
*/

#include <stdlib.h>
#include <stdio.h>

typedef struct {
	uint16_t tag;
	uint16_t field_type;
	uint32_t n_values;
	uint32_t value; /* value or offset. value if it fits into 4 bytes */
} ifd_entry_t;

typedef struct {
	uint8_t endian; /* 0-> little, 1-> big*/
	uint8_t compressed; /* != 1 if compressed */
	uint32_t width;
	uint32_t height; 
	uint8_t n_ifd_entries;
	uint16_t n_strips; 
	uint16_t n_rows_per_strip;
	uint8_t samples_per_pixel;
	uint32_t* strip_offsets;
	uint32_t* strip_bytecounts;
	uint8_t photometric_interpretation; /* 0->WhiteIsZero, 1->BlackIsZero, 2->RGB */
	ifd_entry_t** ifd_entries;
} tiff_info_t;


tiff_info_t* read_tiff(FILE* fp);

void show_ascii_data_tiff(FILE* fp, tiff_info_t* ti);
void show_tiff_info(FILE* fp, tiff_info_t* ti);

uint8_t* copy_pixel_data_tiff(FILE* fp, tiff_info_t* ti);

void add_red_frame_tiff(tiff_info_t* ti, uint8_t* pixel_data);

void put_pixel_data_tiff(FILE* fp, 
	char* outfile, 
	tiff_info_t* ti, 
	uint8_t* pixel_data);

long get_data_size_tiff(tiff_info_t* ti);

void tiff_info_free(tiff_info_t* ti);

#endif
