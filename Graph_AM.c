#include <stdio.h>
#include <stdlib.h>
#include<stdbool.h>
#include<string.h>
#include "Graph.h"

struct Graphrep {
	int nE;
	int nV;
	int** edges;
};

Graph NewGraph(int num_vertices) {
	if (num_vertices <= 0) {
		printf("Invalid Graph");
		return NULL;
	}
	else {
		Graph g;
		g = malloc(sizeof(*g));
		if (g == NULL) {
			printf("Out of memory");
			exit(1);
		}
		g->edges = malloc(sizeof(int*) * num_vertices);
		if (g->edges == NULL) {
			printf("Out of memory");
			exit(1);
		}
		for (int i = 0; i < num_vertices; i++) {
			g->edges[i] = malloc(sizeof(int) * num_vertices);
			if (g->edges[i] == NULL) {
				printf("Out of memory");
				exit(1);
			}
			for (int j = 0; j < num_vertices; j++) {
				g->edges[i][j] = 0;
			}
		}
		g->nV = num_vertices;
		g->nE = 0;
		return g;
	}
}

void showGraph(Graph g) {
	if (g == NULL) {
		printf("NULL graph\n");
	}
	else {
		printf("V=%d, E=%d\n", g->nV, g->nE);
		for (int i = 0; i < g->nV; i++) {
			int shown = 0;
			for (int j = 0; j < g->nV; j++) {
				if (j != i && g->edges[i][j] == 1) {
					printf("< %d %d >  ",i,j);
					shown++;
				}
			}
			if (shown > 0) {printf("\n");	}
		}
	}
}

static int validV(Graph g, Vertex v) {
	return (v >= 0 && v < g->nV);
}

Edge newEdge(Vertex v, Vertex w) { 
	Edge e = { v, w };
	return e;
}

void ShowEdge(Edge e) {
	printf("< %d %d>", e.V, e.W);
}

int isEdge(Edge e, Graph g) {
	if (validV(g, e.V) && validV(g, e.W)) {
		return g->edges[e.V][e.W];
	}
	return 0;
}

void insertEdge(Edge e, Graph g) {
	if (g == NULL) {
		printf("Graph not initialized");
	}
	else {
		if (!validV(g, e.V) || !validV(g, e.W)) {
           printf("insertEdge: invalid vertices <%d %d>\n", e.V, e.W);
		}
		else {
			if (isEdge(e, g) == 0) {g->nE++;}
			g->edges[e.V][e.W] = 1;
			g->edges[e.W][e.V] = 1;
		}
	}
}

void removeEdge(Edge e, Graph g) {
	if (g == NULL) {
printf("Graph not initialized");
	}
	else {
	if (!validV(g, e.V) || !validV(g, e.W)) {
		printf("insertEdge: invalid vertices <%d %d>\n", e.V, e.W);
	}
	else {
		if (isEdge(e, g) == 1) {
			g->nE--;
			g->edges[e.V][e.W] = 0;
			g->edges[e.W][e.V] = 0;
		}
	}
	}
}

int isleaf(Graph g, int node) {
	int sum = 0;
	for (int i = node + 1; i < g->nV; i++) {
		sum = sum + g->edges[node][i];
	}
	return sum;
}

void max_path_from_node(Graph g, int** path_matrix, int* visited, int node) {
	visited[node] = 1;
	int child_count = 0;
	for (int j = node + 1; j < g->nV; j++) {
		if (visited[j] == 0 && g->edges[node][j] == 1) {
			visited[j] = 1;
			max_path_from_node(g, path_matrix, visited, j);
		}
	}
	path_matrix[node][node] = 0;

	//finding children with max paths
	int max_dist = 0;
	int max_node = -1;
	for (int j = node + 1; j < g->nV; j++) {
		if (path_matrix[j][j] == max_dist && g->edges[node][j] == 1) { //switch on for equal distances
			path_matrix[node][j] = 1;
			max_dist = path_matrix[j][j];
		}
		if (path_matrix[j][j] > max_dist && g->edges[node][j] == 1) { // switch off for previous if a better node is found
			max_dist = path_matrix[j][j];
			path_matrix[node][j] = 1;
			for (int k = j - 1; k > node; k--) {path_matrix[node][k] = 0;}
		}
	}
	if (isleaf(g, node) != 0) {path_matrix[node][node] = max_dist + 1;}
}

int count_paths(int** path_matrix, int size, int* start_node, int* max_length) {
	int paths = 1;
	*max_length = path_matrix[0][0];
	for (int i = 0; i < size; i++) {
		int branches = 0;
		for (int j = i + 1; j < size; j++) {
			if (path_matrix[i][j] != -1) {branches += path_matrix[i][j];}
			else { path_matrix[i][j] = 0; }
		}
		if (branches != 0) { paths = paths - 1 + branches; }
		//paths *= branches;
		if (i != 0) { path_matrix[i][0] = branches; }
		else if (i==0){path_matrix[2][1] = branches; }	//storing at (2,1) for 0 node
		if (path_matrix[i][i] > * max_length) {	//finding the length of max path and start node
			*max_length = path_matrix[i][i];
			*start_node = i;
		}
	}
	return paths;
}

void print_path(int** path_matrix, char** words, int node, int size) {
	int start_node = 0;
	int max_len = 0;
	int paths_cnt = count_paths(path_matrix, size, &start_node, &max_len);
	int path_no = 1;
	while (paths_cnt > 0) {
		printf(" %d: ", path_no);
		path_no++;
		int next = start_node;

		if (start_node != 0) { path_matrix[start_node][0]--; }
		else if(start_node == 0){ path_matrix[2][1]--; }

		for (int i = 0; i < path_matrix[start_node][start_node]; i++) {
			printf("%s ->", words[next]);
			for (int j = next + 1; j < size; j++) {
				if (path_matrix[next][j] == 1) {
					path_matrix[j][0]--;
					if (path_matrix[j][0] < 1) { path_matrix[next][j] = 0; }
					next = j;
					break;
				}
			}
		}
		printf("%s\n", words[next]);
		paths_cnt--;
		//resetting start_node for next path
		for (int i = start_node; i < size; i++){
			if (i == 0 && path_matrix[2][1]>0){start_node = 0; break; }
			else if (path_matrix[i][0] > 0){start_node = i; break; }
		}
	}
}

bool ladder(char* c1, char* c2) {
	int a = strlen(c1);
	int b = strlen(c2);
	int count = 0;
	if (a == b) {
		for (int i = 0; i < a; i++) {
			if (*(c1 + i) != *(c2 + i)) { count += 1; }
			if (count > 1) { return 0; }
		}
		if (count == 0) { return 0; }
		return 1;
	}
	else if (a - b == 1 || b - a == 1) {
		char* first = a < b ? c1 : c2;
		char* second = a > b ? c1 : c2;
		int n = a < b ? a : b;
		for (int j = 0; j < n; j++) {
			int k = j;
			if (count != 0) {
				k = j + 1;
				if (k == n + 1) { return 0; }
			}
			if (*(first + j) == *(second + k)) { continue; }
			else {
				count += 1;
				if (*(first + j) != *(second + k + 1)) { return 0; }
			}
		}
		return 1;
	}
	return 0;
}