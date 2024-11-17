#include "Room.h"
#include "Scene.h"
#include "SpriteFactory.h"
#include "RenderableObject.h"
#include "StaticObject.h"
#include "Door.h"
#include "Mario.h"
#include "GameScene.h"
#include "HUD.h"
#include <iostream>
#include <list>
#include <algorithm>
#include <random>

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
	_roomTypeUp	= roomTypeUp;
	_roomTypeDown = roomTypeDown;
	_roomTypeRight= roomTypeRight;
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

	Mario* mario = dynamic_cast<GameScene*>(_scene)->player();

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
	new StaticObject(_scene, RectF(_x, _y, 2, 5 + 0.4), spriteLoader->get("empty"));
	new StaticObject(_scene, RectF(_x, _y + 7 - 0.75, 2, 5), spriteLoader->get("empty"));
	//up						   _
	new StaticObject(_scene, RectF(_x + 2, _y, 5 + 0.6, 2), spriteLoader->get("empty"));
	new StaticObject(_scene, RectF(_x + 2 + 7 - 0.6, _y, 5 + 1.2, 2), spriteLoader->get("empty"));
	//down						   _
	new StaticObject(_scene, RectF(_x + 2, _y + 10, 5 + 0.6, 2), spriteLoader->get("empty"));
	new StaticObject(_scene, RectF(_x + 2 + 7 - 0.6, _y + 10, 5 + 1.2, 2), spriteLoader->get("empty"));
	//right						   _
	new StaticObject(_scene, RectF(_x + 14, _y, 2, 5 + 0.4), spriteLoader->get("empty"));
	new StaticObject(_scene, RectF(_x + 14, _y + 7 - 0.75, 2, 5), spriteLoader->get("empty"));

	// DOOR (viene inserita la porta senza collider oppure il collider sopra il muro)

	RectF rect = RectF(_x + 7, _y, 2, 2);
	if (_roomTypeUp == RoomType::BOSS || _roomTypeUp == RoomType::TREASURE )
		_doorUp = new Door(_scene, rect, _roomTypeUp, DoorPosition::TOP);
	else if (_roomTypeUp == RoomType::NORMAL || _roomTypeUp == RoomType::INITIAL || _roomTypeUp == RoomType::SHOP)
		_doorUp = new Door(_scene, rect, _roomType, DoorPosition::TOP);
	else if (_roomTypeUp == RoomType::EMPTY)
		new StaticObject(_scene, rect, spriteLoader->get("empty"));

	rect = RectF(_x + 7, _y + 10, 2, 2);
	if (_roomTypeDown == RoomType::BOSS || _roomTypeDown == RoomType::TREASURE)
		_doorDown = new Door(_scene, rect, _roomTypeDown, DoorPosition::BOTTOM, 0, SDL_FLIP_VERTICAL);
	else if (_roomTypeDown == RoomType::NORMAL || _roomTypeDown == RoomType::INITIAL || _roomTypeDown == RoomType::SHOP)
		_doorDown = new Door(_scene, rect, _roomType, DoorPosition::BOTTOM, 0, SDL_FLIP_VERTICAL);
	else if (_roomTypeDown == RoomType::EMPTY)
		new StaticObject(_scene, rect, spriteLoader->get("empty"));

	rect = RectF(_x + 14, _y + 5 + 0.12, 2, 2);
	if (_roomTypeRight == RoomType::BOSS || _roomTypeRight == RoomType::TREASURE)
		_doorRight = new Door(_scene, rect, _roomTypeRight, DoorPosition::RIGHT, 270);
	else if (_roomTypeRight == RoomType::NORMAL || _roomTypeRight == RoomType::INITIAL || _roomTypeRight == RoomType::SHOP)
		_doorRight = new Door(_scene, rect, _roomType, DoorPosition::RIGHT, 270);
	else if (_roomTypeRight == RoomType::EMPTY)
		new StaticObject(_scene, rect, spriteLoader->get("empty"));

	rect = RectF(_x, _y + 5 + 0.12, 2, 2);
	if (_roomTypeLeft == RoomType::BOSS || _roomTypeLeft == RoomType::TREASURE)
		_doorLeft = new Door(_scene, rect, _roomTypeLeft, DoorPosition::LEFT, 90);
	else if (_roomTypeLeft == RoomType::NORMAL || _roomTypeLeft == RoomType::INITIAL || _roomTypeLeft == RoomType::SHOP)
		_doorLeft = new Door(_scene, rect, _roomType, DoorPosition::LEFT, 90);
	else if (_roomTypeLeft == RoomType::EMPTY)
		new StaticObject(_scene, rect, spriteLoader->get("empty"));

	std::cout << _roomType << std::endl;
	if (_roomType == RoomType::INITIAL) {
		new RenderableObject(_scene, RectF(2.25, 4.4, 11.5, 3.2), spriteLoader->get("controls"));
	}	
	else if (_roomType == RoomType::TREASURE)
	{	
		new CollidableObject(_scene, RectF(_x + 5.5, _y + 4.0, 1, 1), spriteLoader->get("fireplace_blue"));
		new RenderableObject(_scene, RectF(_x + 5.5, _y + 3.5, 1, 1), spriteLoader->get("bluefire"));
		new CollidableObject(_scene, RectF(_x + 9.5, _y + 4.0, 1, 1), spriteLoader->get("fireplace_blue"));
		new RenderableObject(_scene, RectF(_x + 9.5, _y + 3.5, 1, 1), spriteLoader->get("bluefire"));
		new CollidableObject(_scene, RectF(_x + 5.5, _y + 7.0, 1, 1), spriteLoader->get("fireplace_blue"));
		new RenderableObject(_scene, RectF(_x + 5.5, _y + 6.5, 1, 1), spriteLoader->get("bluefire"));
		new CollidableObject(_scene, RectF(_x + 9.5, _y + 7.0, 1, 1), spriteLoader->get("fireplace_blue"));
		new RenderableObject(_scene, RectF(_x + 9.5, _y + 6.5, 1, 1), spriteLoader->get("bluefire"));
	}	
	else if (_roomType == RoomType::NORMAL)
	{
		float vec[4][8][2] = {  {{3,1.8},{2,2.8},{2,7.8},{3,8.8},{12,1.8},{12.9,2.8},{12.9,7.8},{12,8.8} },
							    {{5,4.2},{6,4.2},{7,4.2},{8,4.2},{6,6.5},{7,6.5},{8,6.5},{9,6.5}},
								{{6.4,4.6},{7.4,4.6},{8.4,4.6},{6.4,5.4},{8.4,5.4},{6.4,6.2},{7.4,6.2},{8.4,6.2}},
								{{7.4,4.2},{8,4.2},{3.4,4.2},{4,4.2}, {5,6.5},{9,6.5},{11,8.4},{9,6.5}} };
		int num = rand() % 4;
		for (int i = 0; i < 8; i++)
		{
			new StaticObject(_scene, RectF(_x + vec[num][i][0],_y + vec[num][i][1], 1.4, 1.2), spriteLoader->get("rock"));
		}
	}

	new RenderableObject(_scene, RectF(_x, _y, 16, 12), spriteLoader->get("shading"));
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
	int typeRoom[13] = { 1,2,2,3,2,2,4,2,2,5}; // 1 init // 2 normal // 3 treasure // 4 shop // 5 boss

	int SIZE = 7; //std::size(typeRoom);
	int START_COORD = static_cast<int>(SIZE/2);

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
				
				Room* room = new Room(world, RectF(j - START_COORD, i - START_COORD, 16, 12), roomType, roomTypeUp, roomTypeDown, roomTypeRight, roomTypeLeft, { j - START_COORD, i - START_COORD });
				
				_mapRooms[{j - START_COORD, i - START_COORD}] = room;
				
				HUD::instance()->drawMinimap(RectF(j - START_COORD, i - START_COORD, 1, 1), roomType);
			}
		}
	}

	for (int i = 0; i < SIZE; i++) {
		for (int j = 0; j < SIZE; j++) {
			std::cout << matrix[i][j] << " ";
		}
		std::cout << std::endl;
	}

}
