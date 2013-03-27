
/*
	stego.c
	
	hide a message with steganography to a given BMP file

	read a hidden message from given BMP file

	Author: Teemu Patja <tpatja@cs.helsinki.fi>

*/
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "util.h"

uint8_t combine_lsbs(uint8_t* lsbs, uint8_t count) {
	uint8_t ret=0;
	for(int i=0; i < count; ++i) {
		ret |= (lsbs[7-i] << i);
	}
	return ret;
}

static uint8_t mask8[] = {128, 64, 32, 16, 8, 4, 2, 1};

/* extract bits from var */
void bits_from_byte(uint8_t* bits, uint8_t var) {
	//printf("var=%u\n", var);
	for(int i=7; i >= 0; --i) {
		bits[i] = ((var & mask8[i]) != 0) ? 1 : 0;
	}
}

void show_byte_as_bits(uint8_t val) {
	printf("dec: %u\n", val);
	printf("hex: 0x%x\n", val);
	printf("bin: ");
	uint8_t bits[8];
	bits_from_byte(bits, val);
	for(int i=0; i<8; ++i) {
		printf("%u", bits[i]);
	}
	printf("\n");
}

uint8_t set_bit(uint8_t n, uint8_t var) {
	return  var | 1 << (n);
}

uint8_t unset_bit(uint8_t n, uint8_t var) {
	return  var & ~(1 << n);
}

uint8_t get_lsb(uint8_t val) {
	return val & 1;
}

/* set val (0 or 1) to lsb of var */
uint8_t set_lsb(uint8_t var, uint8_t val) {
	if(val)
		return set_bit(0, var);
	else
		return unset_bit(0, var);
}

/* set lsbs of 8 consecutive bytes in data*/
void hide_byte(uint8_t* data, uint8_t value, uint8_t reverse_bits) {
	uint8_t bits[8];
	bits_from_byte(bits, value);
	
	for(int i=0; i<8; ++i) {
		if(reverse_bits)
			data[i] = set_lsb(data[i], bits[7-i]);
		else
			data[i] = set_lsb(data[i], bits[i]);
	}
} 

/* read lsbs of 8 bytes and combine them to a byte */
uint8_t get_byte(uint8_t* data) {
	uint8_t values[8];
	for(int i=0; i<8; ++i) {
		values[i] = get_lsb(data[i]);
	}
	return combine_lsbs(values, 8);
}

uint8_t read_lsb(FILE* fp) {
	return get_lsb(read_byte(fp));	
}

/* read 8 consecutive bytes, pack LSB bits into retval */
uint8_t extract_byte(FILE* fp, uint8_t reverse_bits) {
	uint8_t values[8];
	if(reverse_bits) {
		for(int i=7; i>=0; --i) {
			values[i] = read_byte(fp);
		}
	}
	else {
		for(int i=0; i<8; ++i) {
			values[i] = read_byte(fp);
		}
	}
	return get_byte(values);
}

uint32_t read_pixel_offset(FILE* fp) {
	fseek(fp, 0xa, SEEK_SET);
	uint32_t offset;
	fread(&offset, 4, 1, fp);
	return offset;
}

void show_hidden_msg(char* filename, uint8_t reverse_bits) {
	printf("  show_hidden_msg\n");
	FILE* fp = fopen(filename, "rb");
	uint32_t pixel_offset = read_pixel_offset(fp);
	//printf(" pixel_offset=%u\n", pixel_offset );

	fseek(fp, pixel_offset, SEEK_SET);
	
	char* msg = malloc(1);
	uint32_t i=0;
	for(;;) {
		uint8_t decoded = extract_byte(fp, reverse_bits);;
		msg = realloc(msg, ++i);
		memcpy(msg +(i-1), &decoded, 1);
		if(decoded==0)
			break;
	}
	printf("hidden message='%s'\n", msg );
	free(msg);
}


/* assumes files are identical. msg must be nul terminated */
void hide_message(char* bmp_file_in, 
	char* bmp_file_out, 
	char* msg,
	uint8_t reverse_bits) {
	
	printf("hide_message: msg='%s'\n", msg);
	FILE* fp = fopen(bmp_file_in, "rb");

	if(cp_file(fp, bmp_file_out) != 0)
		exit(1);
	fclose(fp);
	
	FILE* fp_out = fopen(bmp_file_out, "rb+");

	uint32_t pixel_offset = read_pixel_offset(fp_out);
	//printf(" pixel_offset=%u\n", pixel_offset );
	fseek(fp_out, pixel_offset, SEEK_SET);
	
	uint32_t msg_len = strlen(msg);
	int len = (msg_len+1)*8;
	uint8_t* pdata = malloc( len );
	/*uint32_t read =*/ fread(pdata, 1, len, fp_out);
	//printf("read %u bytes, msg len=%d, data len=%d\n", read, msg_len, len);

	//hexdump(pdata, len);

	for(int i=0; i <= msg_len; ++i) {
		//printf("msg[%d]=%c\n", i, msg[i]);
		hide_byte(pdata, msg[i], reverse_bits);
		pdata += 8;
		if(i==msg_len)
			pdata -= (len);
	}
	fseek(fp_out, pixel_offset, SEEK_SET);
	fwrite(pdata, 1, len, fp_out);
	fclose(fp_out);
	free(pdata);
}
