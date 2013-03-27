/* 
	image_stego.c

	perform steganography on image file

*/


#include "stego.h"
//#include "bmp.h"
#include "tiff.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>


enum op_mode {
	OP_HIDE,
	OP_UNHIDE
};


void usage_and_exit(char** argv) {
		printf("USAGE: %s -h/-u <input-image-file> [<output-file>] [<data-file>]\n\n"
		     " where \n"
		     "  -h: hide\n"
		     "  -u: unhide\n\n"
		     "  <input-image-file>: BMP or TIFF image file\n"
		     "  <output-file>: Filename where modified image will be stored (hide mode)\n"
		     "                or filename where hidden data will be stored (unhide mode)\n"
		     "  <data-file>: File containing data to hide. Only relevant for hide mode\n", argv[0] );
	exit(0);
}

int main(int argc, char* argv[]) {

	uint8_t opmode = OP_HIDE;

	if(argc < 2)
		usage_and_exit(argv);

	if(!strcmp(argv[1], "-h"))
		opmode = OP_HIDE;
	else if(!strcmp(argv[1], "-u"))
		opmode = OP_UNHIDE;
	else
		usage_and_exit(argv);

	if((opmode == OP_UNHIDE && (argc != 4 && argc != 3)) 
		|| (opmode == OP_HIDE && argc != 5)) {
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
		data_file = argv[3];
	}

	printf("opmode=%d, in_file=%s, out_file=%s, data_file=%s\n", 
		opmode, in_file, out_file, data_file );

	return 0;
}