/*
	Graph class
    directed v undirected
*/

#include <vector>
#include <string>
#include <iostream>
#include <algorithm> /* for std::find, std::sort */
#include <queue>          // std::queue
//for MST
#include <map>

using std::string;
using std::vector;
using std::cout;

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


template <class T>
class Link;

//template?
template <class T>
struct Node
{
    int dest;
    vector< Link <T> > edges;
    //secondary link array for MST generation
    vector< Link<T> > MST_Edges;
    T* data;
    
    //============================================================= TO DO
    friend bool operator== (const Node& n1, const Node& n2)
    {
        //cout<<"LORD... "<<n1.data<<" "<<n2.data<<std::endl;
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
        //cout<<"HELLO? IS THIS THING ON?"<<std::endl;
       /* bool out = (L1.from == L2.from && L1.to == L2.to);
        cout<<out; */
        return (L1.from->data == L2.from->data && L1.to->data == L2.to->data);
    }

    //for sorting
    bool operator < (const Link& L) const
    {
        return (weight < L.weight);
    }
};

//since it's a template, implementation has to be in header
template <class T>
class Graph
{
    public:
        //================================functions
        Graph()
        {
            edges = vector< Link<T> >();
            nodes = vector< Node<T>* >();
            size = 0;
        }

        void AddEdge(Node<T>* A, Node<T>* B, int weight = 1)
        {
            Link<T> l = Link<T>();
            l.from = A;
            l.to = B;
            l.weight = weight;

            Link<T> l2 = Link<T>();
            l2.from = B;
            l2.to = A;
            l2.weight = weight;

            //=================================================== TO DO
            //get comparisons working, jeez..
            /*typename std::vector< Link<T> >::iterator it;
            //it = std::find(A->edges.begin(), A->edges.end(), l);
            it = std::find(edges.begin(), edges.end(), l);

            //i simply do not understand why this doesn't work.
            //this would cut down on the time dramatically, but whatever i guess.
            //if(it != A->edges.end())
            if(it != edges.end())
            {
                return;
            }*/

            for(int i = 0; i < edges.size(); i++)
            {
                if(l == edges[i] || l2 == edges[i])
                    return;
            }

            A->edges.push_back(l);
            B->edges.push_back(l2);

            edges.push_back(l);
            edges.push_back(l2);

            cout<<"EDGE ADDED."<<std::endl;
        }

        void AddEdge(Link<T> E)
        {
            /*typename std::vector< Link<T> >::iterator it;
            it = std::find(edges.begin(), edges.end(), E);

            //if the edge was found, return
            if(it != edges.end())
            {
                return;
            }*/

            //bi-directional for now
            Link<T> l2 = Link<T>();
            l2.from = E.to;
            l2.to = E.from;
            l2.weight = E.weight;

            for(int i = 0; i < edges.size(); i++)
            {
                if(E == edges[i] || l2 == edges[i])
                    return;
            }

            edges.push_back(E);
            E.to->edges.push_back(E);
            E.from->edges.push_back(l2);
            //cout<<"EDGE ADDED."<<std::endl;
        }

        void AddNode(T* data)
        {
            Node<T> *tmp = new Node<T>();
            tmp->data = data;
            nodes.push_back(tmp);
        }

        void AddNode(Node<T>* A)
        {
           /* typename std::vector< Node<T>* >::iterator it;
            it = std::find(nodes.begin(), nodes.end(), A);

            //if the node was found, return
            if(it != nodes.end())
            {
                return;
            }*/

            /*cout<<"ADDING NODE WITH DATA "<<A->data<<std::endl;
            cout<<"DATA ALREADY CONTAINED..."<<std::endl;
            for(int i = 0; i < nodes.size(); i++)
            {
                cout<<nodes[i]->data<<std::endl;
            }
            cout<<std::endl;

            for(int i = 0; i < nodes.size(); i++)
            {
                //if the node is present
                if(*A == *(nodes[i]))
                {
                    cout<<"Nodes are equal!"<<std::endl;
                    return;
                }
            }*/

            nodes.push_back(A);
            //cout<<"NODE ADDED WITH DATA "<<A->data<<std::endl;
            size++;
        }

        void RemoveNode(Node<T>* A)
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

