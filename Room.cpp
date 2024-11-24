#include "Room.h"
#include "Scene.h"
#include "SpriteFactory.h"
#include "RenderableObject.h"
#include "StaticObject.h"
#include "Door.h"
#include "Isaac.h"
#include "GameScene.h"
#include "HUD.h"
#include "Game.h"
#include <iostream>
#include <list>
#include <algorithm>
#include <random>
#include "Altar.h"
#include "Rock.h"
#include "Poop.h"
using namespace agp;

Room::Room(Scene* scene,
	const RectF& rect,
	RoomType roomType,
	RoomType roomTypeUp,
	RoomType roomTypeDown,
	RoomType roomTypeRight,
	RoomType roomTypeLeft,
	std::pair<int, int> coords,
	int layer) :
	Object(scene, RectF(rect.pos.x * 16, rect.pos.y * 12, 16, 12), layer)
{
	_scene = scene;
	_rect = RectF(rect.pos.x * 16, rect.pos.y * 12, 16, 12);
	_roomState = RoomState::INACTIVE;
	_roomType = roomType;
	_roomTypeUp = roomTypeUp;
	_roomTypeDown = roomTypeDown;
	_roomTypeRight = roomTypeRight;
	_roomTypeLeft = roomTypeLeft;
	_coords = coords;
	_x = _rect.pos.x;
	_y = _rect.pos.y;
	_layer = layer;
	_doorUp = nullptr;
	_doorDown = nullptr;
	_doorRight = nullptr;
	_doorLeft = nullptr;

	Draw();

}

void Room::update(float dt)
{
	Object::update(dt);
}

