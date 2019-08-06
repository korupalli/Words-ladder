
typedef int Vertex;
typedef struct {
	Vertex V;
	Vertex W;
} Edge;

typedef struct Graphrep* Graph;

Graph NewGraph(int num_vertices);
void showGraph(Graph g);
static int validV(Graph g, Vertex v);
Edge newEdge(Vertex v, Vertex w);
void ShowEdge(Edge e);
int isEdge(Edge e, Graph g);
void insertEdge(Edge e, Graph g);
void removeEdge(Edge e, Graph g);

void max_path_from_node(Graph g, int** path_matrix, int* visited, int node);
bool ladder(char* c1, char* c2);
int count_paths(int** path_matrix, int size, int* start_node, int* max_length);
void print_path(int** path_matrix, char** words, int node, int size);
