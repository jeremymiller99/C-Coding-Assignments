#include "bitwriter.h"
#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>

struct BitWriter {
    	FILE *underlying_stream;
    	uint8_t byte;
    	uint8_t bit_position;
};

BitWriter *bit_write_open(const char *filename) {
    	BitWriter *writer = (BitWriter *)malloc(sizeof(BitWriter));
    	if (!writer) {
        	return NULL;
    	}

    	writer->underlying_stream = fopen(filename, "wb");
    	if (!writer->underlying_stream) {
        	free(writer);
		return NULL;
    	}

    	writer->byte = 0;
    	writer->bit_position = 0;
    	return writer;
}

void bit_write_close(BitWriter **pbuf) {
    	if (pbuf && *pbuf) {
        	if ((*pbuf)->bit_position > 0) {
            	fputc((*pbuf)->byte, (*pbuf)->underlying_stream);
        	}
        	fclose((*pbuf)->underlying_stream);
        	free(*pbuf);
        	*pbuf = NULL;
    	}
}

void bit_write_bit(BitWriter *buf, uint8_t bit) {
    	if (!buf) return;

    	if (bit) {
        	buf->byte |= (1 << buf->bit_position);
    	}

    	buf->bit_position++;

    	if (buf->bit_position == 8) {
        	fputc(buf->byte, buf->underlying_stream);
        	buf->byte = 0;
        	buf->bit_position = 0;
    	}
}

void bit_write_uint8(BitWriter *buf, uint8_t byte) {
    	for (int i = 0; i < 8; i++) {
        	bit_write_bit(buf, (byte >> i) & 1);
    	}
}

void bit_write_uint16(BitWriter *buf, uint16_t x) {
    	for (int i = 0; i < 16; i++) {
        	bit_write_bit(buf, (x >> i) & 1);
    	}
}

void bit_write_uint32(BitWriter *buf, uint32_t x) {
    	for (int i = 0; i < 32; i++) {
        	bit_write_bit(buf, (x >> i) & 1);
    	}
}	

