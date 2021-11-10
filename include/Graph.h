/*
	Graph class
    directed v undirected
*/

#pragma once

#include <vector>
#include <string>
#include <iostream>
#include <algorithm> /* for std::find, std::sort */
#include <queue>          // std::queue
//for MST
#include <map>
#include <bits/stdc++.h>  //for INT_MAX
#include <unordered_map>

//Dijkstra implementation
#include "PriorityQueue.h"

using std::string;
using std::vector;
using std::cout;


template <class T>
struct Node;

template <class T>
class Edge;


template <class T>
struct AdjNode
{
    Node<T>* node;
    float weight;
    struct AdjNode* next;
};

template <class T>
struct AdjList
{
    struct AdjNode<T> *head; 

    Node<T>* Remove(Node<T>* A)
    {
        AdjNode<T>* prev = head;
        AdjNode<T>* tmp = prev->next;

        while(tmp)
        {
            if(tmp->node == A)
            {
                prev->next = tmp->next;
                return tmp->node;
            }
            prev = tmp;
            tmp = tmp->next;
        }
    }

    void Add(Node<T>* A, float w)
    {
        AdjNode<T>* N = new AdjNode<T>();
        N->node = A;
        N->weight = w;

        if(head == NULL)
            head = N;
        else
        {
            N->next = head;
            head = N;
        }
    }
};

template <class T>
struct AdjGraph
{
	int V;
    struct AdjList<T>* array;
};


template <class T>
struct Node
{
    int dest;
    T* data;
    
    friend bool operator== (const Node& n1, const Node& n2)
    {
        return (n1->data == n2->data);
    }
};

template <class T>
struct Edge
{
    Node<T>* to;
    Node<T>* from;
    float weight;

    friend bool operator== (const Edge &L1, const Edge &L2)
    {
        return (L1.from->data == L2.from->data && L1.to->data == L2.to->data);
    }

    bool operator < (const Edge& L) const
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
        Graph<T>()
        {
            edges = vector< Edge<T> >();
            nodes = vector< Node<T>* >();
        }

        Graph<T>( vector<T> d )
        {
            for(auto& i: d)
            {
                AddNode(&i);
            }
        }

        Graph<T>( vector< Node<T>* > nds)
        {
            for(auto& n : nds)
            {
                AddNode(n);
            }
        }

        Graph<T>( vector< Edge<T> > e )
        {
            edges = e;

            for(auto& edge: e)
            {
                //AddNode(&i);
                if (!( data.find(edge.from) != data.end() ) )
                {
                    AddNode(edge.from);
                }
                if(!( data.find(edge.to) != data.end() ) )
                {
                    AddNode(edge.to);
                }
                AddEdge(edge);
            }
        }


        void AddEdge( Node<T>* A, Node<T>* B, float weight = 1 )
        {
            /*if( data.find(A) != data.end() )
            {
                data[A].Add(B, weight);
            }*/

            Edge<T> tmp = Edge<T>();
            tmp.to = A;
            tmp.from = B;
            tmp.weight = weight;

            AddEdge(tmp);
        }

        void AddEdge( Edge<T> E )
        {
            if(data.find(E.from) != data.end() )
            {
                data[E.from].Add(E.to, E.weight);
            }
            edges.push_back(E);
        }

        void AddNode(T data)
        {
            Node<T> *tmp = new Node<T>();
            tmp->data = &data;
            //nodes.push_back(tmp);
            AddNode(tmp);
        }

        void AddNode(T* data)
        {
            Node<T> *tmp = new Node<T>();
            tmp->data = data;
            //std::cout<<tmp->data<<" vs "<<data<<std::endl;
            //nodes.push_back(tmp);
            AddNode(tmp);
        }

        void AddNode(Node<T>* A)
        {
            nodes.push_back(A);
            data[A] = AdjList<T>();
        }

        void RemoveNode(Node<T>* A)
        {
            if(Size() == 0)
            {
                return;
            }
            data.erase(A);
        }

