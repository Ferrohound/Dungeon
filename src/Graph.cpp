#include "Graph.h"

template <class T>
Graph::Graph()
{
    edges = vector< Link< Node<T> > >();
    nodes = vector< Node<T>* >();
    size = 0;
}

//========================================================== TO DO
//run a check first
template <class T>
void Graph::AddEdge(Node<T>* A, Node<T>* b, int weight)
{
    Link l = Link<T>();
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
template <class T>
void Graph::AddEdge(Link<T> E)
{
    std::vector< Link<T> >::iterator it;
    it = std::find(edges.begin(), edges.end(), E);

    if(it == edges.end())
    {
        return;
    }

    edges.push_back(E);
}


//add a check to see if it isn't already in the vector of nodes
template <class T>
void Graph::AddNode(Node<T>* A)
{
    std::vector< Node<T>* >::iterator it;
    it = std::find(nodes.begin(), nodes.end(), A);

    if(it == nodes.end())
        return;

    nodes.push_back(A);
    size++;
}

template <class T>
void Graph::RemoveNode(Node<T>* A)
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
template <class T>
void Graph::RemoveEdge(Link<T> L)
{
    if(edges.size() == 0)
    {
        return;
    }
    //logic to remove the edge from edges list
    //confused about this too...

    std::vector< Link<T> >::iterator it;
    it = std::find(edges.begin(), edges.end(), L);
    edges.erase(it);

}

//remove an edge from the graph while still keeping it
//this function doesn't make a whole lot of sense in retrospect..
template <class T>
Link<T>* Graph::PopEdge(Link L)
{

    std::vector<Link>::iterator it;
    it = std::find(edges.begin(), edges.end(), L);
    
    Link* out = it;
    
    edges.erase(it);
    
    return out;

}

template <class T>
Node<T>* Graph::GetNode(T data)
{
    std::vector< Node<T>* >::iterator it;
    it = std::find(nodes.begin(), nodes.end(), A);

    if(it == nodes.end())
        return NULL;
    return it;
}

template <class T>
void Graph::Save(string path)
{

}

//BFS check if two nodes are connected
template <class T>
bool Graph::Connected(Node<T>* A, Node<T>* B)
{
    std::queue< Node<T>* > Q;
    std::map<T, bool> visited;
    std::map<T, bool>::iterator it;

    Node<T> *current;
    Q.push(A);

    while(Q.size!=0)
    {
        current = Q.pop();
        visited[current->data] = true;
        
        if(current == B)
        {
            return true;
        }

        //add unvisited nodes to the queue
        for(int i = 0; i < current->edges.size(); i++)
        {
            //if the value isn't in there, then add it to the queue
            it = visited.find(edges[i]->to.data);
            if(it == visited.end())
            {
                Q.push(edges[i]->to);
            }
        }
    }

    return false;
}

//create a minimum spanning tree of this graph
template <class T>
vector< Link<T> > Graph::MST()
{
    //algorithm...
    /*
        sort edges by weight
        while the list of edges isn't empty, remove an edge from the set
        if this edge connects two unconnected nodes, add it to the MST

    */
    vector< Link<T> > out;
    vector< Link<T> > tmp = edges;
    std::sort(tmp.begin(), tmp.end());

    std::queue< Link<T> > Q;

    for(int i = 0; i < tmp.size(); i++)
    {
        Q.push(tmp[i]);
    }

    while(Q.size()!=0)
    {
        Link<T> l = Q.pop();
        if(!Connected(l.to, l.from))
        {
            out.push_back(l);
        }
    }
    return out;
}




