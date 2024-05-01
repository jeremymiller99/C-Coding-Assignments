#include "node.h"
#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>

Node *node_create(uint8_t symbol, uint32_t weight) {
	Node *newNode = (Node *)malloc(sizeof(Node));
    	if (newNode == NULL) {
    		return NULL;
    	}

    	newNode->symbol = symbol;
    	newNode->weight = weight;
    	newNode->code = 0;
    	newNode->code_length = 0;
    	newNode->left = NULL;
    	newNode->right = NULL;

    	return newNode;
}

void node_free(Node **node) {
    	if (node == NULL || *node == NULL) {
        	return;
    	}

    	node_free(&((*node)->left));
    	node_free(&((*node)->right));

    	free(*node);
    	*node = NULL;
}

static void node_print(const Node *node, int level) {
    	if (node == NULL) {
        	return;
    	}

    	for (int i = 0; i < level; i++) {
        	printf("  ");
    	}

    	printf("Symbol: 0x%02x, Weight: %u", node->symbol, node->weight);
    	if (node->left == NULL && node->right == NULL) {
        	printf(", Code: %" PRIu64 ", Length: %u", node->code, node->code_length);
    	}
    	printf("\n");

    	node_print(node->left, level + 1);
    	node_print(node->right, level + 1);
}

void node_print_tree(Node *tree) {
    	node_print(tree, 0);
}

