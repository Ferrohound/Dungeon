#include <stdio.h>
#include <iostream>
#include <string>
#include <fstream>

#include "characters.h"
#include "world.h"
#include "engine.h"

#define offset "		"

using std::string;
using std::cout;
using std::cin;
using std::endl;

Engine::Engine()
{
	
	
	
}

Engine::~Engine()
{
	//clear everything
	
	
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
	while (true)
	{
		//add cases for each direction pressed, pause menu, etc...
		if(GetAsyncKeyState(VK_ESCAPE) or GetAsyncKeyState(0x51))
		{
			exit();
			system("cls");
			universe[0]->print();
		}
		cin>>key;
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

void Engine::exit()
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

void Engine::save()
{
	
}

void Engine::load()
{
	
}