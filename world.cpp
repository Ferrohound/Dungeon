
#include <string>
#include <iostream>
#include <cstdlib>
#include <time.h>
#include <cmath>

#include "world.h"

using std::string;
using std::cin;
using std::cout;
using std::endl;
using std::abs;

//========================FLOOR METHODS================================
Floor::Floor(int difficulty){
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
//method to randomly generate a floor given certain parameters
//fix this up
//perhaps rename floor into room and have pointers to other rooms like a quad tree
void Floor::generate(int difficulty){
	for(int i=0; i<_height;i++){
		//get the number of non-space characers on the floor
		int width = rand() % 20 + 10;
		_numF[i] = width;
		//cout<<"test1";
		if(i!=0){
			if(_numF[i-1]-_numF[i] == 0){
				//cout<<"test4";
				_offset[i] = rand() % 5;
			}
			else{
				//cout<<"test5";
				int diff = (abs(_numF[i-1] - _numF[i]))/2;
				_offset[i] = rand() % diff;
			}
			//cout<<"test2";
		}
		else{
			_offset[i] = rand() % 5;
			//cout<<"test3";
		}
		_totalW[i] = _offset[i] + _numF[i];
		_data[i] = new char [_totalW[i]];
		for(int j=0;j<_totalW[i];j++){
			if(j<_offset[i]){
				_data[i][j] = ' ';
			}
			else if(i==0 || i == _height-1 ||j == _offset[i] || j==_totalW[i]-1) {
				_data[i][j] = 'X';
			}
			else{
				_data[i][j] = '.';
			}
		}
	}
	
	//loop through a second time to clean up the map aethetic
	return;
}


void Floor::clear(){
	
	return;
}

void Floor::print(){
	for(int i=0; i<_height;i++){
		for(int j=0;j<_totalW[i];j++){
			cout<<_data[i][j];
			cout<<" ";
		}
		cout<<endl;
	}
}

//======================WORLD METHODS=================================
//constructor
World::World(const string& name/* = "Earth"*/, int floors/*=5*/, int difficulty/* = 1*/){
	_name = name;
	_data = new Floor*[floors];
	_floors = floors;
	_current = 0;
	_difficulty = difficulty;
	generate();
}

Floor* World::getFloor(int index){
	return _data[index];
}

//generate all the floors for the world
void World::generate(){
	//think of a better equation for scaling difficulty
	int diff = 1;
	//iterate over every floor, delete it and create a new one
	for(int i=0; i<_floors; i++){
		delete _data[i];
		_data[i] = new Floor(diff);
	}
	return;
}

void World::print(){
	_data[_current]->print();
}

void World::clear(){
	
	return;
}
