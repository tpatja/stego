/* 
	image_stego.c

	perform steganography on image file

*/

#include "stego.h"
#include "bmp.h"
#include "tiff.h"
#include "util.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#define REVERSE_BITS 1


/* returns malloced char* */
char* get_hidden_msg_bmp(bmp_info_t* bi, 
	uint8_t* pixel_data, 
	uint8_t reverse_bits) {

	return reveal_message(pixel_data, bi->data_size, reverse_bits);
}

char* get_hidden_msg_tiff(tiff_info_t* ti, 
	uint8_t* pixel_data, 
	uint8_t reverse_bits) {

	return reveal_message(pixel_data, 
		                    get_data_size_tiff(ti),
		                    reverse_bits);
}


/* assumes files are identical. msg must be nul terminated */
void hide_message_bmp(FILE* fp,
	bmp_info_t* bi, 
	char* out_file,
	uint8_t* pixel_data, 
	char* msg,
	uint8_t reverse_bits) {

	hide_message(msg, pixel_data, bi->data_size, reverse_bits);
	put_pixel_data_bmp(fp, out_file, bi, pixel_data);

}


void hide_message_tiff(FILE* fp,
	tiff_info_t* ti, 
	char* out_file,
	uint8_t* pixel_data, 
	char* msg,
	uint8_t reverse_bits) {

	hide_message(msg, pixel_data, get_data_size_tiff(ti), reverse_bits);
	put_pixel_data_tiff(fp, out_file, ti, pixel_data);
}

enum op_mode {
	OP_HIDE,
	OP_UNHIDE,
	OP_ADDFRAME
};


void usage_and_exit(char** argv) {
		printf("USAGE: %s -h/-u/-f <input-image-file> [<output-file>] [<data-file>]\n\n"
		     " where \n"
		     "  -h: hide\n"
		     "  -u: unhide\n"
		     "  -f: add red frame\n"
		     "  <input-image-file>: BMP or TIFF image file\n"
		     "  <output-file>: Filename where modified image will be stored (hide mode)\n"
		     "                or filename where hidden data will be stored (unhide mode)\n"
		     "  <data-file>: File containing data to hide (only required when hiding/unhiding)\n"
		     , argv[0] );
	exit(0);
}

void handle_tiff(uint8_t opmode, 
	char* in_file, 
	char* out_file, 
	char* data_file) {

		FILE* fp = fopen(in_file, "rb");
		tiff_info_t* ti = read_tiff(fp);
		
		uint8_t grayscale_ok = ((ti->photometric_interpretation == 0 
			|| ti->photometric_interpretation == 1)
				&& ti->samples_per_pixel == 1);

		uint8_t color_ok = (ti->photometric_interpretation == 2 && 
				(ti->samples_per_pixel == 3 || ti->samples_per_pixel == 4));

		//printf("grayscale_ok=%d, color_ok=%d\n", grayscale_ok, color_ok );
		if(!ti || ti->compressed != 1
			|| (!grayscale_ok && !color_ok) ) {
			
			printf("Only non-compressed "
				     "Grayscale, RGB and RGBA TIFF files are supported\n");
		  printf("photometric_interpretation=%d, samples_per_pixel=%d\n", 
		  	ti->photometric_interpretation, ti->samples_per_pixel);
			exit(1);
		}
		uint8_t* pixel_data = copy_pixel_data_tiff(fp, ti);

		switch(opmode) {
			case OP_HIDE:
			{
				if(!strcmp(in_file, out_file)) {
					fprintf(stderr, "input and output files must not be the same file\n");
					exit(1);
				}
					
				FILE* data_f = fopen(data_file, "rb");
				/* ensure data fits into image */
				long data_size = file_size_fp(data_f);
				if(data_size > (get_data_size_tiff(ti)/8)) {
					fprintf(stderr, "data is too large to fit in image\n");
					exit(1);
				}
				char* msg = malloc(data_size);
				memset(msg, 0, data_size);
				fseek(data_f, 0L, SEEK_SET);
				fread(msg, 1, data_size, data_f);
				fclose(data_f);
					
				hide_message_tiff(fp,
					ti, 
					out_file,
					pixel_data, 
					msg,
					REVERSE_BITS);

				break;
			}
		 	case OP_ADDFRAME:
		 	{
				add_red_frame_tiff(ti, pixel_data);
				put_pixel_data_tiff(fp, out_file, ti, pixel_data);
				break;
			}
			case OP_UNHIDE:
			{
				char* msg = get_hidden_msg_tiff(ti, pixel_data, REVERSE_BITS);
				if(out_file) {
					FILE* fout = fopen(out_file, "wb");
					fwrite(msg, 1, strlen(msg), fout);
					fclose(fout);
				}
				else {
					puts(msg);
				}
				free(msg);
				break;
			}
		}
		free(pixel_data);
		tiff_info_free(ti);
		fclose(fp);
}

