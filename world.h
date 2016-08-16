/*
	contains constructor, taking the number of floors as an argument
	an rng to randomly generate a layout for each floor
	draw function
	delete function to clear itself

	Contains the code for the World and the individual floors
	The world contains each of the floors
	The floor contains the layout of the floor along with the enemies, items, npcs on
	said floor
	
	perhaps instead of going floor by floor, maybe room by room, and rename World to Floor?
	
	Author: Ferrohound
*/

#include <string>
#include <stdio.h>
#include <iostream>

using std::cout;
using std::cin;
using std::endl;
using std::string;

//============================FLOOR============================================
//each floor takes care of its own data, the world contains pointers to each floor
//and probably whatever enemies exist on it to keep consistency
class Floor{
	public:
		Floor(int difficulty=1);
		//print the current board, including where the player and enemies are
		void print();
		
	private:
		//member variables
		int _width;
		int _height;
		char ** _data;
		//the length of non-space characters
		int* _numF;
		//the offset of spaces per line
		int* _offset;
		//the total length of the line
		int* _totalW;
		
		//member functions
		void generate(int difficulty);
		void clear();
};

//========================World=================================================
//contains the floors, total number of enemies(?), difficulty, etc...
class World{
	public:
		World(const string& name = "Earth", int floors=5, int difficulty = 1);
		//method to delete each floor, free up memory
		void clear();
		void print();
		Floor* getFloor(int index);
	
	private:
		string _name;
		int _floors, _difficulty, _current;
		Floor ** _data;
		
		//method to generate however many floors are required
		void generate();
};



