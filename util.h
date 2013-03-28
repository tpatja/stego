#ifndef UTIL_H
#define UTIL_H 1

#include <stdio.h>

uint8_t file_exists(char* filename);
uint8_t read_byte(FILE* fp);

long file_size(char* filename);
long file_size_fp(FILE* filename);

size_t read_at_offset(FILE* fp, 
		int offset, 
		void* data, 
		size_t size, 
		int count);

uint8_t cp_file(FILE* src, char* dest);
void hexdump(uint8_t* data, size_t len);

#endif // UTIL_H
