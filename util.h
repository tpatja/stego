#ifndef UTIL_H
#define UTIL_H 1

#include <stdio.h>

uint8_t cp_file(FILE* src, char* dest);
void hexdump(uint8_t* data, size_t len);

#endif // UTIL_H
