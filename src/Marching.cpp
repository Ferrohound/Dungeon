#include "Marching.h"

public struct Node
{
	glm::vec3 position;
	int vertexIndex = -1;
	
	Node(glm::vec3 pos)
	{
		position = pos;
	}
};

struct ControlNode : public Node
{
	bool active;
	Node above, right;
		
	ControlNode(glm::vec3 pos, bool act, float squareSize) : Node(pos) 
	{
		active = act;
		above = new Node(pos + glm::vec3(0, 1, 0) * squareSize/2f);
		right = new Node(pos + glm::vec3(1, 0, 0) * squareSize/2f);
	}
};

struct Triangle
{
	//vertex indices a b and c
	int A, B, C;
	
	Triangle(int a, int b, int c)
	{
		A = a;
		B = b; 
		C = c;
	}
	
	
	int& operator[] (const int i)
	{
		if (i == 0) return A;
		if (i == 1) return B;
		if (i == 2) return C;
		
		return -1;
	}
	
	bool Contains(int x)
	{
		return x == A || x == B || x == C;
	}
};


class Square
{
	/*
		A	t	B
		
		l		r
		
		D	d	C
	*/
	
	public:
		ControlNode A, B, C, D;
		Node t, r, l, b;
		int configuration;
			
		Square(ControlNode TL, ControlNode TR,
			ControlNode BL, ControlNode BR)
		
		{
			A = TL;
			B = TR;
			C = BR;
			D = BL;
			
			t = A.right;
			l = D.above;
			r = C.above;
			b = D.right;
			
			//configuration = 15;
			
			if(A.active)
				configuration+=8;
			if(B.active)
				configuration+=4;
			if(C.active)
				configuration+=2;
			if(D.active)
				configuration+=1;
		}
};


public class Grid
{
	public Square grid[][];
	
	public Grid(int map[][], float squareSize)
	{
		int nX = map.GetLength(0);
		int nY = map.GetLength(1);
		
		float mapWidth = nX * squareSize;
		float mapHeight = nY * squareSize;
		
		ControlNode* cNodes[nX][nY];/* = new ControlNode[nX, nY];*/
		
		for(int x = 0; x < nX ; x++)
		{
			for(int y = 0; y < nY ; y++)
			{
				glm::vec3 pos = glm::vec3(-mapWidth/2 + x * squareSize + squareSize/2, 0, 
					-mapHeight/2 + y * squareSize + squareSize/2);
				
				//each square on the map/grid is a control node, set active or 
				//inactive
				cNodes[x][y] = new ControlNode(pos, map[x][y] == 1, squareSize);
			}
		}
		
		//there will be n-1 squares in the grid
		grid = new Square[nX-1][ nY-1];
		
		for(int x = 0; x < nX-1 ; x++)
		{
			for(int y = 0; y < nY-1 ; y++)
			{
				grid[x][y] = new Square(cNodes[x][y+1], cNodes[x+1][y+1],
					cNodes[x][y], cNodes[x + 1][y]);
			}
		}
		
	}
};


//============================================================================

void CalculateOutline()
{
	for(int v = 0; v < verts.Count; v++)
	{
		if(!checkedVerts.Contains(v))
		{
			int newOutlineVert = GetConnectedOutlineV(v);
			if(newOutlineVert!=-1)
			{
				checkedVerts.Add(v);
				List<int> newOutline = new List<int>();
				newOutline.Add(v);
				outlines.Add(newOutline);
				
				FollowOutline(newOutlineVert, outlines.Count-1);
				outlines[outlines.Count - 1].Add(v);
			}
		}
	}
}
	
void FollowOutline(int index, int outlineIndex)
{
	outlines[outlineIndex].Add(index);
	checkedVerts.Add(index);
	int next = GetConnectedOutlineV(index);
	
	if(next!=-1)
	{
		FollowOutline(next, outlineIndex);
	}
}
	
//outline edges only share one triangle with each other
bool isOutline(int a, int b)
{
	List<Triangle> ATri = VTLookup[a];
	//List<Triangle> BTri = VTLookup[b];
	
	int sharedTriangles = 0;
	
	for(int i = 0; i < ATri.Count; i++)
	{
		if(ATri[i].Contains(b))
		{
			sharedTriangles++;
		}
		if(sharedTriangles > 1)
			break;
	}
	
	return sharedTriangles == 1;
}

//add this triangle to the list of this vert's triangles	
void AddVTTriangle(int key, Triangle value)
{
	if(VTLookup.ContainsKey(key))
	{
		VTLookup[key].Add(value);
	}
	else
	{
		List<Triangle> tmp = new List<Triangle>();
		tmp.Add(value);
		VTLookup.Add(key, tmp);
	}
	
}
	
//==========================================================================

void AssignVertices(Node[] points, length)
{
	for(int i = 0; i < length; i++)
	{
		if(points[i].vertexIndex == -1)
		{
			points[i].vertexIndex = verts.Count;
			verts.Add(points[i].position);
		}
	}
}


