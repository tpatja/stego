/*

	Program for reading and modifying TIFF files. 

	TODO:
	- add functionality for embedding hidden message in image data

	Author: Teemu Patja <tpatja@cs.helsinki.fi>

  TIFF file format reference used:
  http://partners.adobe.com/public/developer/en/tiff/TIFF6.pdf

*/

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "util.h"
#include "tags.h"

#define FRAME_WIDTH 25

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
	uint8_t photometric_interpretation;
	ifd_entry_t** ifd_entries;
} tiff_info_t;

enum IFD_field_type {
	BYTE = 1,  /* 8-bit uint32_t */
	ASCII = 2, /* 8-bit byte containing 7-bit ASCII code. last byte must be NUL */
	SHORT = 3, /* 2-byte uint32_t */
	LONG = 4,  /* 4-byte uint32_t */
	RATIONAL = 5, /* 64 bits. two LONGs */
};

static uint8_t n_tag_names = sizeof(tag_names) / sizeof(tag_name_t);

typedef struct {
	uint8_t type;
	char* name;
} field_type_name_t;

#define NAME_ENTRY( name ) { name, #name } 

static field_type_name_t field_type_names[] = {
	NAME_ENTRY( BYTE ),
	NAME_ENTRY( ASCII ),
	NAME_ENTRY( SHORT ),
	NAME_ENTRY( LONG ),
	NAME_ENTRY( RATIONAL ),
};

static uint8_t n_field_names = sizeof(field_type_names)/sizeof(field_type_name_t);


void tiff_info_free(tiff_info_t* ti) {
	for(int i=0; i<ti->n_ifd_entries; ++i) {
		free(ti->ifd_entries[i]);
	}
	free(ti->ifd_entries);
	//for(int i=0; i<ti->n_strips; ++i) {
		free(ti->strip_offsets);
		free(ti->strip_bytecounts);
	//}
}

char* name_for_type(uint8_t type) {
	for(int i=0; i<n_field_names; ++i) {
		if(field_type_names[i].type == type)
			return field_type_names[i].name;
	}
	return "";
}

char* name_for_tag(uint16_t tag) {
	for(int i=0; i<n_tag_names; ++i) {
		if(tag_names[i].tag == tag)
			return tag_names[i].name;
	}
	return "";
}

uint8_t tag_by_name(char* name) {
	for(int i=0; i<n_tag_names; ++i) {
		if(strcmp(tag_names[i].name, name) == 0)
			return tag_names[i].tag;
	}
	return -1;
}

void read_at_offset(FILE* fp, 
		int offset, 
		void* data, 
		size_t size, 
		int count) 
{
	int fpos = ftell(fp);
	fseek(fp, offset, SEEK_SET);
	fread(data, size, count, fp);
	fseek(fp, fpos, SEEK_SET);
}


uint8_t type_in_bytes(uint8_t type) {
	switch(type) {
		case BYTE:
		case ASCII:
			return 1;
		case SHORT:
			return 2;
		case LONG:
			return 4;
		case RATIONAL:
			return 8;
	}
	printf("invalid type %d\n", type);
	return 0;
}

int fits_in_header(ifd_entry_t* ie) {
	return (type_in_bytes(ie->field_type) <= 4) && ie->n_values == 1;
}


ifd_entry_t* entry_by_tag(tiff_info_t* ti, uint16_t tag) {
	for(int i=0; i< ti->n_ifd_entries; ++i) {
		if(ti->ifd_entries[i]->tag == tag)
			return ti->ifd_entries[i];
	}
	return 0;
}

