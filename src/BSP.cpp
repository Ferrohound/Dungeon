#include "BSP.h"

/*
vector< Vector2<int> > GetMidpoints(vector< Room* > rooms)
{
	//rooms created with the Leaf class will have the upper left tile as the first
	//and lower right as the last
	
	vector< Vector2<int> > out;
	for(int i = 0; i < rooms.size(); i++)
	{
		//out.push_back(Vector2<int>(midX, midY));
		out.push_back(Vector2<int>(rooms[i]->mX, rooms[i]->mY));
	}
	return out;
}

//to save myself some trouble
vector <Vector2<int> > GetMidpoints(vector< Leaf* > leaves)
{
	vector<Room*> tmp = vector<Room*>();

	for(int i = 0; i < leaves.size(); i++)
	{
		tmp.push_back(leaves[i]->room);
	}

	return GetMidpoints(tmp);
}
*/

//initialize room vector, probably not doing this right..
vector<Leaf*> Leaf::LeafNodes = vector<Leaf*>();
int Leaf::MINLEAFSIZE = 11;

Leaf::Leaf(int X, int Y, int W, int H)
{
	/*if(leaves == NULL)
	{
		leaves = vector<Room>();
	}*/

	x = X;
	y = Y;
	width = W;
	height = H;
	
	left = NULL;
	right = NULL;
}


bool Leaf::Split(Floor* grid, bool debug)
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
		
		if(debug)
		{
			for(int i = x; i < x+width; i++)
			{
				grid->_map[i][y+split] = 2;
			}
		}
	}
	else
	{
		left = new Leaf(x, y, split, height);
		//cout<<x<<" plus "<<split<<" is "<<(x+split)<<std::endl;
		right = new Leaf(x + split, y, width-split, height);
		if(debug)
		{
			for(int i = y; i < y+height; i++)
			{
				grid->_map[x+split][i] = 2;
			}
		}
	}
	
	//LeafNodes.push_back(this);
	
	//cout<<"Successfully split"<<std::endl;
	return true;
}

//recursive function to create the rooms
void Leaf::CreateRooms(Floor* grid, bool debug)
{
	if (debug) std::cout<< "Creating room..." <<std::endl;
	//not at a leaf room yet, go deeper
	if( left!=NULL || right!=NULL)
	{
		if ( debug ) std::cout << "not at a leaf room yet, go deeper" <<std::endl;

		if(left!=NULL)
		{
			left->CreateRooms(grid, debug);
		}
		
		if(right!=NULL)
		{
			right->CreateRooms(grid, debug);
		}
	}
	//leaf is ready to make a room
	else
	{
		LeafNodes.push_back(this);
		//cout<<"Generating room!"<<std::endl;
		
		//fill with more interesting rooms========================= TO DO
		
		int seed = time(NULL);
		srand(seed);

		if (debug) std::cout<< "Generating room sizes for "
			<< "Width : " << width 
			<< " Height: " << height <<std::endl;
		
		int roomSizeX = rand() % (width - MINLEAFSIZE + 1) + 3; //(Registry.randomNumber(3, width - 2), Registry.random
		int roomSizeY = rand() % (height - MINLEAFSIZE + 1) + 3;

		if (debug) std::cout << "Room Size X: " << roomSizeX
			<< " Room Size Y: " << roomSizeY <<std::endl; 

		/*
		// place the room within the Leaf, but don't put it right 
		// against the side of the Leaf (that would merge rooms together)
		roomPos = new Point(Registry.randomNumber(1, width - roomSize.x - 1), Registry.randomNumber(1, height - roomSize.y - 1));
		room = new Rectangle(x + roomPos.x, y + roomPos.y, roomSize.x, roomSize
		*/
		
		int xPos = rand() % (width - roomSizeX - 1) + 1;
		int yPos = rand() % (height - roomSizeY - 1) + 1;
		//create a new room
		
		for(int i = xPos; i < xPos + roomSizeX; i++)
		{
			for(int j = yPos; j < yPos + roomSizeY; j++)
			{
				//cout<<x<<" "<<y<<std::endl;

				//if this room tile is within bounds, do the do
				if(grid->InMapRange(x+i, y+j))
					grid->_map[x + i][y + j] = 0;
			}
		}
		
		room = new Room(x + xPos, y + yPos, roomSizeX, roomSizeY, grid->_map);
		grid->_rooms.push_back(room);
	}
}

