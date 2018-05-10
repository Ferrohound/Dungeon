//binary space partition for the dungeon rooms
//see https://gamedevelopment.tutsplus.com/tutorials/how-to-use-bsp-trees-to-generate-game-maps--gamedev-12268

#if !defined(BSP_H)
#define BSP_H

#include <vector>

#include "world.h"

//for triangulation, move this elsewhere when the time comes
#include "vector2.h"
#include "triangle.h"
#include "delaunay.h"

using std::vector;

class Leaf
{
	public:
		Leaf(int X, int Y, int W, int H);
		bool Split(Floor* grid);
		void CreateRooms(Floor* grid);
		void Generate(Floor* grid, int minSize, int maxSize);
		
		//have a graph struct; create a MST from these edges & re-add some edges 
		//based on some heuristic
		vector< Edge<int> > TriangulateEdges(Leaf* head);
		void DrawHallways(vector< Edge<int> > edges);
		void FillMap(Floor* floor);
	
		Leaf* left;
		Leaf* right;
	
		Room* room;
		
	private:
		int x, y, width, height;
		int MINLEAFSIZE = 6;
		bool debug;
};


#endif