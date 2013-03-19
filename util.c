/* util.c */

#include <stdio.h>

uint8_t cp_file(FILE* src, char* dest) {
	/* copy file */
	FILE* out_file;
	if((out_file = fopen(dest, "wb")) == 0) {
		fprintf(stderr, "failed to open %s for writing\n", dest);
		return 1;
	}
	int num_read, num_written;
	char buffer[100];

	fseek(src, 0, SEEK_SET);
	while(feof(src)==0){	
		if((num_read = fread(buffer,1,100,src))!=100){
			if(ferror(src)!=0){
				fprintf(stderr,"read file error.\n");
				return 1;
			}
			else if(feof(src)!=0);
		}
		if((num_written = fwrite(buffer, 1, num_read, out_file)) != num_read){
			fprintf(stderr,"write file error.\n");
			return 1;
		}
	}
	fclose(out_file);
	return 0;
}

void hexdump(uint8_t* data, size_t len) {
	for(int i=0; i<len; ++i) {
		printf("0x%x ", data[i] );
	}
	printf("\n");
}

uint8_t cmp(uint8_t* data1, uint8_t* data2, size_t len) {
	for(int i=0; i<len; ++i) {
		if(data1[i] != data2[i])
			return 0;
	}
	return 1;
}