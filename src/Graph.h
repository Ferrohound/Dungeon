/*
	Graph Class
*/

//adjacency list node
struct AdjNode
{
    int dest;
    struct AdjNode* next;
};

struct AdjList
{
    struct AdjNode *head; 
};

//edge struct?


struct Graph
{
	int V;
    struct AdjList* array;
};
