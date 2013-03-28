/* tiff_test.c */

#include "tiff.h"
#include "util.h"

int main(int argc, char** argv)
{

	/*
	static char* filenames[]= {
		"gen.py",
		"ei_oo",
		"foo.tif"
	};
	int n_filenames = 3;

	for(int i=0; i<n_filenames; ++i)
		printf("%s exists: %d\n", filenames[i], file_exists(filenames[i]) );
	exit(0);
*/
	if(argc != 3) {
		printf(" Modifies TIFF files\n");
		printf("USAGE: %s <TIFF file> <output file>\n", argv[0]);
		return -1;
	}
	char* infile = argv[1];
	char* outfile = argv[2];

	FILE* fp = fopen(infile, "rb");

	tiff_info_t* ti;
	ti = read_tiff(fp);
	show_tiff_info(fp, ti);

	if(ti->compressed != 1 
		|| ti->photometric_interpretation != 2 
		|| (ti->samples_per_pixel != 3 && ti->samples_per_pixel != 4) ) {
		printf("Data modification possible only with non-compressed "
			     "RGB and RGBA files\n");
		exit(0);
	}

	printf("copying\n");
	uint8_t* pixel_data = copy_pixel_data_tiff(fp, ti);
	printf("adding frame\n");
	add_red_frame_tiff(ti, pixel_data);
	printf("saving to %s\n", outfile);
	put_pixel_data_tiff(fp, outfile, ti, pixel_data);
	printf("done\n");

	free(pixel_data);
	fclose(fp);
	tiff_info_free(ti);

	return 0;
}