void CreateTriangle(Node a, Node b, Node c)
{
	tri.Add(a.vertexIndex);
	tri.Add(b.vertexIndex);
	tri.Add(c.vertexIndex);
		
	Triangle t = new Triangle(a.vertexIndex, b.vertexIndex, c.vertexIndex);
	
	AddVTTriangle(a.vertexIndex, t);
	AddVTTriangle(b.vertexIndex, t);
	AddVTTriangle(c.vertexIndex, t);
}


//since we don't know how many points we'll have we use params
void MeshFromPoints(Node[] points, int length)
{
	AssignVertices(points);
	
	if(length >2)
	{
		for(int i = 2; i < length ; i++)
		{
			CreateTriangle(points[0], points[i-1], points[i]);
		}
	}
}


//triangulate the marching square based on its configuration, see
//except rotated so 1 is the upper right
//http://users.polytech.unice.fr/~lingrand/MarchingCubes/resources/marchingS.gif
void TriangulateSquare(Square s)
{
	switch(s.configuration)
	{
		case 0:
		break;
		
		// one point
		case 1:
			MeshFromPoints({s.l, s.b, s.D}, 3);
		break;
		
		case 2:
			MeshFromPoints({s.C, s.b, s.r}, 3);
		break;
		
		case 4:
			MeshFromPoints({s.B, s.r, s.t}, 3);
		break;
		
		case 8:
			MeshFromPoints({s.A, s.t, s.l}, 3);
		break;
		
		
		//two points
		case 3:
			MeshFromPoints({s.r, s.C, s.D, s.l}, 4);
		break;
		
		case 6:
			MeshFromPoints({s.t, s.B, s.C, s.b}, 4);
		break;
		
		case 9:
			MeshFromPoints({s.A, s.t, s.b, s.D}, 4);
		break;
		
		case 12:
			MeshFromPoints({s.A, s.B, s.r, s.l}, 4);
		break;
			case 5:
			MeshFromPoints({s.t, s.B, s.r, s.b, s.D, s.l}, 6);
		break;
		
		case 10:
			MeshFromPoints({s.A, s.t, s.r, s.C, s.b, s.l}, 6);
		break;
		
		
		//three points
		case 7:
			MeshFromPoints({s.t, s.B, s.C, s.D, s.l}, 5);
		break;
		
		
		case 11:
			MeshFromPoints({s.A, s.t, s.r, s.C, s.D}, 5);
		break;
		
		case 13:
			MeshFromPoints({s.A, s.B, s.r, s.b, s.D}, 5);
		break;
		
		case 14:
			MeshFromPoints({s.A, s.B, s.C, s.b, s.l}, 5);
		break;
		
		//four points
		
		case 15:
			MeshFromPoints({s.A, s.B, s.C, s.D}, 4);
		break;
	}
}

void DrawGrid(Grid* g)
{
	if(g!=NULL)
	{
		for(int x = 0; x < g.GetLength() ; x++)
		{
			for(int y = 0; y < g.GetHeight() ; y++)
			{
				glm::vec3 color = (g[x][y].A.active) ? glm::vec3(1, 1, 1):glm::vec3(0, 0, 0);
				Utilities::DrawCube(g[x][y].A.position, Vector3.one * 0.4f);
				
				color = (g[x][y].B.active) ? glm::vec3(1, 1, 1):glm::vec3(0, 0, 0);
				Utilities::DrawCube(g[x][y].B.position, Vector3.one * 0.4f);
				
				color = (g[x][y].C.active) ? glm::vec3(1, 1, 1):glm::vec3(0, 0, 0);
				Utilities::DrawCube(g[x][y].C.position, Vector3.one * 0.4f);
				
				color = (g[x][y].D.active) ? glm::vec3(1, 1, 1):glm::vec3(0, 0, 0);
				Utilities::DrawCube(g[x][y].D.position, Vector3.one * 0.4f);
				
				color = Color.grey;
				Utilities::DrawCube(g[x][y].t.position, Vector3.one * 0.15f);
				Utilities::DrawCube(g[x][y].l.position, Vector3.one * 0.15f);
				Utilities::DrawCube(g[x][y].b.position, Vector3.one * 0.15f);
				Utilities::DrawCube(g[x][y].r.position, Vector3.one * 0.15f);
				
			}
		}
	}
}

//save the grid or mesh as a jpeg
//probably use stb_image to do so
void SaveJPG(Grid* g)
{
	//important to initialize values to avoid malfunction
	unsigned char *Buff = new unsigned char[512*512*3];
	
	/*
	void glReadPixels(	GLint x,
		GLint y,
		GLsizei width,
		GLsizei height,
		GLenum format,
		GLenum type,
		GLvoid * data);
	*/
	
	glReadBuffer(GL_BACK);
	glReadPixels(0, 0, 512, 512, GL_RGB, GL_UNSIGNED_BYTE, Buff);
	
	
}

