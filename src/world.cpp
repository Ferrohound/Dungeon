
#include <string>
#include <iostream>
#include <cstdlib>
#include <time.h>

#include <cmath>
#include <algorithm> /*std::find(vector.begin(), vector.end(), item)!=vector.end())*/
//#include <utility> /*std::pair, std::make_pair */


#include "world.h"

using std::string;
using std::cin;
using std::cout;
using std::endl;
using std::abs;

//========================FLOOR METHODS================================
Floor::Floor(int difficulty)
{
	//replace width and height with rand calls
	srand(time(NULL));
	_width = rand() % 20 + 10;
	_height = rand() % 20 + 10;
	
	//initialize all heap memory
	_numF = new int [_height];
	_offset = new int[_height];
	_data = new char*[_height];
	_totalW = new int[_height];
	
	generate(difficulty);
}

Floor::~Floor(){
	
}
//method to randomly generate a floor given certain parameters
//fix this up
//perhaps rename floor into room and have pointers to other rooms like a quad tree
void Floor::generate(int difficulty)
{
	player.x = 5;
	player.y = 5;
	
	for(int i=0; i<_height;i++)
	{
		_offset[i] = 2;
		_totalW[i] = 10;
		_data[i] = new char [_totalW[i]];
		
		for(int j=0;j<_totalW[i];j++)
		{
			cout<<"Loop indices"<<endl;
			cout<<i<<" "<<j<<endl;
			cout<<_height<<" "<<_totalW[i]<<endl;
			
			if(j<_offset[i])
			{
				_data[i][j] = ' ';
			}
			else if(i==0 || i == _height-1 ||j == _offset[i] || j==_totalW[i]-1) 
			{
				_data[i][j] = 'X';
			}
			else
			{
				_data[i][j] = '.';
			}
		}
	}
	
	_data[player.x][player.y] = 'O';
	
	//loop through a second time to clean up the map aethetic
	return;
}


void Floor::clear()
{
	
	return;
}

void Floor::print()
{
	for(int i=0; i<_height;i++){
		for(int j=0;j<_totalW[i];j++){
			cout<<_data[i][j];
			cout<<" ";
		}
		cout<<endl;
	}
}

//move enemies and other things I suppose
void Floor::update()
{
	
}

//fix this later
void Floor::move()
{
	
	int x=player.x + rand()%3 - 1;
	
	if(x<0)
		x = 0;
	if(x>9)
		x = 9;
	
	int y= player.y + rand()%3 - 1;
	
	if(y<0)
		y = 0;
	if(player.y>9)
		y = 9;
	
	if(_data[x][y] != 'X')
	{
		_data[player.x][player.y] = '.';
		player.x = x;
		player.y = y;
		_data[player.x][player.y] = 'O';
	}
	
	update();
}

//======================WORLD METHODS=================================
//constructor
World::World(const string& name/* = "Earth"*/, int floors/*=5*/, int difficulty/* = 1*/)
{
	_name = name;
	_data = new Floor*[floors];
	_floors = floors;
	_current = 0;
	_difficulty = difficulty;
	cout<<"Generating..."<<endl;
	generate();
}

World::~World(){
	
}

Floor* World::getFloor(int index)
{
	return _data[index];
}

//generate all the floors for the world
void World::generate()
{
	//think of a better equation for scaling difficulty
	int diff = 1;
	//iterate over every floor, delete it and create a new one
	for(int i=0; i<_floors; i++){
		delete _data[i];
		_data[i] = new Floor(diff);
	}
	return;
}

void World::print()
{
	_data[_current]->print();
}

void World::clear()
{
	
	return;
}

void World::update()
{
	
}

void World::move()
{
	_data[_current]->move();
}
