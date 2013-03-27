#ifndef STEGO_H_
#define STEGO_H_

/* 
	stego.h 

	steganography related functions

  Author:Teemu Patja <tpatja@cs.helsinki.fi>
*/

#include <stdint.h>
#include <stdio.h>

uint8_t combine_lsbs(uint8_t* lsbs, uint8_t count);
void bits_from_byte(uint8_t* bits, uint8_t var);
void show_byte_as_bits(uint8_t val);
uint8_t set_bit(uint8_t n, uint8_t var);
uint8_t unset_bit(uint8_t n, uint8_t var);
uint8_t get_lsb(uint8_t val);
uint8_t set_lsb(uint8_t var, uint8_t val);
void hide_byte(uint8_t* data, uint8_t value, uint8_t reverse_bits);
uint8_t get_byte(uint8_t* data);

uint8_t read_lsb(FILE* fp);
uint8_t extract_byte(FILE* fp, uint8_t reverse_bits);

#endif
