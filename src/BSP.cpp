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
	int seed = time(NULL);
	srand(seed);
	
	if(left!=null || right!=null) return false;
	
	//determine direction of split
	bool hSplit = false;
	//change this later
	hSplit = (width > height && width / height >= 1.25) ? true : false;
	
	int max = (hSplit ? height: width) - MINLEAFSIZE;
	
	if(max <= MINLEAFSIZE)
		return false;
	
	int difference = max - MINLEAFSIZE;
	int split = rand() % (difference) + MINLEAFSIZE; //RNG between MINLEAFSIZE and max
	
	left = new Leaf(x, y, width, split);
	
	if(hSplit)
		right = new Leaf(x, y + split, width, height-split);
	else
		right = new Leaf(x + split, y, width-split, height);
	
	return true;
}

//recursive function to create the rooms
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
		int seed = time(NULL);
		srand(seed);
	
		int roomSizeX = rand() % (width-2) + 3; //(Registry.randomNumber(3, width - 2), Registry.random
		int roomSizeY = rand() % (height-2) + 3;
		/*
		// place the room within the Leaf, but don't put it right 
		// against the side of the Leaf (that would merge rooms together)
		roomPos = new Point(Registry.randomNumber(1, width - roomSize.x - 1), Registry.randomNumber(1, height - roomSize.y - 1));
		room = new Rectangle(x + roomPos.x, y + roomPos.y, roomSize.x, roomSize
		
		*/
		
		int xPos, yPos;
		xPos = rand() % (width - roomSizeX - 1) + 1;
		yPos = rand() % (height - roomSizeY - 1) + 1;
		
		//create a new room
		//==================================================================TO DO 
		
	}
}

//call from head
void Leaf::Generate(Leaf* head)
{
	int MAXLEAFSIZE = 20;
	//vector of all the leaves
	vector<Leaf*> leaves;
	leaves.push_back(head);
	bool split = true;
	
	while(split)
	{
		split = false;
		//optimize this, probably use a queue instead
		for(int i = 0; i < leaves.size(); i++)
		{
			if(leaves[i]->left == null && leaves[i]->right == null)
			{
				if(leaves[i]->width > MAXLEAFSIZE || leaves[i]->height > MAXLEAFSIZE)
				{
					if(leaves[i]->Split())
					{
						leaves.push_back(leaves[i]->right);
						leaves.push_back(leaves[i]->left);
						split = true;
					}
				}
			}
		}
	}
}

vector< Edge<int> > Leaf::TriangulateEdges()
{
	//get the midpoints from all of the rooms from function
	//================================================================== TO DO 
	/*
	
		function for getting all the rooms
		function for getting all the midpoints from those rooms as 
		point structures
	
	*/
	//====================================================================
	
	std::vector<Vector2<int>> points;
	Deluanay<int> triangulation;
	
	triangulation.triangulate(points);
	return triangulation.getEdges();
}

//fill map with 1s then iterate over each room & fill them up with 0s
//then create the halls and stuff
void Leaf::FillMap(Floor* floor)
{
	//getRooms
	//getEdges
	
	vector< Edge<int> > edges;
	//need a way to translate those edges to rooms?
	//maybe, maybe not.. just do a check to see if the current tile is 1
	//or not
	
}


