#pragma once


#include <map>
#include <time.h>
#include "Object.h"
#include "geometryUtils.h"
#include "HammerBrother.h"

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

	int _layer;
	RoomState _roomState;
	RoomType _roomType;
	RoomType _roomTypeUp;
	RoomType _roomTypeDown;
	RoomType _roomTypeRight;
	RoomType _roomTypeLeft;
	int _x;
	int _y;
	int _w = 16;
	int _h = 12;


public:

	Room(Scene* scene,
		const RectF& rect, 
		RoomType _roomType,
		RoomType _roomTypeUp,
		RoomType _roomTypeDown,
		RoomType _roomTypeRight,
		RoomType _roomTypeLeft,
		int layer = 0);
	virtual ~Room() {};

	Room& operator=(const Room& r) = default;

	void Draw();

	virtual void update(float dt);
};


class agp::Basement
{

protected:

	std::map<agp::Vec2D<int>, agp::Room*> _mapRooms;

public:

	Basement() { srand(static_cast<unsigned int>(time(0))); };

	void generateRooms(Scene* world);


};