//call from head
Leaf* Leaf::Generate(Floor* grid, int minSize, int maxSize, bool debug)
{
	if(minSize < 2)
	{
		cout<<"Min size too small."<<std::endl;
		return NULL;
	}
	int MAXLEAFSIZE = maxSize;
	MINLEAFSIZE = minSize;
	
	//vector of all the leaves
	Leaf* root = new Leaf(0, 0, grid->GetWidth(), grid->GetHeight());
	vector<Leaf*> leaves;
	leaves.push_back(root);
	bool split = true;
	
	//split the place up
	while(split)
	{
		if(debug)
			cout<<"Split loop, let's go!"<<std::endl;
		split = false;
		//optimize this, probably use a queue instead
		for(int i = 0; i < leaves.size(); i++)
		{
			if(leaves[i]->left == NULL && leaves[i]->right == NULL)
			{
				if(leaves[i]->width > MAXLEAFSIZE || leaves[i]->height > MAXLEAFSIZE)
				{
					if(leaves[i]->Split(grid, debug))
					{
						leaves.push_back(leaves[i]->right);
						leaves.push_back(leaves[i]->left);
						split = true;
					}
				}
			}
		}
	}
	
	if(debug) cout<<"Done splitting loop!"<<std::endl;
	
	if(debug) cout<<(*grid);
	
	root->CreateRooms(grid, debug);
	if(debug) cout<<"Done Creating Rooms"<<std::endl;

	if(debug)	cout<<"Populating Graph..."<<std::endl;
	//================================================================== TO DO
	//need a function to turn that vector of edges into a vector of Links
	Graph<Room> G = Graph<Room>();
	//populate graph
	for(int j = 0; j < LeafNodes.size(); j++)
	{
		//cout<<"?? "<<LeafNodes[j]->room<<std::endl;
		Node<Room> n = Node<Room>();
		n.data = LeafNodes[j]->room;
		G.AddNode(LeafNodes[j]->room);
	}

	ConnectRooms(grid, GetRooms(this) );

	return root;
}

void Leaf::ConnectRooms(Floor* grid, vector< Room* > rooms)
{
	// put every room in queue of unreachable rooms;
	// while this queue is not empty:
		// get next room from queue;
		// if room is touching any number of halls:
			// make door, facing any avaliable hall;
			// put this room in queue of reachable rooms;
				// `continue`;
			// if room is touching any other reachable room:
				// connect this with other;
				// place door, if Random wants so;
				// `continue`;
			// put this room in queue of unreachable rooms;

	std::queue< Room* > UR; //unreachable rooms
	std::vector< Room* > RR; //reachable rooms
}

//draw halls from the given edges
void Leaf::DrawHallways(Floor* grid, vector< Link<Room> > edges)
{
	for(int i = 0; i < edges.size(); i++)
	{
		//draw that sweet, sweet angular path
		grid->ConnectRooms(edges[i].to->data, edges[i].from->data, true);
	}
}

//return a vector of all the rooms in this tree
vector< Room* > GetRooms(Leaf* head)
{
	vector<Room*> out;
	vector<Room*> in;
	
	if(head->left!=NULL)
	{
		in = GetRooms(head->left);
		out.insert(out.end(), in.begin(), in.end());
	}
	
	if(head->right!=NULL)
	{
		in = GetRooms(head->right);
		out.insert(out.end(), in.begin(), in.end());
	}
	
	if(head->room!=NULL)
	{
		out.push_back(head->room);
		//return out;
	}
	
	return out;
}