void handle_bmp(uint8_t opmode, 
	char* in_file, 
	char* out_file, 
	char* data_file) {

	FILE* fp = fopen(in_file, "rb");
	bmp_info_t* bi = read_bmp(fp);
	
	if(!bi || bi->compression != 0 || bi->bits_per_pixel != 24) {
		printf("Only non-compressed "
			     "BGR BMP files are supported\n");
		exit(1);
	}

	uint8_t* pixel_data = copy_pixel_data_bmp(fp, bi);

	switch(opmode) {
		case OP_HIDE:
		{
			if(!strcmp(in_file, out_file)) {
				fprintf(stderr, "input and output files must not be the same file\n");
				exit(1);
			}
			FILE* data_f = fopen(data_file, "rb");
			/* ensure data fits into image */
			long data_size = file_size_fp(data_f);
			//printf("data_size=%ld, bi->data_size/8=%ld\n", data_size, (long)bi->data_size/8);
			if(data_size > (bi->data_size/8)) {
				fprintf(stderr, "data is too large to fit in image\n");
				exit(1);
			}
			char* msg = malloc(data_size);
			memset(msg, 0, data_size);
			fseek(data_f, 0L, SEEK_SET);
			fread(msg, 1, data_size, data_f);
			fclose(data_f);

			hide_message_bmp(fp,
				bi, 
				out_file,
				pixel_data, 
				msg,
				REVERSE_BITS);

			break;
		}
	 	case OP_ADDFRAME:
	 	{
			add_red_frame_bmp(bi, pixel_data);
			put_pixel_data_bmp(fp, out_file, bi, pixel_data);
			break;
		}
		case OP_UNHIDE:
		{
			char* msg = get_hidden_msg_bmp(bi, pixel_data, REVERSE_BITS);
			if(out_file) {
				FILE* fp = fopen(out_file, "wb");
				fwrite(msg, 1, strlen(msg), fp);
				fclose(fp);
			}
			else {
				puts(msg);
			}				
			free(msg);
			break;
		}
	}

	bmp_info_free(bi);
	fclose(fp);
}



int main(int argc, char* argv[]) {

	uint8_t opmode = OP_HIDE;

	if(argc < 2)
		usage_and_exit(argv);

	if(!strcmp(argv[1], "-h"))
		opmode = OP_HIDE;
	else if(!strcmp(argv[1], "-u"))
		opmode = OP_UNHIDE;
	else if(!strcmp(argv[1], "-f"))
		opmode = OP_ADDFRAME;
	else
		usage_and_exit(argv);

	if(  (opmode == OP_UNHIDE && (argc != 4 && argc != 3)) 
		|| (opmode == OP_HIDE && argc != 5)
		|| (opmode == OP_ADDFRAME && argc != 4) ) {
		usage_and_exit(argv);
	}
	char* in_file = argv[2];
	char* out_file = 0;
	char* data_file = 0;

	if(opmode == OP_HIDE) {
		out_file = argv[3];
		data_file = argv[4];
	}
	else if(argc > 3)
	{
		out_file = argv[3];
	}

	//printf("opmode=%d, in_file=%s, out_file=%s, data_file=%s\n", 
	//	opmode, in_file, out_file, data_file );

	uint8_t tiff_mode=0;
	if(strstr(in_file, ".tif"))
		tiff_mode = 1;
	else if(!strstr(in_file, ".bmp")) {
		fprintf(stderr, "input file must have .tif or .bmp suffix\n");
		exit(2);
	}

	if(tiff_mode) {
		handle_tiff(opmode, in_file, out_file, data_file);
	}
	else {
		handle_bmp(opmode, in_file, out_file, data_file);
	}

	return 0;
}
