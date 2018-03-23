#include "BSP.h"

Leaf::Leaf(int X, int Y, int W, int H)
{
	x = X;
	y = Y;
	width = W;
	height = H;
	
	left = NULL;
	right = NULL;
}

bool Leaf::Split(Floor* grid)
{
	//cout<<"Splitting.."<<std::endl;
	int seed = time(NULL);
	srand(seed);
	
	if(left!=NULL || right!=NULL) 
	{
		//cout<<"Not splitting..."<<std::endl;
		return false;
	}
	
	//determine direction of split
	bool hSplit = false;
	// if the width is >25% larger than height, we split vertically
    // if the height is >25% larger than the width, we split horizontally
    // otherwise we split randomly
	
	//cout<<float(width)/float(height)<<std::endl;
	//cout<<float(height)/float(width)<<std::endl;
	
	if(width > height && float(width)/float(height) >= 1.25)
	{
		hSplit = false;
	}
	else if(height > width && float(height)/float(width) >= 1.25)
	{
		hSplit = true;
	}
	else
	{
		(rand() > 0.5) ? true : false;
	}
	//hSplit = (width > height && width / height >= 1.25) ? false : true;
	//hSplit = (height > width && height / width >= 1.25) ? true : hSplit;
	//(rand() > 0.5) ? true : false;
	
	int max = (hSplit ? height: width) - MINLEAFSIZE;
	
	if(max <= MINLEAFSIZE)
		return false;
	
	int difference = max - MINLEAFSIZE;
	//cout<<"The difference is " << difference<<std::endl;
	//cout<<MINLEAFSIZE<<std::endl;
	int split = rand() % (difference) + MINLEAFSIZE; //RNG between MINLEAFSIZE and max
	//cout<<"Split size is.. " <<split<<std::endl;
	
	
	
	if(hSplit)
	{
		left = new Leaf(x, y, width, split);
		//cout<<y<<" plus "<<split<<" is "<<(y+split)<<std::endl;
		right = new Leaf(x, y + split, width, height-split);
		for(int i = x; i < x+width; i++)
		{
			grid->_map[i][y+split] = 2;
		}
	}
	else
	{
		left = new Leaf(x, y, split, height);
		//cout<<x<<" plus "<<split<<" is "<<(x+split)<<std::endl;
		right = new Leaf(x + split, y, width-split, height);
		for(int i = y; i < y+height; i++)
		{
			grid->_map[x+split][i] = 2;
		}
	}
	
	//cout<<(*grid);
	
	//cout<<"Successfully split"<<std::endl;
	return true;
}

//recursive function to create the rooms
void Leaf::CreateRooms(Floor* grid)
{
	//cout<<"Creating Rooms..."<<std::endl;
	
	if( left!=NULL || right!=NULL)
	{
		if(left!=NULL)
		{
			left->CreateRooms(grid);
		}
		
		if(right!=NULL)
		{
			right->CreateRooms(grid);
		}
	}
	//leaf is ready to make a room
	else
	{
		cout<<"Generating room!"<<std::endl;
		
		int seed = time(NULL);
		srand(seed);
		//cout<<"Generating room!"<<std::endl;
		int roomSizeX = rand() % (width-2) + 3; //(Registry.randomNumber(3, width - 2), Registry.random
		int roomSizeY = rand() % (height-2) + 3;
		/*
		// place the room within the Leaf, but don't put it right 
		// against the side of the Leaf (that would merge rooms together)
		roomPos = new Point(Registry.randomNumber(1, width - roomSize.x - 1), Registry.randomNumber(1, height - roomSize.y - 1));
		room = new Rectangle(x + roomPos.x, y + roomPos.y, roomSize.x, roomSize
		
		*/
		//cout<<"Generating room!"<<std::endl;
		
		int xPos, yPos;
		
		//cout<<"RoomSizeX " <<roomSizeX << " RoomSizeY "<<roomSizeY<<std::endl;
		
		xPos = rand() % (width - roomSizeX - 1) + 1;
		yPos = rand() % (height - roomSizeY - 1) + 1;
		cout<<"What's with the RNG?"<<std::endl;
		//create a new room
		//==================================================================TO DO 
		//for now just change the tile values to see if it works
		
		for(int i = xPos; i < xPos + roomSizeX; i++)
		{
			for(int j = yPos; j < yPos + roomSizeY; j++)
			{
				//cout<<x<<" "<<y<<std::endl;
				grid->_map[x + i][y + j] = 0;
			}
		}
		
		//cout<<(*grid);
		
	}
}

//call from head
void Leaf::Generate(Floor* grid, int minSize, int maxSize)
{
	if(minSize < 2)
	{
		cout<<"Min size too small."<<std::endl;
		return;
	}
	int MAXLEAFSIZE = maxSize;
	MINLEAFSIZE = minSize;
	
	//vector of all the leaves
	Leaf* root = new Leaf(0, 0, grid->GetWidth(), grid->GetHeight());
	vector<Leaf*> leaves;
	leaves.push_back(root);
	bool split = true;
	
	while(split)
	{
		cout<<"Split loop, let's go!"<<std::endl;
		split = false;
		//optimize this, probably use a queue instead
		for(int i = 0; i < leaves.size(); i++)
		{
			if(leaves[i]->left == NULL && leaves[i]->right == NULL)
			{
				if(leaves[i]->width > MAXLEAFSIZE || leaves[i]->height > MAXLEAFSIZE)
				{
					if(leaves[i]->Split(grid))
					{
						leaves.push_back(leaves[i]->right);
						leaves.push_back(leaves[i]->left);
						split = true;
					}
				}
			}
		}
	}
	
	cout<<"Done splitting loop!"<<std::endl;
	cout<<(*grid);
	root->CreateRooms(grid);
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
	Delaunay<int> triangulation;
	
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


