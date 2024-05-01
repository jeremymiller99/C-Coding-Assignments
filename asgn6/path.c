#include "path.h"
#include "stack.h"
#include "graph.h"
#include <stdio.h>
#include <stdlib.h>

struct path {
    	uint32_t total_weight;
    	Stack *vertices;
};

Path *path_create(uint32_t capacity) {
    	Path *p = malloc(sizeof(Path));
    	if (p == NULL) {
        	return NULL;
    	}
    	p->total_weight = 0;
    	p->vertices = stack_create(capacity);
    	if (p->vertices == NULL) {
        	free(p);
        	return NULL;
    	}
    	return p;
}

void path_free(Path **pp) {
    	if (pp && *pp) {
        	stack_free(&((*pp)->vertices));
        	free(*pp);
        	*pp = NULL;
    	}
}

uint32_t path_vertices(const Path *p) {
    	return p ? stack_size(p->vertices) : 0;
}

uint32_t path_distance(const Path *p) {
    	return p ? p->total_weight : 0;
}

void path_add(Path *p, uint32_t val, const Graph *g) {
    	if (!p || !g) return;
    
    	uint32_t lastVertex;
    	if (!stack_empty(p->vertices) && stack_peek(p->vertices, &lastVertex)) {
        	uint32_t weight = graph_get_weight(g, lastVertex, val);
        	p->total_weight += weight;
    	}
    	stack_push(p->vertices, val);
}

uint32_t path_remove(Path *p, const Graph *g) {
    	if (!p || !g || stack_empty(p->vertices)) return 0;

    	uint32_t removedVertex;
    	stack_pop(p->vertices, &removedVertex);
    	if (!stack_empty(p->vertices)) {
        	uint32_t lastVertex;
        	stack_peek(p->vertices, &lastVertex);
    	}
    	return removedVertex;
}

void path_copy(Path *dst, const Path *src) {
    	if (!src || !dst) return;

    	dst->total_weight = src->total_weight;
    	stack_copy(dst->vertices, src->vertices);
}

void path_print(const Path *p, FILE *f, const Graph *g) {
    	if (!p || !f || !g) return;

    	fprintf(f, "Alissa starts at: \n");
    	Stack *tempStack = stack_create(stack_size(p->vertices));
    	while (!stack_empty(p->vertices)) {
        	uint32_t vertex;
        	stack_pop(p->vertices, &vertex);
        	stack_push(tempStack, vertex);
    	}


    	uint32_t firstVertex;
    	bool firstVertexSet = false;

    	while (!stack_empty(tempStack)) {
        	uint32_t vertex;
        	stack_pop(tempStack, &vertex);
        	fprintf(f, "%s\n", graph_get_vertex_name(g, vertex));
        	stack_push(p->vertices, vertex);
    
    		if (!firstVertexSet) {
            		firstVertex = vertex;
        		firstVertexSet = true;
		}
    	}

    	if (firstVertexSet) {
        	fprintf(f, "%s\n", graph_get_vertex_name(g, firstVertex));
   	}

    	fprintf(f, "Total Distance: %u\n", p->total_weight);
	stack_free(&tempStack);
}

