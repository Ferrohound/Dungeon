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



	MapSystem creates a more organic kind of map
		First we fill thhe map up with a random amount of noise
		
		We then do several steps of smoothing by going through every tile
		and changing its value based on what surrounds it

		Next, we process the map by getting the wall regions
		Any region with less than the threshold number of tiles in it is removed

		Same goes for the spaces

		The regions that aren't removed are added to a vector, which is then sorted
		by the size in descending order: The biggest room is our main room and it is
		set to accessible

		We then go about connecting the closest rooms via a recursive function that
		takes the grid, a list of rooms and a bool asking whether or not to force
		accessibility 

		initially our list of rooms is every other room we have and accessibility
		is set to false

		We're looking for the two best rooms to connect and the two best tiles
		one in each room to connect them through so we iterate over every combination
		of unconnected rooms and pick the one with the smallest distance between them

		Continue doing this until all rooms are accessible

	In all honesty, the map system could probably use the same MST method but it
	wouldn't look as natural

	
	Author: Ferrohound
*/
#pragma once

#include <string.h>
#include <string>
#include <stdio.h>
#include <stdlib.h>     /* srand, rand */
#include <iostream>
#include <fstream>
#include <vector>
#include <queue>
#include <unordered_map>

#include <cmath>
#include <algorithm> /*std::find(vector.begin(), vector.end(), item)!=vector.end())*/

#include <time.h>
//#include <utility> /*std::pair, std::make_pair */

#include "Grid.h"
#include "perlin_noise.h"
#include "Graph.h"
#include "vec.h"

using std::cout;
using std::cin;
using std::endl;

using std::string;
using std::vector;
using std::queue;

/*typedef struct Vector2{
	int x;
	int y;
} Vector2;*/


class Region{
	public:

	protected:
};

//============================ROOM============================================
//rename to region?
//eventually implement different shapes
class Room{
	public:
		Room();
		Room(int x, int y, int width, int height, vector< vector<int> > map);
		Room(vector<Tile> _tiles, vector< vector<int> > map, int _id=-1);
		~Room();
		
		//member functions
		void SetAccessibleFromMainRoom();
		bool isConnected(Room* A);
		static int CompareRooms(Room* A, Room* B);
		
		bool operator==(const Room &other) const;
		
		static void ConnectRooms(Room* A, Room* B);
		
		
		//member varialbes
		vector<Tile> tiles;
		vector<Tile> border;
		vector<Room*> connectedRooms;
		
		int size;
		int id;
		bool mainRoom;
		bool accessible;

		//to stop me from going crazy..
		//better struct later ============================ TO DO
		int mX, mY;
		
	
	private:
		
};

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

		Room* GetRoom() {return _r;}
		void SetRoom(Room* r) { _r = r; }
	private:
		float _size;
		vec2 _pos;
		Room* _r;
};

class RoomSystem
{
	public:
		RoomSystem() {}
		
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
		std::vector<RoomNode*> nodes;
		vec2 bounds;
		int cmX, cmY;
		Grid* _grid;
};

//======================================================================
//To create a map

class MapSystem
{
	public:
		MapSystem() { debug = false; perlin = false;}

		void setDebug(bool newval) { debug = newval; }
		void setPerlin(bool newval) { perlin = newval; }
		
		void Generate(Grid* grid, int fillPercentage = 30, bool useRandomSeed = true, int seed = 10, int smoothing = 2, bool connect = true);

		//remove a wall or room region that is below a certain threshold and connect
		//rooms
		void ProcessMap(Grid* grid, bool connect = true);
		void SmoothMap(Grid* grid);
		
		//modify rooms to make them more mountainous, etc..
		void ProcessRooms(Grid* grid, int max = 1, int min = 1, int smoothing = 1);
		
		void ConnectClosestRooms(Grid* grid, vector<Room*> rooms, bool forceAccessibility = false);
		//void ConnectRooms(Grid* grid, Room* A, Room* B, bool angular = false, int fill = 2);
		//void CreatePassage(Grid* grid, Room* A, Room* B, Tile tA, Tile tB, int fill = 2);
		//void DrawCircle(Grid* grid, Tile t, int r, int fill = 2);
		
		//vector<Tile> GetLine(Tile start, Tile end);


	private:
		std::vector< Room* > _rooms;
		std::vector< Room* > _spaces;
		bool debug, perlin;
		Grid* _grid;
};
