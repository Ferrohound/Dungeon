#include "PseudoPhysics.h"

#pragma region rs_methods
//================================== ROOM SYSTEM METHODS ================================
// might want to cap out the size of the room for whatever reason later on so keep min and maxS

Grid<int> *PhysicsSystem::Generate(Grid<int> *grid, int fill, bool dense, int minS, int maxS, int numSteps)
{
	_grid = grid;
	int med, numRooms, fillVolume;

	med = _grid->GetWidth() * _grid->GetWidth();
	fillVolume = (int)(med * ((float)fill / 100.0));

	if (minS == -1 || maxS == -1)
	{
		std::cout << "Sqrt = " << sqrt(fillVolume) << std::endl;
		med = sqrt(sqrt(fillVolume));
		minS = med - 5;

		(minS <= 0) ? minS = 4 : minS = minS;

		maxS = med + 5;

		std::cout << "Med: " << med << " MaxS " << maxS << " MINS " << minS << std::endl;
	}

	nodes.clear();
	cmX = cmY = 0;
	bounds.x = _grid->GetWidth() - 1;
	bounds.y = _grid->GetHeight() - 1;

	PopulateSystem(minS, maxS, _grid->GetWidth(), _grid->GetHeight(), fillVolume);

	for (int i = 0; i < numSteps; i++)
		Tick();

	for (auto &RN : nodes)
		AddRoomToFloor(RN);

	Graph<RoomNode> G = ConnectSystem();
	Graph<RoomNode> mst = G.MST();

	if (!dense)
	{
		cout << "Adding Cycles..." << std::endl;
		// AddCycles(mst);
	}
	else
	{
		cout << "Adding Dense Cycles..." << std::endl;
		AddDenseCycles(mst);
	}

	vector<Edge<RoomNode>> links;

	auto tmpLinks = mst.GetEdges();
	auto rooms = mst.GetNodes();

	// remove links with a weight of 0
	std::copy_if(tmpLinks.begin(), tmpLinks.end(), std::back_inserter(links), [](Edge<RoomNode> i)
				 { return i.weight > 0; });

	for (auto r : rooms)
	{
		auto region = r->data->GetRoom();
		int fillArray[] = {_fill};
		// region->border = _grid.GetRegionOutline(region, {_fill});
		region->border = _grid->GetRegionOutline(region, fillArray);
	}

	std::cout << "Final graph has " << links.size() << " edges" << std::endl;
	AddEdgesToFloor(links);

	Grid<int> *clone = _grid->Clone();

	return clone;
}

//===================== TO DO : free up memory properly ====================

void PhysicsSystem::PopulateSystem(int minSize, int maxSize, int dimX, int dimY, int maxFill)
{
	vector<std::pair<RoomNode *, int>> elements;
	vector<RoomNode *> tmp;

	int totalVolume = 0;

	while (totalVolume < maxFill * 2)
	{
		// get a random x, y  in the range of the room dimensions
		int s = minSize + rand() % (maxSize - minSize + 1);
		// keep the room in range
		int x = rand() % (dimX - s - 1) + 1;
		int y = rand() % (dimY - s - 1) + 1;

		RoomNode *r = new RoomNode(x, y, s);
		tmp.push_back(r);

		totalVolume += (s * s);
	}

	// sort based on fewest surrounding things
	for (int i = 0; i < tmp.size(); i++)
	{
		int surrounding = GetSurroundingNodes(tmp[i], tmp[i]->GetSize() * 2, tmp).size();
		elements.push_back(std::make_pair(tmp[i], surrounding));
	}

	// sort in ascending order
	std::sort(elements.begin(), elements.end(),
			  [](const std::pair<RoomNode *, int> &A,
				 const std::pair<RoomNode *, int> &B) -> bool
			  {
				  return A.second > B.second;
			  });

	for (int i = 0; i < tmp.size() / 2; i++)
	{
		nodes.push_back(elements[i].first);
	}

	std::cout << "Total of " << nodes.size() << " nodes." << std::endl;
}

void PhysicsSystem::Tick()
{
	CalculateCoM(cmX, cmY);

	for (int i = 0; i < nodes.size(); i++)
	{
		// should actually have to do with node's size
		AdjustNode(nodes[i], GetSurroundingNodes(nodes[i], 10, nodes));
	}
}

std::vector<RoomNode *> PhysicsSystem::GetSurroundingNodes(RoomNode *R, int distance, std::vector<RoomNode *> s)
{
	std::vector<RoomNode *> out;

	for (int i = 0; i < s.size(); i++)
	{
		if (vec2::VecDistance(R->GetPosition(), s[i]->GetPosition()) < distance)
			out.push_back(s[i]);
	}

	return out;
}

//==================== to do =========================
// apply the vector of the surrounding nodes to the main node
// maybe apply the "elastic" model later on if it isn't as nice

