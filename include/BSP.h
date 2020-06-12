//binary space partition for the dungeon rooms
//see https://gamedevelopment.tutsplus.com/tutorials/how-to-use-bsp-trees-to-generate-game-maps--gamedev-12268

#if !defined(BSP_H)
#define BSP_H

#include <vector>

#include "floor.h"
#include "Graph.h"

//for triangulation, move this elsewhere when the time comes
#include "vec.h"

using std::vector;

//split this up into leaf and BSP_Tree at some point

class Leaf
{
	public:
		Leaf(int X, int Y, int W, int H);
		bool Split(Floor* grid, bool debug = false);
		void CreateRooms(Floor* grid, bool debug = false);
		static Leaf* Generate(Floor* grid, int minSize, int maxSize, bool debug = false);

		void DrawHallways(Floor* grid, vector< Link <Room> > edges);

		vector< Link <Room> > GetHalls( Graph<Room> *g, vector< Edge<int> > e);
		vector< Room* > GetRooms(Leaf* head);
		void ConnectRooms(Floor* grid, vector< Room* > rooms);
	
		Leaf* left;
		Leaf* right;
	
		Room* room;

		static vector< Leaf* > LeafNodes;
		static int MINLEAFSIZE;
		
	private:
		int x, y, width, height;
};


#endif