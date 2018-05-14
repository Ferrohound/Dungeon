/*
	Graph class
    directed v undirected
*/

#include <vector>
#include <string>
#include <iostream>
#include <algorithm> /* for std::find, std::sort */

using std::string;
using std::vector;

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


struct AdjGraph
{
	int V;
    struct AdjList* array;
};



//==========================================================
//Node and edge style
//should probably connect this to the dungeon in one way or another..

//template?
template <class T>
struct Node
{
    int dest;
    vector< Link <T> > edges;
    T data;
    
    //============================================================= TO DO
    friend bool operator== (const Node& n1, const Node& n2)
    {
        return (n1.data == n2.data);
    }
};

//template?
//worry about this later...
template <class T>
struct Link
{
    Node<T>* to;
    Node<T>* from;
    int weight;

    friend bool operator== (const Link &L1, const Link &L2)
    {
        return (L1.from == L2.from && L1.to == L2.to);
    }

    //for sorting
    bool operator < (const Link& L) const
    {
        return (weight < L.weight);
    }
};

template <class T>
class Graph
{
    public:
        //================================functions
        Graph();

        void AddEdge(Node<T>* A, Node<T>* b, int weight = 1);
        void AddEdge(Link<T> E);
        void AddNode(Node<T>* A);
        void RemoveNode(Node<T>* A);
        void RemoveEdge(Link<T> L);
        
        Link<T>* PopEdge( Link<T> L);
        Node<T>* GetNode(T data);

        Graph<T> MST();

        //function to save an ascii and coordinates of a tile in the room/hallway
        //use floodfill to get the tiles in the room and rebuild
        void Save(string path);

    //================================members

    private:
        int size;
        //set instead of vector? maybe.
        /*
            Array
            Set
            List
            Vector
        */
        vector< Link<T> > > edges;
        vector< Node<T>* > nodes;
};