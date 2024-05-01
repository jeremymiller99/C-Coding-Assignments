#include "graph.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct graph {
    	uint32_t vertices;
    	bool directed;
    	bool *visited;
    	char **names;
    	uint32_t **weights;
};

Graph *graph_create(uint32_t vertices, bool directed) {
    	Graph *g = calloc(1, sizeof(Graph));
    	if (g == NULL) {
        	return NULL;
    	}

    	g->vertices = vertices;
    	g->directed = directed;
    	g->visited = calloc(vertices, sizeof(bool));
    	g->names = calloc(vertices, sizeof(char *));
    	g->weights = calloc(vertices, sizeof(uint32_t *));

    	if (g->visited == NULL || g->names == NULL || g->weights == NULL) {
        	free(g->visited);
        	free(g->names);
        	for (uint32_t i = 0; i < vertices; ++i) {
            		free(g->weights[i]);
        	}
        	free(g->weights);
        	free(g);
        	return NULL;
    	}

    	for (uint32_t i = 0; i < vertices; ++i) {
        	g->weights[i] = calloc(vertices, sizeof(uint32_t));
        	if (g->weights[i] == NULL) {
            		for (uint32_t j = 0; j < i; ++j) {
                		free(g->weights[j]);
            		}
            		free(g->weights);
            		free(g->visited);
            		free(g->names);
            		free(g);
            		return NULL;
        	}
    	}

    	return g;
}

void graph_free(Graph **gp) {
    	if (gp == NULL || *gp == NULL) {
        	return;
    	}

    	Graph *g = *gp;

    	for (uint32_t i = 0; i < g->vertices; i++) {
        	free(g->names[i]);
        	free(g->weights[i]);
    	}

    	free(g->visited);
    	free(g->names);
    	free(g->weights);
    	free(g);
    	*gp = NULL;
}

uint32_t graph_vertices(const Graph *g) {
    	return g ? g->vertices : 0;
}

void graph_add_vertex(Graph *g, const char *name, uint32_t v) {
    	if (v >= g->vertices) {
        	return;
    	}
    	g->names[v] = strdup(name);
}

void graph_add_edge(Graph *g, uint32_t start, uint32_t end, uint32_t weight) {
    	if (start >= g->vertices || end >= g->vertices) {
        	return;
    	}
    	g->weights[start][end] = weight;
    	if (!g->directed) {
        	g->weights[end][start] = weight;
    	}
}

uint32_t graph_get_weight(const Graph *g, uint32_t start, uint32_t end) {
    	if (start >= g->vertices || end >= g->vertices) {
        	return 0;
    	}
    	return g->weights[start][end];
}

void graph_visit_vertex(Graph *g, uint32_t v) {
    	if (v < g->vertices) {
        	g->visited[v] = true;
    	}
}

void graph_unvisit_vertex(Graph *g, uint32_t v) {
    	if (v < g->vertices) {
        	g->visited[v] = false;
    	}
}

bool graph_visited(const Graph *g, uint32_t v) {
    	return v < g->vertices ? g->visited[v] : false;
}

char **graph_get_names(const Graph *g) {
    	return g->names;
}

const char *graph_get_vertex_name(const Graph *g, uint32_t v) {
    	if (v >= g->vertices) {
        	return NULL;
    	}
   	return g->names[v];
}

void graph_print(const Graph *g) {
    	if (!g) return;
    	printf("Graph with %u vertices:\n", g->vertices);
    	for (uint32_t i = 0; i < g->vertices; i++) {
        	printf("Vertex %u (%s):", i, g->names[i]);
        	for (uint32_t j = 0; j < g->vertices; j++) {
            		if (g->weights[i][j] != 0) {
                		printf(" -> %u (%u)", j, g->weights[i][j]);
            		}
        	}
        	printf("\n");
    	}
}

