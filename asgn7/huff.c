#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <string.h>
#include "node.h"
#include "pq.h"
#include "bitwriter.h"

#define HISTOGRAM_SIZE 256

typedef struct {
    	uint64_t code;
    	uint8_t code_length;
} Code;

uint32_t fill_histogram(FILE *fin, uint32_t *histogram);
Node *create_tree(uint32_t *histogram, uint16_t *num_leaves);
void fill_code_table(Code *code_table, Node *node, uint64_t code, uint8_t code_length);
void huff_compress_file(BitWriter *outbuf, FILE *fin, uint32_t filesize, uint16_t num_leaves, Node *code_tree, Code *code_table);
void huff_write_tree(BitWriter *outbuf, Node *node);

int main(int argc, char *argv[]) {
    	char *inputFilePath = NULL;
    	char *outputFilePath = NULL;
    	int opt;

    	while ((opt = getopt(argc, argv, "i:o:")) != -1) {
        	switch (opt) {
            		case 'i':
                	inputFilePath = optarg;
                	break;
            	case 'o':
                	outputFilePath = optarg;
                	break;
            	default:
                	fprintf(stderr, "Usage: %s -i <input file> -o <output file>\n", argv[0]);
                	return 1;
        	}
    	}

    	if (!inputFilePath || !outputFilePath) {
        	fprintf(stderr, "Both input and output files must be specified.\n");
        	fprintf(stderr, "Usage: %s -i <input file> -o <output file>\n", argv[0]);
        	return 1;
    	}

    	FILE *fin = fopen(inputFilePath, "rb");
    	if (!fin) {
        	perror("Error opening input file");
        	return 1;
    	}

    	uint32_t histogram[HISTOGRAM_SIZE];
    	uint32_t filesize = fill_histogram(fin, histogram);

    	uint16_t num_leaves;
    	Node *code_tree = create_tree(histogram, &num_leaves);

    	Code code_table[HISTOGRAM_SIZE];
    	memset(code_table, 0, sizeof(code_table));
    	fill_code_table(code_table, code_tree, 0, 0);

    	BitWriter *outbuf = bit_write_open(outputFilePath);
    	if (!outbuf) {
        	fprintf(stderr, "Error opening output file for writing\n");
        	fclose(fin);
        	return 1;
    	}

    	rewind(fin);
    	huff_compress_file(outbuf, fin, filesize, num_leaves, code_tree, code_table);

    	bit_write_close(&outbuf);
    	fclose(fin);

    	printf("Compression completed successfully.\n");

    	return 0;
}

uint32_t fill_histogram(FILE *fin, uint32_t *histogram) {
    	uint32_t filesize = 0;
    	int byte;
    	memset(histogram, 0, HISTOGRAM_SIZE * sizeof(uint32_t));
    	histogram[0x00]++;
    	histogram[0xff]++;
    	while ((byte = fgetc(fin)) != EOF) {
        	histogram[(unsigned char)byte]++;
        	filesize++;
    	}
    	return filesize;
}

Node *create_tree(uint32_t *histogram, uint16_t *num_leaves) {
    	PriorityQueue *pq = pq_create();
    	*num_leaves = 0;
    	for (int i = 0; i < HISTOGRAM_SIZE; i++) {
        	if (histogram[i] > 0) {
            	enqueue(pq, node_create((uint8_t)i, histogram[i]));
            	(*num_leaves)++;
        	}
    	}

    	while (pq_size_is_1(pq) == false) {
        	Node *left = dequeue(pq);
        	Node *right = dequeue(pq);
        	uint32_t combined_weight = left->weight + right->weight;
        	Node *parent = node_create(0, combined_weight);
        	parent->left = left;
        	parent->right = right;
        	enqueue(pq, parent);
    	}

    	Node *root = dequeue(pq);
    	pq_free(&pq);
    	return root;
}

void fill_code_table(Code *code_table, Node *node, uint64_t code, uint8_t code_length) {
    	if (!node->left && !node->right) {
        	code_table[node->symbol].code = code;
        	code_table[node->symbol].code_length = code_length;
    	} else {
        	if (node->left) {
            	fill_code_table(code_table, node->left, code, code_length + 1);
        	}
        	if (node->right) {
            		code |= (uint64_t)1 << code_length;
            		fill_code_table(code_table, node->right, code, code_length + 1);
        	}
    	}
}

void huff_compress_file(BitWriter *outbuf, FILE *fin, uint32_t filesize, uint16_t num_leaves, Node *code_tree, Code *code_table) {
    	bit_write_uint8(outbuf, 'H');
    	bit_write_uint8(outbuf, 'C');
    	bit_write_uint32(outbuf, filesize);
    	bit_write_uint16(outbuf, num_leaves);
    	huff_write_tree(outbuf, code_tree);

    	rewind(fin);
    	int b;
    	while ((b = fgetc(fin)) != EOF) {
        	uint64_t code = code_table[b].code;
        	uint8_t code_length = code_table[b].code_length;
        	for (int i = 0; i < code_length; i++) {
        		bit_write_bit(outbuf, code & 1);
            		code >>= 1;
        	}
    	}
}

void huff_write_tree(BitWriter *outbuf, Node *node) {
    	if (!node->left && !node->right) {
        	bit_write_bit(outbuf, 1);
        	bit_write_uint8(outbuf, node->symbol);
    	} else {
        	huff_write_tree(outbuf, node->left);
        	huff_write_tree(outbuf, node->right);
        	bit_write_bit(outbuf, 0);
    	}
}


