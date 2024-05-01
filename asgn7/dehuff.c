#include "bitreader.h"
#include "node.h"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <unistd.h>

#define MAX_STACK_SIZE 64


void dehuff_decompress_file(FILE *fout, BitReader *inbuf);

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
                		fprintf(stderr, "Usage: %s -i <input_file.huff> -o <output_file>\n", argv[0]);
                		return EXIT_FAILURE;
        	}
    	}

    	if (!inputFilePath || !outputFilePath) {
        	fprintf(stderr, "Both input and output files must be specified.\n");
        	fprintf(stderr, "Usage: %s -i <input_file.huff> -o <output_file>\n", argv[0]);
        	return EXIT_FAILURE;
    	}

    	FILE *fin = fopen(inputFilePath, "rb");
    	if (!fin) {
        	perror("Error opening input file");
        	return EXIT_FAILURE;
    	}

    	BitReader *inbuf = bit_read_open(inputFilePath);
    	if (!inbuf) {
        	fprintf(stderr, "Failed to initialize BitReader\n");
        	fclose(fin);
        	return EXIT_FAILURE;
    	}

    	FILE *fout = fopen(outputFilePath, "wb");
    	if (!fout) {
        	perror("Error opening output file");
        	bit_read_close(&inbuf);
        	fclose(fin);
        	return EXIT_FAILURE;
    	}

    	dehuff_decompress_file(fout, inbuf);
    	bit_read_close(&inbuf);
    	fclose(fout);
    	fclose(fin);

    	return EXIT_SUCCESS;
}

Node* stack[MAX_STACK_SIZE];
int stack_top = -1;

void stack_push(Node* node) {
    	if (stack_top < MAX_STACK_SIZE - 1) {
        	stack[++stack_top] = node;
    	} else {
        	fprintf(stderr, "Stack overflow\n");
        	exit(EXIT_FAILURE);
    	}
}

Node* stack_pop(void) {
    	if (stack_top >= 0) {
        	return stack[stack_top--];
    	} else {
        	fprintf(stderr, "Stack underflow\n");
        	exit(EXIT_FAILURE);
    	}
}

void free_huffman_tree(Node* node) {
    	if (node == NULL) {
        	return;
    	}
    	free_huffman_tree(node->left);
    	free_huffman_tree(node->right);
    	free(node);
}

void dehuff_decompress_file(FILE *fout, BitReader *inbuf) {
    	uint8_t type1 = bit_read_uint8(inbuf);
    	uint8_t type2 = bit_read_uint8(inbuf);
    	uint32_t filesize = bit_read_uint32(inbuf);
    	uint16_t num_leaves = bit_read_uint16(inbuf);

    	assert(type1 == 'H' && type2 == 'C');
    	int num_nodes = 2 * num_leaves - 1;
    	Node* node = NULL;

    	for (int i = 0; i < num_nodes; ++i) {
        	uint8_t bit = bit_read_bit(inbuf);
        	if (bit == 1) {
            		uint8_t symbol = bit_read_uint8(inbuf);
            		node = node_create(symbol, 0);
        	} else {
            		node = node_create(0, 0);
            		node->right = stack_pop();
            		node->left = stack_pop();
        	}
        	stack_push(node);
    	}

    	Node* code_tree = stack_pop();

    	for (uint32_t i = 0; i < filesize; ++i) {
        	node = code_tree;
        	while (node->left != NULL || node->right != NULL) {
            		uint8_t bit = bit_read_bit(inbuf);
            		node = (bit == 0) ? node->left : node->right;
        	}
        	fputc(node->symbol, fout);
    	}
    	free_huffman_tree(code_tree);
}

