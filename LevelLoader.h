// ----------------------------------------------------------------
// From "Algorithms and Game Programming" in C++ by Alessandro Bria
// Copyright (C) 2024 Alessandro Bria (a.bria@unicas.it). 
// All rights reserved.
// 
// Released under the BSD License
// See LICENSE in root directory for full details.
// ----------------------------------------------------------------

#pragma once

#include <string>

namespace agp
{
	class GameScene;
	class LevelLoader;
	class Basement;
	class Room;
}

// LevelLoader (singleton)
// - provides game scene creation methods
class agp::LevelLoader
{

	protected:

		// constructor inaccesible due to singleton
		LevelLoader();

	public:

		// singleton
		static LevelLoader* instance();

		GameScene* load(const std::string& name);
};



//
//
//enum TYPE_ROOM
//{
//	EMPTY, NORMAL, TREASURE, BOSS, SHOP
//};
//
//class agp::Room
//{
//	protected:
//
//		
//
//	public:
//
//		Room();
//		
//
//
//
//};
//
//
//
//class agp::Basement
//{
//private:
//	int _numPhases;
//	std::map<std::pair<int, int>, Room> _rooms;
//
//public:
//
//	Basement();
//
//	void generateRooms(GameScene* world, int numRooms);
//};
//













//struct agp::Room
//{
//
//	int _x;  // coords to map
//	int _y;
//	TYPE_ROOM _doorUp;
//	TYPE_ROOM _doorDown;
//	TYPE_ROOM _doorRight;
//	TYPE_ROOM _doorLeft;
//	TYPE_ROOM _type_room;
//	int _phase;
//
//	Room();
//	Room(int x, int y, TYPE_ROOM type_room, TYPE_ROOM doorUp, TYPE_ROOM doorDown, TYPE_ROOM doorRight, TYPE_ROOM doorLeft, int phase);
//};
//
