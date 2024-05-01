#include "bitreader.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <inttypes.h>

struct BitReader {
    	FILE *underlying_stream;
    	uint8_t byte;
    	int8_t bit_position;
    	bool end_of_file;
};

BitReader *bit_read_open(const char *filename) {
    	BitReader *reader = (BitReader *)malloc(sizeof(BitReader));
    	if (!reader) {
        	return NULL;
    	}

    	reader->underlying_stream = fopen(filename, "rb");
    	if (!reader->underlying_stream) {
        	free(reader);
        	return NULL;
    	}

    	reader->byte = 0;
    	reader->bit_position = 8;
    	reader->end_of_file = false;
    	return reader;
}

void bit_read_close(BitReader **pbuf) {
    	if (pbuf && *pbuf) {
        	fclose((*pbuf)->underlying_stream);
        	free(*pbuf);
        	*pbuf = NULL;
    	}
}

uint8_t bit_read_bit(BitReader *buf) {
    	if (!buf || buf->end_of_file) {
        	return 0;
    	}

    	if (buf->bit_position == 8) {
        	if (fread(&buf->byte, 1, 1, buf->underlying_stream) == 1) {
            		buf->bit_position = 0;
        	} else {
            		buf->end_of_file = true;
            		return 0;
        	}
    	}

    	uint8_t bit = (buf->byte >> buf->bit_position) & 1;
    	buf->bit_position++;
    	return bit;
}

uint8_t bit_read_uint8(BitReader *buf) {
    	uint8_t result = 0;
    	for (int i = 0; i < 8; i++) {
        	result |= (bit_read_bit(buf) << i);
    	}
    	return result;
}

uint16_t bit_read_uint16(BitReader *buf) {
    	uint16_t result = 0;
    	for (int i = 0; i < 16; i++) {
        	result |= ((uint16_t)bit_read_bit(buf) << i);
    	}
    	return result;
}

uint32_t bit_read_uint32(BitReader *buf) {
    	uint32_t result = 0;
    	for (int i = 0; i < 32; i++) {
        	result |= ((uint32_t)bit_read_bit(buf) << i);
    	}
    	return result;
}