/* parse entry dat, update th id needed */
void parse_entry(FILE* fp, tiff_info_t* ti, ifd_entry_t* ie) {
	switch(ie->tag) {
		case ImageWidth:
		{
			ti->width = ie->value; /* value or offset */
			break;
		}
		case ImageLength:
		{
			ti->height = ie->value; /* value or offset */
			break;
		}
		case Compression:
		{
			ti->compressed = ie->value;
			break;
		}
		case NewSubfileType:
		{
			uint32_t mask = 1 << 1;
			printf(" reduced-resolution: %d\n", ie->value & mask); 
			mask = 1 << 2;
			printf(" multi-page: %d\n", ie->value & mask); 
			mask = 1 << 3;
			printf(" transparency-mask: %d\n", ie->value & mask); 
			break;
		}

		case PhotometricInterpretation:
		{
			ti->photometric_interpretation = ie->value;
			switch(ie->value)
			{
				case 0:
				{
					printf(" WhiteIsZero\n");
					break;
				}
				case 1:
				{
					printf(" BlackIsZero\n");
					break;
				}
				case 2:
				{
					printf(" RGB\n");
					break;
				}
				case 3:
				{
					printf(" Palette color\n");
					break;
				}
				case 4:
				{
					printf(" Transparency mask\n");
					break;
				}
				default: break;
			}
			break;
		}

		case StripByteCounts:
		case StripOffsets:
		{
			printf(" (%s)\n", name_for_tag(ie->tag));
			uint32_t values[ie->n_values];
			read_at_offset(fp, ie->value, &values, sizeof(uint32_t), ie->n_values);
			for(int i=0; i<ie->n_values; ++i) {
				printf(" value %u: %u\n", i, values[i]);
			}
			break;
		}

		case SamplesPerPixel:
			ti->samples_per_pixel = ie->value;
		case BitsPerSample:
		case RowsPerStrip:
		{
			printf(" (%s)\n", name_for_tag(ie->tag));
			if(fits_in_header(ie)) {
				printf(" %d\n", ie->value);
			}
			else {
				uint16_t values[ie->n_values];
				read_at_offset(fp, ie->value, &values, sizeof(uint16_t), ie->n_values);
				for(int i=0; i<ie->n_values; ++i) {
					printf(" value %u: %d\n", i, values[i]);
				}
			}
			if(ie->tag == RowsPerStrip)
				ti->n_rows_per_strip = ie->value;

			break;
		}

		default: break;
	}
}

/* show IFD values with ASCII type */
void show_ascii_data(FILE* fp, tiff_info_t* ti) {
	int fpos = ftell(fp);
	for(int i=0; i< ti->n_ifd_entries; ++i) {
		ifd_entry_t * entry = ti->ifd_entries[i];
		if (entry->field_type == ASCII) {
			fseek(fp, entry->value, SEEK_SET);
			char data[entry->n_values];
			fread(data, sizeof(data), 1, fp);
			printf("         (tag %s (0x%x)): %s\n",
				 name_for_tag(entry->tag), entry->tag, data);
		}
	}
	fseek(fp, fpos, SEEK_SET);
}

char* compression_type(int ctype) {
	switch(ctype)
	{
		case 1:
			return "uncompressed";
		case 6:
			return "JPEG";
		default:
			return "compressed (other)";
	}
}

void show_tiff_info(FILE* fp, tiff_info_t* ti) {
	printf("endianness: %s\n", ti->endian == 0 ? "little (MSB)" : "big (LSB)");
	printf("compressed: %d (%s)\n", ti->compressed, compression_type(ti->compressed));
	printf("dimensions: %dx%d pixels\n", ti->width, ti->height);
	printf("strips: %d\n", ti->n_strips);
	printf("fields with ASCII value:\n");
	show_ascii_data(fp, ti);
}

/* mallocs a copy of pixel data in image. assumes chunked RGB format */
uint8_t* copy_pixel_data(FILE* fp, tiff_info_t* ti) {

	uint32_t size=0;
	for(int i=0; i<ti->n_strips; ++i) {
		size += ti->strip_bytecounts[i];
	}
	printf("total image size: %d\n", size);
	printf("width: %d,height: %d, w*h=%d, w*h*samples_per_pixel=%d\n", 
		ti->width, ti->height, ti->width*ti->height, 
		ti->width*ti->height*ti->samples_per_pixel);

	uint8_t* ret = malloc( size );
	uint8_t* ptr = ret;

	for(int i=0; i<ti->n_strips; ++i) {
		fseek(fp, ti->strip_offsets[i], SEEK_SET);
		fread(ptr, 1, ti->strip_bytecounts[i], fp);

		if(i<(ti->n_strips -1))
			ptr += ti->strip_bytecounts[i];
	}
	return ret;
}

/* adds a red frame around image (FRAME_WIDTH pixels thick) */
void modify_pixel_data(tiff_info_t* ti, uint8_t* pixel_data) {
	
	for(uint16_t x=0; x<ti->width; ++x) {
		for(uint16_t y=0; y<ti->height; ++y) {
			uint32_t idx = (y*ti->width+x)*ti->samples_per_pixel;

			/* draw frame */
			if(x % ti->width < FRAME_WIDTH 
				|| ti->width - (x % ti->width) < FRAME_WIDTH
				||y % ti->height < FRAME_WIDTH 
				|| ti->height - (y % ti->height) < FRAME_WIDTH ) {
					pixel_data[idx] = 0xff;
					pixel_data[idx+1] = 0x00;
					pixel_data[idx+2] = 0x00;
					if(ti->samples_per_pixel>3)
						pixel_data[idx+3] = 0xff;
			}

		}
	}
}