void Room::Draw()
{

	SpriteFactory* spriteLoader = SpriteFactory::instance();
	std::string wallUp;
	std::string wallDown;
	std::string wallRight;
	std::string wallLeft;


	if (_roomType == RoomType::BOSS)
	{
		wallUp = "basement_boss_UpL";
		wallDown = "basement_boss_UpR";
		wallRight = "basement_boss_DownL";
		wallLeft = "basement_boss_DownR";
	}
	else if (_roomType == RoomType::SHOP)
	{
		wallUp = "basement_shop_UpL";
		wallDown = "basement_shop_UpR";
		wallRight = "basement_shop_DownL";
		wallLeft = "basement_shop_DownR";
	}
	else if (_roomType == RoomType::TREASURE)
	{
		wallUp = "basement_treasure_UpL";
		wallDown = "basement_treasure_UpR";
		wallRight = "basement_treasure_DownL";
		wallLeft = "basement_treasure_DownR";
	}
	else
	{
		wallUp = "basement_UpL";
		wallDown = "basement_UpR";
		wallRight = "basement_DownL";
		wallLeft = "basement_DownR";
	}

	// WALL and FLOOR
	new RenderableObject(_scene, RectF(_x, _y, 8, 6), spriteLoader->get(wallUp), 0, 0, SDL_FLIP_NONE);
	new RenderableObject(_scene, RectF(_x + 8, _y, 8, 6), spriteLoader->get(wallDown), 0, 0, SDL_FLIP_HORIZONTAL);
	new RenderableObject(_scene, RectF(_x, _y + 6, 8, 6), spriteLoader->get(wallRight), 0, 0, SDL_FLIP_VERTICAL);
	new RenderableObject(_scene, RectF(_x + 8, _y + 6, 8, 6), spriteLoader->get(wallLeft), 0, 0, SDL_RendererFlip(SDL_FLIP_VERTICAL | SDL_FLIP_HORIZONTAL));
	//COLLIDER
	//left
	new StaticObject(_scene, RectF(_x, _y, 2, 5 + 0.4f), spriteLoader->get("wall"));
	new StaticObject(_scene, RectF(_x, _y + 7 - 0.75f, 2, 5 + 0.75f), spriteLoader->get("wall"));
	new DynamicObject(_scene, RectF(_x, _y, 1, 12), spriteLoader->get("upWall"));
	//up						   _
	new StaticObject(_scene, RectF(_x + 2, _y, 5 + 0.6f, 2), spriteLoader->get("wall"));
	new StaticObject(_scene, RectF(_x + 2 + 7 - 0.6f, _y, 5 + 0.6f, 2), spriteLoader->get("wall"));
	new DynamicObject(_scene, RectF(_x, _y, 16, 1), spriteLoader->get("upWall"));
	//down						   _
	new StaticObject(_scene, RectF(_x + 2, _y + 10, 5 + 0.6f, 2), spriteLoader->get("wall"));
	new StaticObject(_scene, RectF(_x + 2 + 7 - 0.6f, _y + 10, 5 + 0.6f, 2), spriteLoader->get("wall"));
	new DynamicObject(_scene, RectF(_x, _y + 10.2f, 16, 1.8f), spriteLoader->get("upWall"));
	//right						   _
	new StaticObject(_scene, RectF(_x + 14, _y, 2, 5 + 0.4f), spriteLoader->get("wall"));
	new StaticObject(_scene, RectF(_x + 14, _y + 7 - 0.75f, 2, 5 + 0.75f), spriteLoader->get("wall"));
	new DynamicObject(_scene, RectF(_x + 15, _y, 1, 12), spriteLoader->get("upWall"));

	// DOOR (viene inserita la porta senza collider oppure il collider sopra il muro)

	RectF rect = RectF(_x + 7, _y, 2, 2);
	if (_roomTypeUp == RoomType::BOSS || _roomTypeUp == RoomType::TREASURE)
		_doorUp = new Door(_scene, rect, _roomTypeUp, DoorPosition::TOP);
	else if (_roomTypeUp == RoomType::NORMAL || _roomTypeUp == RoomType::INITIAL || _roomTypeUp == RoomType::SHOP)
		_doorUp = new Door(_scene, rect, _roomType, DoorPosition::TOP);
	else if (_roomTypeUp == RoomType::EMPTY)
		new StaticObject(_scene, rect, spriteLoader->get("wall"));

	rect = RectF(_x + 7, _y + 10, 2, 2);
	if (_roomTypeDown == RoomType::BOSS || _roomTypeDown == RoomType::TREASURE)
		_doorDown = new Door(_scene, rect, _roomTypeDown, DoorPosition::BOTTOM, 0, SDL_FLIP_VERTICAL);
	else if (_roomTypeDown == RoomType::NORMAL || _roomTypeDown == RoomType::INITIAL || _roomTypeDown == RoomType::SHOP)
		_doorDown = new Door(_scene, rect, _roomType, DoorPosition::BOTTOM, 0, SDL_FLIP_VERTICAL);
	else if (_roomTypeDown == RoomType::EMPTY)
		new StaticObject(_scene, rect, spriteLoader->get("wall"));

	rect = RectF(_x + 14, _y + 4.95f, 2, 2.44f);
	if (_roomTypeRight == RoomType::BOSS || _roomTypeRight == RoomType::TREASURE)
		_doorRight = new Door(_scene, rect, _roomTypeRight, DoorPosition::RIGHT, 270);
	else if (_roomTypeRight == RoomType::NORMAL || _roomTypeRight == RoomType::INITIAL || _roomTypeRight == RoomType::SHOP)
		_doorRight = new Door(_scene, rect, _roomType, DoorPosition::RIGHT, 270);
	else if (_roomTypeRight == RoomType::EMPTY)
		new StaticObject(_scene, rect, spriteLoader->get("wall"));

	rect = RectF(_x, _y + 4.85f, 2, 2.44f);
	if (_roomTypeLeft == RoomType::BOSS || _roomTypeLeft == RoomType::TREASURE)
		_doorLeft = new Door(_scene, rect, _roomTypeLeft, DoorPosition::LEFT, 90);
	else if (_roomTypeLeft == RoomType::NORMAL || _roomTypeLeft == RoomType::INITIAL || _roomTypeLeft == RoomType::SHOP)
		_doorLeft = new Door(_scene, rect, _roomType, DoorPosition::LEFT, 90);
	else if (_roomTypeLeft == RoomType::EMPTY)
		new StaticObject(_scene, rect, spriteLoader->get("wall"));

	if (_roomType == RoomType::INITIAL)
	{
		new RenderableObject(_scene, RectF(2.25f, 4.4f, 11.5f, 3.2f), spriteLoader->get("controls"));
	}
	else if (_roomType == RoomType::TREASURE)
	{
		new StaticObject(_scene, RectF(_x + 5.5f, _y + 4.0f, 1, 1), spriteLoader->get("fireplace_blue"));
		new RenderableObject(_scene, RectF(_x + 5.5f, _y + 3.5f, 1, 1), spriteLoader->get("bluefire"));
		new StaticObject(_scene, RectF(_x + 9.5f, _y + 4.0f, 1, 1), spriteLoader->get("fireplace_blue"));
		new RenderableObject(_scene, RectF(_x + 9.5f, _y + 3.5f, 1, 1), spriteLoader->get("bluefire"));
		new StaticObject(_scene, RectF(_x + 5.5f, _y + 7.0f, 1, 1), spriteLoader->get("fireplace_blue"));
		new RenderableObject(_scene, RectF(_x + 5.5f, _y + 6.5f, 1, 1), spriteLoader->get("bluefire"));
		new StaticObject(_scene, RectF(_x + 9.5f, _y + 7.0f, 1, 1), spriteLoader->get("fireplace_blue"));
		new RenderableObject(_scene, RectF(_x + 9.5f, _y + 6.5f, 1, 1), spriteLoader->get("bluefire"));
		new Altar(_scene, RectF(_x + 7.5f, _y + 5.5f, 1, 1), spriteLoader->get("altar"));
	}
	else if (_roomType == RoomType::NORMAL)
	{
		//ROCKS && POOPS
		if (rand() % 2 == 0)
		{
			float vec[4][2] = { {1.9f,1.63f},{16 - 2 - 1.3f,1.63f},{1.9f,12 - 2 - 1.4f},{16 - 2 - 1.3f,12 - 2 - 1.4f} };
			for (int i = 0; i < 4; i++)
				new Rock(_scene, { _x + vec[i][0], _y + vec[i][1] });
			int cases = rand() % 2;
			if (cases == 0)
			{
				float vec1[8][2] = { {1,0},{1.8f,0.8f},{1.8f,-0.8f},{2.6f,0},{-1,0},{-1.8f,0.8f},{-1.8f,-0.8f},{-2.6f,0} };
				for (int i = 0; i < 8; i++)
					new Rock(_scene, { vec1[i][0] * 1.4f + _x + 8 - 1.4f / 2, vec1[i][1] * 1.4f + _y + 6 + 0 - 1.4f / 2 });
			}
			if (cases == 1)
				for (int i = 0; i < 8; i++)
					new Rock(_scene, { (rand() % 5 - 2) * 1.4f + _x + 8 - 1.4f / 2, (rand() % 5 - 2) * 1.4f + _y + 6 + 0 - 1.4f / 2 });
		}
		else
		{
			float vec[4][2] = { {1.9f,1.63f},{16 - 2 - 1.3f,1.63f},{1.9f,12 - 2 - 1.4f},{16 - 2 - 1.3f,12 - 2 - 1.4f} };
			for (int i = 0; i < 4; i++)
				new Poop(_scene, { _x + vec[i][0], _y + vec[i][1] });
			int cases = rand() % 2;
			if (cases == 0)
			{
				float vec1[8][2] = { {1,0},{1.8f,0.8f},{1.8f,-0.8f},{2.6f,0},{-1,0},{-1.8f,0.8f},{-1.8f,-0.8f},{-2.6f,0} };
				for (int i = 0; i < 8; i++)
					new Poop(_scene, { vec1[i][0] * 1.4f + _x + 8 - 1.4f / 2, vec1[i][1] * 1.4f + _y + 6 + 0 - 1.4f / 2 });
			}
			if (cases == 1)
				for (int i = 0; i < 8; i++)
					new Poop(_scene, { (rand() % 5 - 2) * 1.4f + _x + 8 - 1.4f / 2, (rand() % 5 - 2) * 1.4f + _y + 6 + 0 - 1.4f / 2 });
		}
	}
	if (_roomType == RoomType::INITIAL || _roomType == RoomType::NORMAL)
	{
		// PROPS
		new RenderableObject(_scene, RectF(_x + 2 + (rand() % 2 ? rand() % 6 : 2), _y + 2 + (rand() % 2 ? rand() % 5 : 1), 1, 1), spriteLoader->get("basement_props"), 1);
		new RenderableObject(_scene, RectF(_x + 2 + (rand() % 2 ? rand() % 6 : 2), _y + 6 + (rand() % 2 ? rand() % 5 : 1), 1, 1), spriteLoader->get("basement_props"), 1);
		new RenderableObject(_scene, RectF(_x + 8 + (rand() % 8 ? rand() % 6 : 2), _y + 2 + (rand() % 2 ? rand() % 5 : 1), 1, 1), spriteLoader->get("basement_props"), 1);
		new RenderableObject(_scene, RectF(_x + 8 + (rand() % 8 ? rand() % 6 : 2), _y + 6 + (rand() % 2 ? rand() % 5 : 1), 1, 1), spriteLoader->get("basement_props"), 1);

		// OVERLAY
		new RenderableObject(_scene, RectF(_x, _y, 16, 12), spriteLoader->get("basement_overlay"), 3);
	}

	//SHADING
	new RenderableObject(_scene, RectF(_x, _y - 0.5f, 16, 13), spriteLoader->get("shading"), 2);
}

