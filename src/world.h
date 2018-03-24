/*
	contains constructor, taking the number of floors as an argument
	an rng to randomly generate a layout for each floor
	draw function
	delete function to clear itself

	Contains the code for the World and the individual floors
	The world contains each of the floors
	The floor contains the layout of the floor along with the enemies, items, npcs on
	said floor
	
	perhaps instead of going floor by floor, maybe room by room, and rename World to Floor?
	
	Author: Ferrohound
*/
#if !defined(WORLD_H)
#define WORLD_H

#include <string.h>
#include <string>
#include <stdio.h>
#include <stdlib.h>     /* srand, rand */
#include <iostream>
#include <fstream>
#include <vector>
#include <queue>

#include <cmath>
#include <algorithm> /*std::find(vector.begin(), vector.end(), item)!=vector.end())*/

#include <time.h>
//#include <utility> /*std::pair, std::make_pair */

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



struct Tile{
	Tile();
	Tile(int _x, int _y);
	//Tile(Vector2 position);
	~Tile();

	//Vector2 pos;
	int x, y;
};

//============================ROOM============================================
//rename to region?

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
		
	
	private:
		
};

//============================FLOOR============================================
//each floor takes care of its own data, the world contains pointers to each floor
//and probably whatever enemies exist on it to keep consistency

//rename to grid?
class Floor{
	public:
		Floor(int width, int height, int fillPercentage = 55, bool useRandom = false, bool connect = true);
		
		void Generate(int fillPercentage, bool useRandomSeed, int seed, int smoothing, bool connect = true);
		bool InMapRange(int x, int y);
		vector<Tile> GetRegionTiles(int sx, int sy);
		
		vector< vector<Tile> > GetRegions (int tileType);
		
		//is the given position an outline tile
		bool IsOutlineTile(int x, int y);
		
		//remove a wall or room region that is below a certain threshold and connect
		//rooms
		void ProcessMap(bool connect = true);
		void RandomFillMap(bool useRandomSeed, int seed, int fillPercentage);
		int GetSurroundingWallCount(int sx, int sy);
		int GetAvgWallValue(int sx, int sy, int radius = 1);
		void SmoothMap();
		
		//modify rooms to make them more mountainous, etc..
		void ProcessRooms(int max = 1, int min = 1, int smoothing = 1);
		void SmoothRoom(Room* room);
		
		void ConnectClosestRooms(vector<Room*> rooms, bool forceAccessibility = false);
		void ConnectRooms(Room* A, Room* B);
		void CreatePassage(Room* A, Room* B, Tile tA, Tile tB);
		void DrawCircle(Tile t, int r);
		
		vector<Tile> GetLine(Tile start, Tile end);
		
		//save a floor and a load a floor from a given path
		void LoadFloor(string path);
		void SaveFloor(string path);
		
		void LoadFloor();
		void SaveFloor();
		
		void Clear();
		
		vector < vector<int> > GetMap();
		
		int GetWidth() { return _width; }
		int GetHeight() { return _height; }
		
		//this doesn't work for some reason
		friend std::ostream &operator<<( std::ostream &output, const Floor &F )
		{
			output << "Floor is : "<<std::endl;
			for(int i = 0; i < F._width ; i++)
			{
				for(int j = 0; j < F._height ; j++)
				{
					output << F._map[i][j];
				}
				output << '\n';
			}
			return output; 
		}
		
		std::vector< vector<int> > _map;
		std::vector< Room* > _rooms;
		std::vector< Room* > _spaces;
		bool debug;
	private:
		int _width, _height;
};

//========================World=================================================
//contains the floors, total number of enemies(?), difficulty, etc...
/*class World{
	public:
		World(const string& name = "Earth", int floors=5, int difficulty = 1);
		~World();
		//method to delete each floor, free up memory
		void clear();
		void print();
		void update();
		void move();
		
		Floor* getFloor(int index);
	
	private:
		string _name;
		int _floors, _difficulty, _current;
		Floor ** _data;
		
		//method to generate however many floors are required
		void generate();
};*/


#endif

