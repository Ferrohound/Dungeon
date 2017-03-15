#include <stdio.h>
#include <iostream>
#include <string>
#include <fstream>
#include <Windows.h>

#include "engine.h"

#define offset "		"

using std::string;
using std::cout;
using std::cin;
using std::endl;

Engine::Engine()
{
	cout<<"Creating..."<<endl;
	_universe = new World*[1];
	_universe[0] = new World("Earth", 1);
	cout<<"Done creating..."<<endl;
	
}

Engine::~Engine()
{
	//clear everything
	delete _universe[0];
	delete _universe;
}

//===========================Run====================================
void Engine::run(int state)
{
	//pretty much just transitions to the game states
	switch(state)
	{
		case 0:
		explore();
		break;
		
		case 1:
		explore();
		break;
		
		case 2:
		explore();
		break;
		
		case 3:
		explore();
		break;
	}
	
}

//===============================Game States=================================== 

//ideally load this from a file 
void Engine::intro()
{
	system("cls");
	cout<<"Here we go, kiddo!"<<endl;
	cin.ignore();
	cout<<"This is you. ------------->	O"<<endl;
	cout<<"These are your enemies.------------>	x y z"<<endl;
	cin.ignore();
	system("cls");
	cout<<"Wipe them out."<<endl;
	cin.ignore();
	system("cls");
	cout<<"Use arrow keys or WASD to move"<<endl;
	cout<<"Press escape to quit"<<endl;
	cin.ignore();
}

void Engine::menu()
{
	system("cls");
	cout<<"MENU"<<endl;
}

void Engine::explore()
{
	char key;
	system("cls");
	_universe[0]->print();
	while (true)
	{
		//add cases for each direction pressed, pause menu, etc...
		if(GetAsyncKeyState(VK_ESCAPE) or GetAsyncKeyState(0x51))
		{
			system("cls");
			Exit();
			_universe[0]->print();
		}
		cin>>key;
		system("cls");
		_universe[0]->print();
	}
	
	// ==================movement========================
	//system("cls") is clear screen
	//system("pause") probably freezes a while loop or whatever
	//Sleep(10) pauses the system for 10 milliseconds
	
	return;
}

void Engine::battle(Enemy** enemies)
{
	
	
}

//================================Exit, Load, Save=========================================

void Engine::Exit()
{
	char key;
	system("cls");
	cout<<"Do you want to quit? (y/n + Enter)"<<endl;
	cin>>key;
	switch(key){
		case 'y':
			exit(0);
		case 'n':
		default:
			break;
	}
	return;
}

void Engine::Save()
{
	
}

void Engine::Load()
{
	
}