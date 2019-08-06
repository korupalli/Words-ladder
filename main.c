#include <stdio.h>
#include<string.h>
#include <stdio.h>
#include<stdlib.h>
#include<stdbool.h>
#include "Graph.h"
#define MAX_LIMIT 10

int main()
{
	char a[100];
	int i = 0;
	do {
		char c = fgetc(stdin);
		if (feof(stdin) || c == '\n') {
			break;
		}
		a[i] = c;
		i++;
	} while (1);
	char* s = malloc(sizeof(char) * (i + 1));
	for (int j = 0; j < i; j++) {
		s[j] = a[j];
	}
	s[i] = '\0';
	char* token;
	char* rest = s;
	char* words[100];
	int n = 0;
	while ((token = strtok_s(rest, " ", &rest))) {
		if (n!=0 && strcmp(token,words[n - 1])!=0) {
			words[n] = token;
			n++;
		}
		else if(n == 0) {
			words[n] = token;
			n++;
		}
	}
	printf("siz is %d\n", n);
	int q = 0;

	Graph g = NewGraph(n);
	for (int i = 0; i < (n - 1); i++) {
		for (int j = i + 1; j < n; j++) {
			q = 0;
			if (ladder(words[i], words[j])) {
				q += 1;
				insertEdge(newEdge(i, j), g);
			}
		}
	}
	printf("Ordered Word Ladder Graph\n");
	showGraph(g);

	int** path_memory = malloc(sizeof(int*) * n);
	int* visited = malloc(sizeof(int) * n);

	for (int i = 0; i < n; i++) {
		path_memory[i] = malloc(sizeof(int) * n);
		visited[i] = 0;
		for (int j = 0; j < n; j++) {
			path_memory[i][j] = -1;
		}
	}

	max_path_from_node(g, path_memory, visited, 0);
	/*for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			printf("%3d ", path_memory[i][j]);
		}
		printf("\n");
	}*/

	printf("Longest ladder length:: %d\n", path_memory[0][0]+1);
	printf("Longest ladders:\n");
	print_path(path_memory, words, 0, n);

	for (int i = 0; i < n; i++) {
		free(path_memory[i]);
	}
	free(path_memory);
	free(visited);
	free(s);

	return 0;

}