void Room::openCloseDoor()
{
	if (_doorUp)
		_doorUp->openClose();
	if (_doorDown)
		_doorDown->openClose();
	if (_doorRight)
		_doorRight->openClose();
	if (_doorLeft)
		_doorLeft->openClose();
}

void Room::offLightDoor()
{
	if (_doorUp)
		_doorUp->offLight();
	if (_doorDown)
		_doorDown->offLight();
	if (_doorRight)
		_doorRight->offLight();
	if (_doorLeft)
		_doorLeft->offLight();
}

std::string Room::name()
{
	if (_roomType == RoomType::BOSS)
		return strprintf("RoomBoss[%d]", _id);
	else if (_roomType == RoomType::SHOP)
		return strprintf("RoomShop[%d]", _id);
	else if (_roomType == RoomType::TREASURE)
		return strprintf("RoomTreasure[%d]", _id);
	else
		return strprintf("Room[%d]", _id);
}

void Basement::generateRooms(Scene* world)
{
	int typeRoomIndex = 1;
	int typeRoom[13] = { 1,2,2,3,2,2,4,2,2,5 }; // 1 init // 2 normal // 3 treasure // 4 shop // 5 boss

	int SIZE = 7; //std::size(typeRoom);
	int START_COORD = static_cast<int>(SIZE / 2);

	std::vector<std::vector<int>> matrix(SIZE, std::vector<int>(SIZE, 0));
	matrix[START_COORD][START_COORD] = 1;

	Vec2D<int> dir[4] = { {0,-1},{0,1},{1,0},{-1,0} };
	std::list<Vec2D<int>> rooms;

	rooms.push_back({ START_COORD,START_COORD });


	while (!rooms.empty())
	{

		int checkAtLeastOneDir = 0;
		for (const auto& room : rooms)
		{

			std::list<Vec2D<int>> newDirs;

			// Generatore di numeri casuali, per non iniziare sempre dalla direzione in alto
			std::vector<int> numbers = { 0, 1, 2, 3 };
			std::random_device rd;
			std::mt19937 gen(rd());
			std::shuffle(numbers.begin(), numbers.end(), gen);

			/////////////////////////////////////////// Ciclo scelta direzioni

			for (int i : numbers) //randomizza l'ordine della creazione delle stanze
			{

				int probability = 70;
				Vec2D<int> tempDirRoom = dir[i] + room;

				// controlli per evitare direnzioni che portano ad errori nel codice
				if (!(tempDirRoom.x - 1 >= 0 && tempDirRoom.x + 1 < SIZE && tempDirRoom.y - 1 >= 0 && tempDirRoom.y + 1 < SIZE))
					break;
				if (matrix[tempDirRoom.x][tempDirRoom.y] != 0)
					continue;

				// Se ci sono stanze vicine a "tempDirRoom" riduci la probabilità di generazione stanze
				int nearRoomsCount = 0;
				for (int j = 0; j < 4; j++)
				{
					Vec2D<int> temp_room_near = tempDirRoom + dir[j];
					if (matrix[temp_room_near.x][temp_room_near.y] != 0)
						nearRoomsCount++;
				}
				if (nearRoomsCount > 2)
					probability = -1;
				else if (nearRoomsCount == 2)
					probability = 1;


				// se "randomDir" supera la probabilità -> inserisci la direzione in "newDirs"
				int randomDir = rand() % 100 > probability ? 0 : 1;
				if (randomDir == 1) // creazione di una stanza
				{

					newDirs.push_back(tempDirRoom);
					probability = std::max(15, probability -= 10);
				}
			}
			/////////////////////////////////////////////////

			for (const auto newDir : newDirs)
			{
				if (typeRoomIndex > std::size(typeRoom) - 1)
					break;

				matrix[newDir.x][newDir.y] = typeRoom[typeRoomIndex];

				if (typeRoom[typeRoomIndex] != 3 && typeRoom[typeRoomIndex] != 4)
					rooms.push_back(newDir);

				typeRoomIndex++;
				checkAtLeastOneDir++;
			}

			if (typeRoomIndex > std::size(typeRoom) - 1)
				break;
		}

		if (typeRoomIndex > std::size(typeRoom) - 1)
		{
			rooms.clear();
			break;
		}

	}
	int count = 0;
	// stampa di controllo -- per controllare quali stanze verranno renderizzate
	for (int i = 0; i < SIZE; i++) {
		for (int j = 0; j < SIZE; j++) {
			if (matrix[i][j] != 0) {
				RoomType roomType = RoomType(matrix[i][j]);

				RoomType roomTypeUp = (i > 0) ? RoomType(matrix[i - 1][j]) : RoomType::EMPTY;
				RoomType roomTypeDown = (i < SIZE - 1) ? RoomType(matrix[i + 1][j]) : RoomType::EMPTY;
				RoomType roomTypeRight = (j < SIZE - 1) ? RoomType(matrix[i][j + 1]) : RoomType::EMPTY;
				RoomType roomTypeLeft = (j > 0) ? RoomType(matrix[i][j - 1]) : RoomType::EMPTY;

				Room* room = new Room(world, RectI(j - START_COORD, i - START_COORD, 16, 12), roomType, roomTypeUp, roomTypeDown, roomTypeRight, roomTypeLeft, { j - START_COORD, i - START_COORD });

				_mapRooms[{j - START_COORD, i - START_COORD}] = room;

				Game::instance()->hud()->drawMinimap(RectI(j - START_COORD, i - START_COORD, 1, 1), roomType);
			}
		}
	}

	//for (int i = 0; i < SIZE; i++) {
	//	for (int j = 0; j < SIZE; j++) {
	//		std::cout << matrix[i][j] << " ";
	//	}
	//	std::cout << std::endl;
	//}

}

float tCoordX(float x, float w)
{
	return x * w + 8 - w / 2;
}

float tCoordY(float y, float h)
{
	return y * h + 6 - h / 2;
}
