/*

	Program for reading and modifying TIFF files. 

	TOFO:
	- add functionality for embedding hidden message in image data
	- add functionality for programmatically adding a black frame

	Author: Teemu Patja <tpatja@cs.helsinki.fi>

  TIFF file format reference used:
  http://partners.adobe.com/public/developer/en/tiff/TIFF6.pdf

*/

#include <stdio.h>

typedef struct ifd_entry {
	short tag;
	short field_type;
	int n_values;
	int value; /* value or offset */
} ifd_entry_t;

typedef struct tiff_info {
	int endian; /* 0-> little, 1-> big*/
	int compressed; /* non-zero if compressed */
	int width;
	int height; 
	int n_ifd_entries;
	ifd_entry_t* ifd_entries;
} tiff_info_t;

enum IFD_field_type {
	ifd_BYTE = 1,  /* 8-bit unsigned int */
	ifd_ASCII = 2, /* 8-bit byte containing 7-bit ASCII code. last byte must be NUL */
	ifd_SHORT = 3, /* 2-byte unsigned int */
	ifd_LONG = 4,  /* 4-byte unsigned int */
	ifd_RATIONAL = 5, /* 64 bits. two LONGs */
};

enum Tiff_Tag {
	tag_NewSubfileType = 254,
	tag_SubfileType = 255,
	tag_ImageWidth = 256,
	tag_ImageLength = 257,
	tag_BitsPerSample = 258,
	tag_Compression = 259,
	tag_PhotometricInterpretation = 262,
	tag_Thresholding = 263,
	tag_CellWidth = 264,
	tag_CellLength = 265,
	tag_FillOrder = 266,
	tag_DocumentName = 269,
	tag_ImageDescription = 270,
	tag_Make = 271,
	tag_Model = 272,
	tag_DateTime = 306
};

#define BUF_SIZE 10

void tiff_info_free(tiff_info_t* th) {
	for(int i=0; i<th->n_ifd_entries; ++i) {
		free(&(th->ifd_entries[i]));
	}
}

char* name_for_tag(int tag) {
	/* TODO: probably possible with some preprocessor magic */
	switch(tag) {
		case tag_NewSubfileType:
			return "NewSubfileType";
		case tag_SubfileType:
			return "SubfileType";
		case tag_ImageWidth:
			return "ImageWidth";
		case tag_ImageLength:
			return "ImageLength";
		case tag_BitsPerSample:
			return "BitsPerSample";
		case tag_Compression:
			return "Compression";
		case tag_DocumentName:
			return "DocumentName";
		case tag_ImageDescription:
			return "ImageDescription";
		case tag_Make:
			return "Make";
		case tag_Model:
			return "Model";
		case tag_DateTime:
			return "DateTime";
		default: return "";
	}
}

void read_at_offset(FILE* fp, int offset, void* data, size_t size) {
	int fpos = ftell(fp);
	fseek(fp, offset, 0);
	fread(data, size, 1, fp);
	fseek(fp, fpos, 0);
}

/* parse entry data into th */
void parse_entry(FILE* fp, tiff_info_t* th, ifd_entry_t* ie) {
	switch(ie->tag) {
		case tag_ImageWidth:
		{
			th->width = ie->value; /* value or offset */
			break;
		}
		case tag_ImageLength:
		{
			th->height = ie->value; /* value or offset */
			break;
		}
		case tag_Compression:
		{
			th->compressed = ie->value;
		}
		default: break;
	}
}



/* show IFD values with ASCII type */
void show_ascii_data(FILE* fp, tiff_info_t* th) {
	int fpos = ftell(fp);
	for(int i=0; i< th->n_ifd_entries; ++i) {
		ifd_entry_t * entry = &(th->ifd_entries[i]);
		if (entry->field_type == ifd_ASCII) {
			fseek(fp, entry->value, /* value or offset */ 0);
			char data[entry->n_values];
			fread(data, sizeof(data), 1, fp);
			printf("         (tag %s (0x%x)): %s\n",
				 name_for_tag(entry->tag), entry->tag, data);
		}
	}
	fseek(fp, fpos, 0);
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

void show_tiff_info(FILE* fp, tiff_info_t* th) {
	printf("endianness: %s\n", th->endian == 0 ? "little" : "big");
	printf("compressed: %d (%s)\n", th->compressed, compression_type(th->compressed));
	printf("dimensions: %dx%d pixels\n", th->width, th->height);
	printf("fields with ASCII value:\n");
	show_ascii_data(fp, th);
}


int main(int argc, char** argv)
{
	if(argc != 2) {
		printf("USAGE: %s <TIFF file>\n", argv[0]);
		return -1;
	}
	FILE* fp = fopen(argv[1], "rb");

	//printf("int->%d, short->%d, char->%d\n",sizeof(int), sizeof(short), sizeof(char));

	tiff_info_t th;

	/* endianness */
	char data[BUF_SIZE];
	fread(data, 2, 1, fp);
	th.endian = (data[0] == 0x49) ? 0 : 1;
	memset((void*)&data[0], 0x0, sizeof(data));
	
	/* TIFF identifier */
	fread(data, 2, 1, fp);
	int is_tiff = data[th.endian ? 1 : 0] == 42;
	//printf("is_tiff=%d)\n", is_tiff);
	if(!is_tiff) {
		printf("%s is not a valid TIFF file\n", argv[1]);
		return -2;
	}
	
	/* IFD addr */

	int ifd_offset;
	int read = fread(&ifd_offset, sizeof(int), 1, fp);
	//printf("read=%d, ifd_offset=0x%x (%d))\n", read, ifd_offset);

	int seek = fseek(fp, ifd_offset, 0);
	//printf("seek=%d\n", seek);

	short n_ifd_entries;
	fread(&n_ifd_entries, sizeof(short), 1, fp);
	//printf("n_ifd_entries=%d\n", n_ifd_entries);

	th.ifd_entries = malloc(sizeof(ifd_entry_t)*n_ifd_entries);
	th.n_ifd_entries = n_ifd_entries;

	for(int i=0; i<n_ifd_entries; ++i) {

		ifd_entry_t* ie = &(th.ifd_entries[i]);
		//printf("sizeof ie=%d\n", sizeof(*ie));

		memset(ie, 0, sizeof(ifd_entry_t));
		fread(ie, sizeof(ifd_entry_t), 1, fp);;
		/*printf("  entry %d: %d (%s), %d, %d, %d\n", 
			i, ie->tag, name_for_tag(ie->tag), ie->field_type, ie->n_values, ie->value);*/

		parse_entry(fp, &th, ie);
	}

	show_tiff_info(fp, &th);

	fclose(fp);

	tiff_info_free(&th);

	return 0;
}