void PhysicsSystem::AdjustNode(RoomNode *node, std::vector<RoomNode *> surrounding)
{
	for (auto &n : surrounding)
	{
		vec2 pos = node->GetPosition();
		vec2 dir = n->GetPosition() - pos;

		if (dir.magnitude() != 0)
			dir = dir.normalize();

		dir.scale(n->GetSize());

		pos = pos - dir;

		if (pos.x + node->GetSize() > bounds.x)
			pos.x = bounds.x - node->GetSize();

		if (pos.x < 1)
			pos.x = 1;

		if (pos.y + node->GetSize() > bounds.y)
			pos.y = bounds.y - node->GetSize();

		if (pos.y < 1)
			pos.y = 1;

		node->SetPosition(pos);
	}

	vec2 endpos = node->GetPosition();
}

void PhysicsSystem::CalculateCoM(int &x, int &y)
{
	int M = 0, X = 0, Y = 0;
	int tx, ty;

	for (int i = 0; i < nodes.size(); i++)
	{
		vec2 pos = nodes[i]->GetPosition();
		X += pos.x * nodes[i]->GetSize();
		Y += pos.y * nodes[i]->GetSize();

		M += nodes[i]->GetSize();
	}

	x = std::floor((float)X / (float)M);
	y = std::floor((float)Y / (float)M);
}

// create a graph out of the nodes, then connect all of them with the others

Graph<RoomNode> PhysicsSystem::ConnectSystem()
{
	Graph<RoomNode> G;

	// avoid this somehow..
	for (int j = 0; j < nodes.size(); j++)
	{
		G.AddNode(nodes[j]);
	}

	for (int i = 0; i < nodes.size() - 1; i++)
	{
		for (int j = i + 1; j < nodes.size(); j++)
		{
			Node<RoomNode> *A = G.GetNode(nodes[i]);
			Node<RoomNode> *B = G.GetNode(nodes[j]);

			if (A == NULL || B == NULL)
			{
				std::cout << "We have a problem here..." << std::endl;
				continue;
			}

			G.AddEdge(A, B,
					  vec2::VecDistance(
						  nodes[i]->GetPosition(),
						  nodes[j]->GetPosition()));
		}
	}

	return G;
}

// re-add edges to the graph
//========== TO DO : get a better factor for re-adding edges =================
// maybe something like if the distance is less than the shortest edge already attached
//  multiplied by some factor add it

void PhysicsSystem::AddCycles(Graph<RoomNode> &MST)
{
	for (int i = 0; i < nodes.size() - 1; i++)
	{
		vector<RoomNode *> sur = GetSurroundingNodes(nodes[i], nodes[i]->GetSize() * 2, nodes);
		for (auto &node : sur)
		{
			int distance = MST.Distance(MST.GetNode(nodes[i]), MST.GetNode(node));
			if (distance > nodes.size() / 2)
			{
				MST.AddEdge(MST.GetNode(nodes[i]), MST.GetNode(node), distance);
			}
		}
	}
}

// add cycles in a way that the floor is noisy in a way that is pleasing
// once no notion of how rooms and corridors are distinguished

void PhysicsSystem::AddDenseCycles(Graph<RoomNode> &MST)
{
	auto nodes = MST.GetNodes();
	for (int i = 0; i < nodes.size() - 1; i++)
	{
		for (int j = i + 1; j < nodes.size(); j++)
		{
			// some asinine factor here
			int distance = MST.Distance(nodes[i], nodes[j]);
			if (distance > nodes.size() / 2)
			{
				MST.AddEdge(nodes[i], nodes[j], distance);
			}
		}
	}
}

//======================= to do =========================

void PhysicsSystem::AddEdgesToFloor(vector<Edge<RoomNode>> &edges)
{
	for (auto &edge : edges)
	{
		_grid->ConnectRegions(edge.to->data->GetRoom(),
							  edge.from->data->GetRoom(), true, _fill);
	}
}

//================= to do ====================
// need to create room then add it to the floor

void PhysicsSystem::AddRoomToFloor(RoomNode *room)
{
	Region<int> *rm;
	vec2 pos = room->GetPosition();

	vector<Cell<int> *> tiles;

	for (int i = pos.x; i < pos.x + room->GetSize(); i++)
	{
		for (int j = pos.y; j < pos.y + room->GetSize(); j++)
		{
			if (_grid->GetCell(i, j))
				tiles.push_back(_grid->GetCell(i, j));
		}
	}

	vector<Cell<int> *> border;
	// create the border tiles list
	for (int i = 0; i < tiles.size(); i++)
	{
		for (int x = tiles[i]->pos.x - 1; x < tiles[i]->pos.x; x++)
		{
			for (int y = tiles[i]->pos.y - 1; y < tiles[i]->pos.y + 1; y++)
			{
				if ((x == tiles[i]->pos.x || y == tiles[i]->pos.y) && _grid->GetCell(x, y)->data == tiles[i]->data)
				{
					border.push_back(tiles[i]);
				}
			}
		}
	}

	rm = new Region<int>(tiles, border);
	rm->Fill(_fill);

	room->SetRoom(rm);
}

#pragma endregion rs_methods
