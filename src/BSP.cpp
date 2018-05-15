#include "BSP.h"

//initialize room vector, probably not doing this right..
vector<Leaf*> Leaf::leaves = vector<Leaf*>();

Leaf::Leaf(int X, int Y, int W, int H)
{
	/*if(leaves == NULL)
	{
		leaves = vector<Room*>();
	}*/

	x = X;
	y = Y;
	width = W;
	height = H;
	
	debug = true;
	
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
	
	//cout<<(*grid);
	//this is a leaf node
	leaves.push_back(this);
	//cout<<"Successfully split"<<std::endl;
	return true;
}

//recursive function to create the rooms
void Leaf::CreateRooms(Floor* grid)
{
	//cout<<"Creating Rooms..."<<std::endl;
	
	//not at the root room yet, go deeper
	//maybe add a list of leaf notdes so we know which leaves are the rooms without having to create this list
	//later
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
		//cout<<"Generating room!"<<std::endl;
		
		//fill with more interesting rooms========================= TO DO
		
		int seed = time(NULL);
		srand(seed);
		
		int roomSizeX = rand() % (width-4) + 3; //(Registry.randomNumber(3, width - 2), Registry.random
		int roomSizeY = rand() % (height-4) + 3;
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
				if(grid->InMapRange(x+1, y+j))
					grid->_map[x + i][y + j] = 0;
			}
		}
		
		room = new Room(x + xPos, y + yPos, roomSizeX, roomSizeY, grid->_map);
		grid->_rooms.push_back(room);
		
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
	
	/*if(debug)
		cout<<(*grid);*/
	
	root->CreateRooms(grid);
	
	//get the edges and make that sweet sweet graph
	vector< Edge<int> > edges = TriangulateEdges(root);

	//================================================================== TO DO
	//need a function to turn that vector of edges into a vector of Links
	Graph<Room*> G = Graph<Room*>();
	//populate graph
	for(int j = 0; j < leaves.size(); j++)
	{
		Node<Room*> n = Node<Room*>();
		n.data = leaves[j]->room;
		G.AddNode(&n);
	}

	vector< Link<Room*> > halls = GetHalls(&G, edges); 

	//=================================================================== TO DO
	
	//get MST; add some percentage of the remaining edges to the tree
	vector< Link<Room*> > MST = G.MST();
	vector< Link<Room*> > tmp = halls;
	std::sort(tmp.begin(), tmp.end());
	//say, the final tree must be at least 60% of the original
	//randomize which links get re-added later..
	while (float(MST.size())/float(halls.size()) < 0.6)
	{
		MST.push_back(tmp[MST.size()]);
	}
	
	//====================================================================
	
	//pass the edges in the graph to ConnectRooms to draw the paths
	DrawHallways(grid, halls);
	
}

//===================================================== TO DO
//improve this, for this shall be awful
vector< Link <Room*> > Leaf::GetHalls(Graph<Room*> *g,  vector< Edge<int> > e )
{
	vector< Link<Room*> > out = vector< Link<Room*> >();

	for(int i = 0; i < e.size(); i++)
	{
		Room* R1, *R2;
		Node<Room*> *N1, *N2;
		//need to have the nodes from the graph as well
		for(int j = 0; j < leaves.size(); j++)
		{
			int midX = (leaves[j]->room->tiles[0].x + leaves[j]->room->tiles[leaves[j]->room->tiles.size()-1].x)/2;
			int midY = (leaves[j]->room->tiles[0].y + leaves[j]->room->tiles[leaves[j]->room->tiles.size()-1].y)/2;

			if(e[i].p1.x == midX && e[i].p1.x == midX)
			{
				R1 = leaves[j]->room;
				N1 = g->GetNode(R1);

				//find the corresponding node from the graph
			}

			if(e[i].p2.x == midX && e[i].p2.x == midX)
			{
				R2 = leaves[j]->room;
				N2 = g->GetNode(R2);
			}
		}

		Link<Room*> L = Link<Room*>();
		L.to = N1;
		L.from = N2;
		out.push_back(L);
	}

	return out;
}

//draw halls from the given edges
void Leaf::DrawHallways(Floor* grid, vector< Link<Room*> > edges)
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

vector< Vector2<int> > GetMidpoints(vector< Room* > rooms)
{
	//rooms created with the Leaf class will have the upper left tile as the first
	//and lower right as the last
	
	vector< Vector2<int> > out;
	
	for(int i = 0; i < rooms.size(); i++)
	{
		int midX = (rooms[i]->tiles[0].x + rooms[i]->tiles[rooms[i]->tiles.size()-1].x)/2;
		int midY = (rooms[i]->tiles[0].y + rooms[i]->tiles[rooms[i]->tiles.size()-1].y)/2;
		
		out.push_back(Vector2<int>(midX, midY));
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



vector< Edge<int> > Leaf::TriangulateEdges(Leaf* head)
{
	//================================================================== TO DO 
	/*
	
		function for getting all the rooms in a particular sector may
		be more useful in the long run
	
	*/
	//====================================================================
	
	//get the midpoints from all of the rooms from function
	std::vector<Vector2<int>> points = GetMidpoints(leaves);
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


