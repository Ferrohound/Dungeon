/*
	RoomSystem is the class for simulating physics like our target gif
		It works by deciding what volume of space to fill with rooms of various sizes
		calculating a min and max size

		The system is then populated by adding randomly sized rooms at random locations
		to the system until double the maximum fill size is reached

		The rooms are then sorted in ascending order based on how many other rooms are within
		a certain range of said room and the more crowded half is discarded

		The faux-physics system is then applied. 
		Over a number of loops, the center of mass of the system is calculated
		then for each node, a "force" vector is applied to that node from the nodes within its
		gravitational range

		The rooms are then added to the floor

		Next, we create a graph using the rooms as nodes and the distance between them
		as the edges
		We connect every node to every other node, then create a Minimum Spanning Tree
		to get the shortest paths from each node to the other in a way that all nodes
		are reachable

		If the floor is dense, we add many edges back, otherwise we just re-add a few
		We do this by going through each node and getting the surrounding nodes within a 
		certain radius
		For every surrounding node, if the distance is greater than the total number
		of nodes divided by two, re-add that edge

		For the dense version
		for every node i in the list of nodes
		we look at every node from i + 1 to the end and do the same thing

		This determining factor obviously needs to change, but it works for now...

		...somehow...

		The edges are then added to the graph in an angular fashion

*/
#pragma once

#include "NumTile.h"
#include "Graph.h"
#include "Region.h"
#include "vec.h"

using std::cout;
using std::cin;
using std::endl;

using std::string;
using std::vector;
using std::queue;

//external helper functions inside of MapGenerator
extern void RandomFillMap(Grid* grid, bool useRandomSeed, int seed, int fillPercentage, bool debug);
extern void PerlinFillMap(Grid* grid, int seed);
extern void SmoothRoom(Grid* grid, Region* room, int radius);

//=================================================================================
// To simulate physics to place the rooms

class RoomNode
{
	public:
		RoomNode(int x, int y, float size = 1) 
		{ _pos.x = x; _pos.y = y; _size = size; }

		float GetSize() { return _size; }
		vec2 GetPosition() { return _pos; }
		void SetPosition(vec2 newpos) { _pos = newpos; }

		Region* GetRoom() {return _r;}
		void SetRoom(Region* r) { _r = r; }
	private:
		float _size;
		vec2 _pos;
		Region* _r;
};


class RoomSystem
{
	public:
		RoomSystem(Tile* fill, Tile* empty): _fill(fill), _empty(empty) {}
		
		//eventually have it so ideal numRooms is calculated itself
		void Generate(Grid* grid, int fill = 10, bool dense = false, int minS = -1, int maxS = -1, int numSteps = 4);

		void PopulateSystem(int minSize, int maxSize, int dimX, int dimY, int maxFill);
		
		void Tick();
		std::vector<RoomNode*> GetSurroundingNodes(RoomNode* R, int distance, std::vector<RoomNode*> s);
		void AdjustNode(RoomNode* node, std::vector<RoomNode*> surrounding);
		void CalculateCoM(int& x, int& y);

		//connect the rooms in MST fashion, then create cycles to make
		//the room more interesting
		Graph<RoomNode> ConnectSystem();
		void AddCycles(Graph<RoomNode>& MST);
		void AddDenseCycles(Graph<RoomNode>& MST);

		//function to add all of the edges to the map
		void AddEdgesToFloor(Grid* grid, vector< Edge<RoomNode> >& edges);
		void AddRoomToFloor(Grid* grid, RoomNode* room);

	private:
		Tile* _fill;
		Tile* _empty;
		std::vector<RoomNode*> nodes;
		vec2 bounds;
		int cmX, cmY;
		Grid* _grid;
};

extern void SmoothRoom(Grid* grid, Region* room, int radius);