#include "BSP.h"

Leaf::Leaf(int X, int Y, int W, int H)
{
	x = x;
	y = y;
	width = W;
	height = H;
	
	left = null;
	right = null;
}

bool Leaf::Split()
{
	if(left!=null || right!=null) return false;
	
	//determine direction of split
	bool hSplit = false;
	//change this later
	hSplit = (width > height && width / height >= 1.25) ? true : false;
	
	int max = (hSplit ? height: width) - MINLEAFSIZE;
	
	if(max <= MINLEAFSIZE)
		return false;
	
	int split = 0 //RNG between MINLEAFSIZE and max
	
	left = new Leaf(x, y, width, split);
	
	if(hSplit)
		right = new Leaf(x, y + split, width, height-split);
	else
		right = new Leaf(x + split, y, width-split, height);
	
	return true;
	
	
}

void Leaf::CreateRooms()
{
	bool kill = false;
	if(left!=null)
	{
		left.CreateRooms();
		kill = true;
	}
	
	if(right!=null)
	{
		right.CreateRooms();
		kill = true;
	}
	
	if(kill)
		return;
	
	//leaf is ready to make a room
	else
	{
		int roomSize = 0; //(Registry.randomNumber(3, width - 2), Registry.random
		/*
		// place the room within the Leaf, but don't put it right 
		// against the side of the Leaf (that would merge rooms together)
		roomPos = new Point(Registry.randomNumber(1, width - roomSize.x - 1), Registry.randomNumber(1, height - roomSize.y - 1));
		room = new Rectangle(x + roomPos.x, y + roomPos.y, roomSize.x, roomSize
		
		*/
	}
}