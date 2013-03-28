#include "bmp.h"
#include "util.h"
#include <string.h>


#define BMP_DATA_OFFSET_ADDRESS 0xa
#define FRAME_WIDTH 15

bmp_info_t* read_bmp(FILE* fp) {
	
	bmp_info_t* bi = malloc(sizeof(bmp_info_t));
	memset(bi, 0x0, sizeof(bmp_info_t));

	uint8_t data[10];
	fread(data, 10, 1, fp);

	if(data[0] != 0x42 ||data[1] != 0x4d) {
		fprintf(stderr, "Not a windows bitmap\n" );
		exit(-2);
	}
	memcpy( &bi->file_size, &data[2], 4);

	fseek(fp, 0xa, SEEK_SET);
	fread( &bi->data_offset, 4, 1, fp );
	fseek(fp, 0x12, SEEK_SET);
	fread( &bi->width, 4, 1, fp);
	fread( &bi->height, 4, 1, fp);
	fseek(fp, 0x1c, SEEK_SET);
	fread( &bi->bits_per_pixel, 2, 1, fp );
	fread( &bi->compression, 4, 1, fp );
	fread( &bi->data_size, 4, 1, fp );

	return bi; 	
}

void bmp_info_free(bmp_info_t* bi)
{
	free(bi);
}

uint8_t* copy_pixel_data_bmp(FILE* fp, bmp_info_t* bi) {
	uint8_t* ret = malloc( bi->data_size );
	fseek(fp, bi->data_offset, SEEK_SET);
	fread(ret, 1, bi->data_size, fp);
	return ret;
}

void add_red_frame_bmp(bmp_info_t* bi, uint8_t* pixel_data) {

	uint8_t samples_per_pixel = bi->bits_per_pixel/8;
	//printf("samples_per_pixel=%d\n", samples_per_pixel);

	for(uint16_t x=0; x<bi->width; ++x) {
		for(uint16_t y=0; y<bi->height; ++y) {
			uint32_t idx = (y*bi->width+x)*samples_per_pixel;
						if(x % bi->width < FRAME_WIDTH 
				|| bi->width - (x % bi->width) < FRAME_WIDTH
				|| y % bi->height < FRAME_WIDTH 
				|| bi->height - (y % bi->height) < FRAME_WIDTH ) {
					pixel_data[idx] = 0x00; /* BGR */
					pixel_data[idx+1] = 0x00;
					pixel_data[idx+2] = 0xff;
					if(samples_per_pixel>3)
						pixel_data[idx+3] = 0xff;
			}
		}
	}
}


void put_pixel_data_bmp(FILE* fp, char* outfile, bmp_info_t* bi, uint8_t* pixel_data) {

	if(cp_file(fp, outfile) != 0)
		exit(1);

	FILE* out_file = fopen(outfile, "rb+");
	fseek(out_file, bi->data_offset, SEEK_SET);
	fwrite(pixel_data, 1, bi->data_size, out_file);
	fclose(out_file);
}