/* creates a copy of file fp to given filename and writes
   given pixel data to it */
void put_pixel_data(FILE* fp, 
	char* outfile, 
	tiff_info_t* ti, 
	uint8_t* pixel_data) {

	if(cp_file(fp, outfile) != 0)
		exit(1);

	/* write pixel data */
	FILE* out_file = fopen(outfile, "rb+");
	uint32_t pos = 0;
	for(int i=0; i<ti->n_strips; ++i) {
		fseek(out_file, ti->strip_offsets[i], SEEK_SET);
		fwrite(&(pixel_data[pos]), 1, ti->strip_bytecounts[i], out_file);
		pos += ti->strip_bytecounts[i];
	}	
	fclose(out_file);
}

/* read TIFF file from given file.
	 return malloced and filled tiff into structure 
*/
tiff_info_t* read_tiff(FILE* fp) {

	tiff_info_t* ti = malloc(sizeof(tiff_info_t));
	memset(ti, 0x0, sizeof(tiff_info_t));

	/* endianness */
	char data[10];
	fread(data, 2, 1, fp);
	ti->endian = (data[0] == 0x49) ? 0 : 1;
	memset((void*)&data[0], 0x0, sizeof(data));
	
	/* TIFF identifier */
	fread(data, 2, 1, fp);
	int is_tiff = data[ti->endian ? 1 : 0] == 42;
	//printf("is_tiff=%d)\n", is_tiff);
	if(!is_tiff) {
		printf("Invalid TIFF file\n");
		exit(-2);
	}
	
	/* IFD addr */
	int ifd_offset;
	fread(&ifd_offset, sizeof(int), 1, fp);
	
	fseek(fp, ifd_offset, SEEK_SET);

	short n_ifd_entries;
	fread(&n_ifd_entries, sizeof(short), 1, fp);
	//printf("n_ifd_entries=%d\n", n_ifd_entries);

	ti->n_ifd_entries = n_ifd_entries;

	ti->ifd_entries = malloc(sizeof(ifd_entry_t*) * n_ifd_entries);
	
	for(int i=0; i<n_ifd_entries; ++i) {

		ifd_entry_t* ie = malloc(sizeof(ifd_entry_t));
		memset(ie, 0, sizeof(ifd_entry_t));
		fread(ie, sizeof(ifd_entry_t), 1, fp);
		printf("  entry %d: %d (%s), %d (%s), %d, %d\n", 
			i, ie->tag, name_for_tag(ie->tag), ie->field_type, name_for_type(ie->field_type), ie->n_values, ie->value);

		ti->ifd_entries[i] = ie;
		parse_entry(fp, ti, ie);
	}

	// calculate strips-per-image
	if(ti->compressed == 1) { /* uncompressed */
		printf("ti->height=%d, ti->n_rows_per_strip=%d\n", ti->height, ti->n_rows_per_strip);
		ti->n_strips = 
			floor((ti->height + ti->n_rows_per_strip - 1)/ti->n_rows_per_strip);

		ti->strip_bytecounts = malloc(sizeof(uint32_t)*ti->n_strips);
		ti->strip_offsets = malloc(sizeof(uint32_t)*ti->n_strips);
		
		ifd_entry_t* ie = entry_by_tag(ti, StripByteCounts);
		uint32_t values[ie->n_values];
		read_at_offset(fp, ie->value, &values, sizeof(uint32_t), ie->n_values);
		for(int i=0; i<ie->n_values; ++i) {
			ti->strip_bytecounts[i] = values[i];
		}
		
		ie = entry_by_tag(ti, StripOffsets);
		read_at_offset(fp, ie->value, &values, sizeof(uint32_t), ie->n_values);
		for(int i=0; i<ie->n_values; ++i) {
			ti->strip_offsets[i] = values[i];
		}
	}
	return ti;
}

int main(int argc, char** argv)
{
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
	uint8_t* pixel_data = copy_pixel_data(fp, ti);
	printf("adding frame\n");
	modify_pixel_data(ti, pixel_data);
	printf("saving to %s\n", outfile);
	put_pixel_data(fp, outfile, ti, pixel_data);
	printf("done\n");

	free(pixel_data);
	fclose(fp);
	tiff_info_free(ti);
	free(ti);

	return 0;
}