        void RemoveEdge(Edge<T> L)
        {
            if(edges.size() == 0)
            {
                return;
            }

            auto it = std::find(edges.begin(), edges.end(), L);
            edges.erase(it);

            data[L.from].Remove(L.to);
        }

        //Dijkstra implementation
        int Distance(Node<T>* start, Node<T>* finish)
        {
            if(start == finish)
                return 0;

            std::unordered_map<Node<T>* , Node<T>* > prev;
            std::unordered_map<Node<T>* , int> dist;

            Heap<Node<T>*, int> Q;

            //initialize the distance to all nodes as infinite
            for(auto& N : nodes)
            {
                dist[N] = INT_MAX;
                prev[N] = NULL;
                Q.Push(N, dist[N]);
            }

            dist[start] = 0;

            while(!Q.IsEmpty())
            {
                Node<T>* U = Q.Pop();

                AdjNode<T>* N = data[U].head;
                while(N)
                {
                    int alt = dist[U] + N->weight;
                    if(alt < dist[N->node])
                    {
                        dist[N->node] = alt;
                        prev[N->node] = U;
                        Q.UpdatePriority(N->node, alt);
                    }
                    N = N->next;
                }

                /*
                OLD
                for(auto& Edge : U->edges)
                {
                    int alt = dist[U] + Edge.weight;
                    if(alt < dist[Edge.to])
                    {
                        dist[Edge.to] = alt;
                        prev[Edge.to] = U;
                        Q.UpdatePriority(Edge.to, alt);
                    }
                }
                */
            }

            

            int output = dist[finish];
            //unconnected, return -1
            if(output == INT_MAX) output = -1;
            return output;
        }

        //vector< Edge<T> > MST()
        Graph<T> MST()
        {
            Graph<T> output = Graph<T>(nodes);
            //return the new graph
            //algorithm...
            /*
                sort edges by weight
                while the list of edges isn't empty, remove an edge from the set
                if this edge connects two unconnected nodes, add it to the MST

            */

            //vector< Edge<T> > out;
            vector< Edge<T> > tmp = edges;
            std::sort(tmp.begin(), tmp.end());

            /*std::cout<<"Sanity check.."<<std::endl;
            for(auto& e : tmp)
            {
                std::cout<<e.weight<<"->";
            }
            std::cout<<std::endl;*/

            //need a fresh set of nodes to make this work..

            //Heap<Node<T>*, int> Q;
            std::queue< Edge<T> > Q;

            for(int i = 0; i < tmp.size(); i++)
            {
                Q.push(tmp[i]);
            }

            while(Q.size()!=0)
            {
                Edge<T> l = Q.front();
                Q.pop();

                if(!output.Connected(l.to, l.from) && !output.Connected(l.from, l.to))
                {
                    //out.push_back(l);
                    //add the edges to the MST_Edges vector
                    //l.to->MST_Edges.push_back(l);
                    //l.from->MST_Edges.push_back(l);
                    //cout<<"Added Edge"<<std::endl;
                    output.AddEdge(l);
                }
            }

            //return out;
            std::cout<<"From original "<<GetEdges().size()<<" edges ";
            std::cout<<"created MST which has "<<output.GetEdges().size()<<" edges."<<std::endl;
            return output;
        }

        bool Connected(Node<T>* A, Node<T>* B)
        {
            return (Distance(A, B) != -1);
        }
        
        Edge<T>* PopEdge( Edge<T> L)
        {
            auto it = std::find(edges.begin(), edges.end(), L);
            
            Edge<T>* out = it;
            
            edges.erase(it);
            
            return out;
        }

        Node<T>* GetNode(T* data)
        {
            //std::cout<<"Looking for: "<<data<<std::endl;
            for(int i = 0; i < nodes.size(); i++)
            {
                //std::cout<<nodes[i]->data<<std::endl;
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

        vector< Edge<T> > GetEdges()
        {
            return edges;
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
        vector< Edge<T> > edges;
        vector< Node<T>* > nodes;
        std::unordered_map< Node<T>*, AdjList<T> > data;
};