
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

uint8_t extract_byte_raw(uint8_t* data, uint8_t reverse_bits) {
	uint8_t values[8];
	if(reverse_bits) {
		for(int i=7; i>=0; --i) {
			values[i] = *(data++);
		}
	}
	else {
		for(int i=0; i<8; ++i) {
			values[i] = *(data++);
		}
	}
	return get_byte(values);
}


void hide_message(char* msg, 
	uint8_t* data,
	size_t data_len,
	uint8_t reverse_bits) {

	uint32_t msg_len = strlen(msg);

	if(msg_len > (data_len/8))
		return;

	int i;
	for(i=0; i <= msg_len; ++i) {
		//printf("msg[%d]=%c\n", i, msg[i]);
		hide_byte(data+(i*8), msg[i], reverse_bits);
	}
	hide_byte(data+(++i*8), 0x0, reverse_bits);	
}


char* reveal_message(uint8_t* data,
	size_t data_len,
	uint8_t reverse_bits) {

	char* msg=malloc(1);
	int count=0;
	for(int i=0; i<data_len; i+=8) {
		
		uint8_t decoded = extract_byte_raw(data+i, reverse_bits);
		msg = realloc(msg, ++count );
		memcpy(msg +(count-1), &decoded, 1);
		if(decoded == 0x0) {
			break;
		}
	}
	return msg;
}

