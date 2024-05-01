#include "pq.h"
#include <stdio.h>
#include <stdlib.h>

typedef struct PriorityQueueElement {
    	Node *node;
    	struct PriorityQueueElement *next;
} PriorityQueueElement;

struct PriorityQueue {
    	PriorityQueueElement *head;
};

PriorityQueue *pq_create(void) {
    	PriorityQueue *q = (PriorityQueue *)malloc(sizeof(PriorityQueue));
    	if (!q) {
        	return NULL;
    	}
    	q->head = NULL;
    	return q;
}

void pq_free(PriorityQueue **q) {
    	if (q == NULL || *q == NULL) {
        	return;
    	}
   	PriorityQueueElement *current = (*q)->head;
   	while (current != NULL) {
        	PriorityQueueElement *temp = current;
        	current = current->next;
        	free(temp);
    	}
    	free(*q);
    	*q = NULL;
}

bool pq_is_empty(PriorityQueue *q) {
    	return q == NULL || q->head == NULL;
}

bool pq_size_is_1(PriorityQueue *q) {
    	if (q == NULL || q->head == NULL) {
        	return false;
    	}
    	return q->head->next == NULL;
}

void enqueue(PriorityQueue *q, Node *tree) {
    	assert(q != NULL);
    	PriorityQueueElement *newElement = (PriorityQueueElement *)malloc(sizeof(PriorityQueueElement));
    	assert(newElement != NULL);

    	newElement->node = tree;
    	newElement->next = NULL;

    	if (q->head == NULL || q->head->node->weight > tree->weight) {
       		newElement->next = q->head;
        	q->head = newElement;
    	} else {
        	PriorityQueueElement *current = q->head;
        	while (current->next != NULL && current->next->node->weight <= tree->weight) {
            		current = current->next;
        	}
        	newElement->next = current->next;
        	current->next = newElement;
    	}
}

Node *dequeue(PriorityQueue *q) {
    	assert(q != NULL && q->head != NULL);	
    	PriorityQueueElement *temp = q->head;
    	Node *node = temp->node;
    	q->head = q->head->next;
    	free(temp);
    	return node;
}

void pq_print(PriorityQueue *q) {
    	assert(q != NULL);
    	PriorityQueueElement *current = q->head;
    	while (current != NULL) {
        	printf("Node weight: %u\n", current->node->weight);
        	current = current->next;
    	}
}

