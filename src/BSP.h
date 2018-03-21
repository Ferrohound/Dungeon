//binary space partition for the dungeon rooms
//see https://gamedevelopment.tutsplus.com/tutorials/how-to-use-bsp-trees-to-generate-game-maps--gamedev-12268
#include <vector>

#include "world.h"

using std::vector;

class Leaf
{
	public:
		Leaf(int X, int Y, int W, int H);
		bool Split();
		void CreateRooms();
	
	
	private:
		int x, y, width, height;
		int MINLEAFSIZE = 6;
		Leaf left, right;
		Room room;
};