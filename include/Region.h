/*
	A region/room is simply a collection (typically the same type) of tiles 
	(more accurately a collection of positions)

*/

#ifndef REGION_H
#define REGION_H
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

using std::cout;
using std::cin;
using std::endl;

using std::string;
using std::vector;
using std::queue;

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
		Room(int x, int y, int width, int height, vector< vector<Tile*> > map, Tile* fill);
		Room(vector<std::pair<int, int>> _tiles, vector< vector<Tile*> > map, Tile* fill, int _id=-1);
		~Room();
		
		//member functions
		void SetAccessibleFromMainRoom();
		bool isConnected(Room* A);
		static int CompareRooms(Room* A, Room* B);
		
		bool operator==(const Room &other) const;
		
		static void ConnectRooms(Room* A, Room* B);
		
		
		//member varialbes
		vector<std::pair<int, int>> tiles;
		vector<std::pair<int, int>> border;
		Tile* tileType;
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


// Helper functions
void ConnectRooms(Grid* grid, Room* A, Room* B, bool angular, Tile* fill);
void AddRoom(Grid* grid, Room* room, Tile* fill);
void DrawCircle(Grid* grid, std::pair<int, int> t, int r, Tile* fill);
vector<std::pair<int, int>> GetLine(std::pair<int, int> start, std::pair<int, int> end);
void CreatePassage(Grid* grid, Room* A, Room* B, std::pair<int, int> tA, std::pair<int, int> tB, Tile* fill);

#endif