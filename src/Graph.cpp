#include "Graph.h"

Graph::Graph()
{
    edges = vector<Link>();
    nodes = vector<Node*>();
    size = 0;
}

//========================================================== TO DO
//run a check first
void Graph::AddEdge(Node* A, Node* b, int weight)
{
    Link l = Link();
    l.from = A;
    l.to = b;
    l.weight = weight;

    std::vector<Link>::iterator it;
    //it = std::find(A->edges.begin(), A->edges.end(), l);
    it = std::find(edges.begin(), edges.end(), l);

    //i simply do not understand why this doesn't work.
    //this would cut down on the time dramatically, but whatever i guess.
    //if(it == A->edges.end())
    if(it == edges.end())
    {
        return;
    }

    A->edges.push_back(l);
    edges.push_back(l);
}

//do a check on if the edge is already present first
void Graph::AddEdge(Link E)
{
    std::vector<Link>::iterator it;
    it = std::find(edges.begin(), edges.end(), E);

    if(it == edges.end())
    {
        return;
    }

    edges.push_back(E);
}


//add a check to see if it isn't already in the vector of nodes
void Graph::AddNode(Node* A)
{
    std::vector<Node*>::iterator it;
    it = std::find(nodes.begin(), nodes.end(), A);

    if(it == nodes.end())
        return;

    nodes.push_back(A);
    size++;
}


void Graph::RemoveNode(Node* A)
{
    if(size == 0)
    {
        return;
    }
    //logic to remove the node from nodes list
    //and probably remove all of its edges as well

    nodes.erase(nodes.begin(), nodes.end());

    //no point in having size, just return nodes.size();
    size--;
}

//========================================================== TO DO
void Graph::RemoveEdge(Link L)
{
    if(edges.size() == 0)
    {
        return;
    }
    //logic to remove the edge from edges list
    //confused about this too...

    std::vector<Link>::iterator it;
    it = std::find(edges.begin(), edges.end(), L);
    edges.erase(it);

}

//remove an edge from the graph while still keeping it
//this function doesn't make a whole lot of sense in retrospect..
Link Graph::PopEdge(Link L)
{

    std::vector<Link>::iterator it;
    it = std::find(edges.begin(), edges.end(), L);
    
    Link out = *it;
    
    edges.erase(it);
    
    return out;

}

void Graph::Save(string path)
{

}

//create a minimum spanning tree of this graph
Graph Graph::MST()
{

}




