#include "graph.h"
#include "path.h"
#include "stack.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <limits.h>

#define START_VERTEX 0

void dfs(Graph *g, uint32_t vertex, Path *currentPath, Path *bestPath, bool *visited);
Graph *loadGraph(const char *filename, bool directed);
void printPath(const Path *path, const Graph *g, FILE *outfile);

int main(int argc, char **argv) {
    	char *inputFile = NULL;
    	char *outputFile = NULL;
    	FILE *outfile = stdout;
    	bool directed = false;

    	int opt;
    	while ((opt = getopt(argc, argv, "i:o:dh")) != -1) {
        	switch (opt) {
            		case 'i':
                		inputFile = optarg;
                		break;
            		case 'o':
                		outputFile = optarg;
                		break;
            		case 'd':
                		directed = true;
                		break;
            		case 'h':
            		default:
                		printf("Usage: %s [-i inputfile.graph] [-o outputfile.txt] [-d] [-h]\n", argv[0]);
                		return opt == 'h' ? EXIT_SUCCESS : EXIT_FAILURE;
        	}
    	}

    	if (!inputFile) {
        	fprintf(stderr, "Input file is required\n");
        	return EXIT_FAILURE;
    	}

    	Graph *g = loadGraph(inputFile, directed);
    	if (!g) {
        	fprintf(stderr, "Failed to load graph from %s\n", inputFile);
		return EXIT_FAILURE;
    	}

    	if (outputFile) {
        	outfile = fopen(outputFile, "w");
        	if (!outfile) {
            		fprintf(stderr, "Failed to open output file %s\n", outputFile);
            		graph_free(&g);
            		return EXIT_FAILURE;
        	}
    	}

    	Path *currentPath = path_create(graph_vertices(g));
    	Path *bestPath = path_create(graph_vertices(g));
    	if (!currentPath || !bestPath) {
        	fprintf(stderr, "Failed to allocate memory for paths\n");
        	return EXIT_FAILURE;
    	}

    	bool *visited = calloc(graph_vertices(g), sizeof(bool));
    	if (!visited) {
        	fprintf(stderr, "Memory allocation failed for visited array\n");
        	graph_free(&g);
        	if (outfile != stdout) fclose(outfile);
        		return EXIT_FAILURE;
    	}

    	for (uint32_t i = 0; i < graph_vertices(g); i++) {
        	visited[i] = false;
    	}

    	dfs(g, START_VERTEX, currentPath, bestPath, visited);

    	if (path_distance(bestPath) != 0) {
        	printPath(bestPath, g, outfile);
    	} else {
        	fprintf(outfile, "No path found! Alissa is lost!\n");
    	}

    	free(visited);
    	graph_free(&g);
    	path_free(&currentPath);
    	path_free(&bestPath);
    	if (outfile != stdout) {
        	fclose(outfile);
    	}

    	return EXIT_SUCCESS;
}

void dfs(Graph *g, uint32_t vertex, Path *currentPath, Path *bestPath, bool *visited) {
    	visited[vertex] = true;
    	path_add(currentPath, vertex, g);

    	bool allVisited = true;
    	for (uint32_t i = 0; i < graph_vertices(g); i++) {
        	if (!visited[i]) {
            		allVisited = false;
        		break;
        	}
    	}

    	if (allVisited && graph_get_weight(g, vertex, START_VERTEX) > 0) {
        	path_add(currentPath, START_VERTEX, g);

        	if (path_distance(currentPath) < path_distance(bestPath) || path_distance(bestPath) == 0) {
            		path_copy(bestPath, currentPath);
        	}
        	path_remove(currentPath, g);
    	} else {
        	for (uint32_t nextVertex = 0; nextVertex < graph_vertices(g); nextVertex++) {
            		if (!visited[nextVertex] && graph_get_weight(g, vertex, nextVertex) > 0) {
                		dfs(g, nextVertex, currentPath, bestPath, visited);
            		}
        	}
    	}

    	visited[vertex] = false;
    	path_remove(currentPath, g);
}

Graph *loadGraph(const char *filename, bool directed) {
    	FILE *file = fopen(filename, "r");
    	if (!file) {
        	perror("Failed to open file");
        	return NULL;
    	}

    	uint32_t vertices;
    	fscanf(file, "%u", &vertices);

    	Graph *g = graph_create(vertices, directed);
    	if (!g) {
        	fclose(file);
        	return NULL;
    	}

    	char line[PATH_MAX];
    	fgets(line, PATH_MAX, file);

    	for (uint32_t i = 0; i < vertices; i++) {
        	if (!fgets(line, PATH_MAX, file)) {
            		fprintf(stderr, "Error reading vertex name\n");
            		graph_free(&g);
            		fclose(file);
            		return NULL;
        	}
        
        	line[strcspn(line, "\n")] = 0;
        	graph_add_vertex(g, line, i);
    	}

    	uint32_t numEdges;
    	fscanf(file, "%u", &numEdges);

    	uint32_t start, end;
    	uint32_t weight;
    	for (uint32_t i = 0; i < numEdges; i++) {
        	fscanf(file, "%u %u %u", &start, &end, &weight);
		graph_add_edge(g, start, end, weight);
        	if (!directed) {
            		graph_add_edge(g, end, start, weight);
        	}
    	}

    	fclose(file);
    	return g;
}

void printPath(const Path *path, const Graph *g, FILE *outfile) {
    	path_print(path, outfile, g);
}

