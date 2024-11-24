#pragma once


#include <map>
#include <time.h>
#include <iostream>
#include "Object.h"
#include "Door.h"
#include "geometryUtils.h"


namespace agp
{
	class Basement; //Logistics for the rooms
	class Room;		//
	enum RoomState;
	enum RoomType;
}

class agp::Room : public Object
{
protected:

	RoomState _roomState;
	RoomType _roomType;
	RoomType _roomTypeUp;
	RoomType _roomTypeDown;
	RoomType _roomTypeRight;
	RoomType _roomTypeLeft;
	Door* _doorUp;
	Door* _doorDown;
	Door* _doorRight;
	Door* _doorLeft;
	std::pair<int, int> _coords;
	float _x;
	float _y;
	int _w = 16;
	int _h = 12;


public:

	Room(Scene* scene,
		const RectF& rect,
		RoomType roomType,
		RoomType roomTypeUp,
		RoomType roomTypeDown,
		RoomType roomTypeRight,
		RoomType roomTypeLeft,
		std::pair<int, int> coords,
		int layer = 0);
	virtual ~Room() {};

	Room& operator=(const Room& r) = default;

	RoomType type() { return _roomType; }
	std::pair<int, int> coords() { return _coords; }

	void Draw();
	void openCloseDoor();
	void offLightDoor();

	virtual void update(float dt);
	virtual std::string name() override;
};


class agp::Basement
{

protected:

	std::map<std::pair<int, int>, agp::Room*> _mapRooms;

public:

	Basement() { srand(static_cast<unsigned int>(time(0))); }
	~Basement() {
		for (auto& room : _mapRooms)
			delete room.second;
		_mapRooms.clear();
	};

	Room* room(const std::pair<int, int>& coords) { return _mapRooms[coords]; }

	void generateRooms(Scene* world);
	

};

float tCoordX(float x, float w);
float tCoordY(float y, float h);