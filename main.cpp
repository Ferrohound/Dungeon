/*
Sample procedurally generated dungeon crawler


Author: Ferrohound
*/

#include <stdio.h>
#include <iostream>
#include <string>
#include <Windows.h>
#include <ctime>
#include <fstream>

#include "characters.h"
#include "world.h"


using std::cout;
using std::cin;
using std::endl;

#define offset "		"

//don't forget to pass by reference, not value~!
//use a hash table to store the ascii letters
//give each character/monster ascii art too


//display a menu of some sort
void menu(){
	system("cls");
	cout<<"MENU"<<endl;
}

//Intro text for first-time players
void intro(){
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
	//system("cls");
}

void exit(){
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

int main(int argx, char*argv[]){
	
	intro();
	
	World** universe = new World*[1];
	universe[0] = new World("Test",1);
	
	universe[0]->getFloor(0)->print();
	
	char key;
	
	//main loop
	while (true){
		//add cases for each direction pressed, pause menu, etc...
		if(GetAsyncKeyState(VK_ESCAPE) or GetAsyncKeyState(0x51)){
			exit();
			system("cls");
			universe[0]->print();
		}
		//cin>>key;
	}
	
	// ==================movement========================
	//system("cls") is clear screen
	//system("pause") probably freezes a while loop or whatever
	//Slee(10) pauses the system for 10 milliseconds
	return 0;
}