        void RemoveEdge(Link<T> L)
        {
            if(edges.size() == 0)
            {
                return;
            }
            //logic to remove the edge from edges list
            //confused about this too...

            typename std::vector< Link<T> >::iterator it;
            it = std::find(edges.begin(), edges.end(), L);
            edges.erase(it);
        }

        //BFS check if two nodes are connected
        bool Connected(Node<T>* A, Node<T>* B, bool MSTE = false)
        {
            std::queue< Node<T>* > Q;
            typename std::map<T*, bool> visited;
            typename std::map<T*, bool>::iterator it;

            Node<T> *current;
            Q.push(A);

            while(Q.size()!=0)
            {
                //plz
                current = Q.front();
                Q.pop();

                visited[current->data] = true;
                
                if(*B == *current)
                {
                    return true;
                }

                //add unvisited nodes to the queue
                if(!MSTE)
                {
                    for(int i = 0; i < current->edges.size(); i++)
                    {
                        //if the value isn't in there, then add it to the queue
                        it = visited.find(current->edges[i].to->data);
                        if(it == visited.end())
                        {
                            Q.push(current->edges[i].to);
                        }
                    }
                }

                else
                {
                    for(int i = 0; i < current->MST_Edges.size(); i++)
                    {
                        //if the value isn't in there, then add it to the queue
                        it = visited.find(current->MST_Edges[i].to->data);
                        if(it == visited.end())
                        {
                            Q.push(current->MST_Edges[i].to);
                        }
                        it = visited.find(current->MST_Edges[i].from->data);
                        if(it == visited.end())
                        {
                            Q.push(current->MST_Edges[i].from);
                        }
                    }
                }
            }

            return false;
        }
        
        Link<T>* PopEdge( Link<T> L)
        {
            typename std::vector< Link<T> >::iterator it;
            it = std::find(edges.begin(), edges.end(), L);
            
            Link<T>* out = it;
            
            edges.erase(it);
            
            return out;
        }

        Node<T>* GetNode(T* data)
        {
           /* Node<T> tmp;
            tmp.data = data;

            typename std::vector< Node<T>* >::iterator it;
            it = std::find(nodes.begin(), nodes.end(), &tmp);

            if(it == nodes.end())
                return NULL;
            return (*it);*/
            //cout<<"LOOKING FOR "<<data<<std::endl;
            for(int i = 0; i < nodes.size(); i++)
            {
                //cout<<nodes[i]->data<<std::endl;
                if(nodes[i]->data == data)
                {
                    return nodes[i];
                }
            }

            return NULL;
        }

        vector< Node<T>* > GetNodes()
        {
            return nodes;
        }

        vector< Link<T> > MST()
        {
            //algorithm...
            /*
                sort edges by weight
                while the list of edges isn't empty, remove an edge from the set
                if this edge connects two unconnected nodes, add it to the MST

            */

           cout<<"CREATING MST OUT OF "<<edges.size()<<" EDGES."<<std::endl;

            vector< Link<T> > out;
            vector< Link<T> > tmp = edges;
            std::sort(tmp.begin(), tmp.end());

            //need a fresh set of nodes to make this work..
            //===================================================== TO DO

            std::queue< Link<T> > Q;

            for(int i = 0; i < tmp.size(); i++)
            {
                Q.push(tmp[i]);
            }

            //clear out the MST edges
            for(int i = 0; i < nodes.size(); i++)
            {
                nodes[i]->MST_Edges.clear();
            }

            while(Q.size()!=0)
            {
                //plz..
                Link<T> l = Q.front();
                Q.pop();

                if(!Connected(l.to, l.from, true) && !Connected(l.to, l.from, true))
                {
                    out.push_back(l);
                    //add the edges to the MST_Edges vector
                    l.to->MST_Edges.push_back(l);
                    l.from->MST_Edges.push_back(l);
                    //cout<<"Added Edge"<<std::endl;
                }
                /*else
                {
                   // cout<<"Dropped Edge"<<std::endl;
                }*/
            }
            return out;
        }

        //function to save an ascii and coordinates of a tile in the room/hallway
        //use floodfill to get the tiles in the room and rebuild
        void Save(string path);

        int Size()
        {
            return nodes.size();
        }

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
        vector< Link<T> > edges;
        vector< Node<T>* > nodes;
};