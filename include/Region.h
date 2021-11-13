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

//============================ REGION ============================================
//eventually implement different shapes
class Region{
	public:
		Region();
		Region(int x, int y, int width, int height, vector< vector<Tile*> > map, Tile* fill);
		Region(vector<std::pair<int, int>> _tiles, vector< vector<Tile*> > map, Tile* fill, int _id=-1);
		~Region();
		
		//member functions
		void SetAccessibleFromMainRegion();
		bool isConnected(Region* A);
		static int CompareRegions(Region* A, Region* B);
		
		bool operator==(const Region &other) const;
		
		static void ConnectRegions(Region* A, Region* B);
		
		
		//member varialbes
		vector<std::pair<int, int>> tiles;
		vector<std::pair<int, int>> border;
		Tile* tileType;
		vector<Region*> connectedRegions;
		
		int size;
		int id;
		bool mainRegion;
		bool accessible;

		//to stop me from going crazy..
		//better struct later ============================ TO DO
		int mX, mY;
		
	
	private:
		
};


// Helper functions
void ConnectRegions(Grid* grid, Region* A, Region* B, bool angular, Tile* fill);
void AddRegion(Grid* grid, Region* Region, Tile* fill);
void DrawCircle(Grid* grid, std::pair<int, int> t, int r, Tile* fill);
vector<std::pair<int, int>> GetLine(std::pair<int, int> start, std::pair<int, int> end);
void CreatePassage(Grid* grid, Region* A, Region* B, std::pair<int, int> tA, std::pair<int, int> tB, Tile* fill);

#endif