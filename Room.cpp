#include "Room.h"
#include "Scene.h"
#include "SpriteFactory.h"
#include "RenderableObject.h"
#include "StaticObject.h"
#include "Door.h"
#include "Mario.h"
#include "GameScene.h"
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
	int layer) :
	Object(scene, rect, layer)
{
	_scene = scene;
	_rect = rect;
	_roomState = RoomState::INACTIVE;
	_roomType = roomType;
	_roomTypeUp	= roomTypeUp;
	_roomTypeDown = roomTypeDown;
	_roomTypeRight= roomTypeRight;
	_roomTypeLeft = roomTypeLeft;
	_x = rect.pos.x;
	_y = rect.pos.y;
	_layer = layer;

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

	float x = _x * 16;
	float y = _y * 12;
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
	new RenderableObject(_scene, RectF(x, y, 8, 6), spriteLoader->get(wallUp), 0, 0, SDL_FLIP_NONE);
	new RenderableObject(_scene, RectF(x + 8, y, 8, 6), spriteLoader->get(wallDown), 0, 0, SDL_FLIP_HORIZONTAL);
	new RenderableObject(_scene, RectF(x, y + 6, 8, 6), spriteLoader->get(wallRight), 0, 0, SDL_FLIP_VERTICAL);
	new RenderableObject(_scene, RectF(x + 8, y + 6, 8, 6), spriteLoader->get(wallLeft), 0, 0, SDL_RendererFlip(SDL_FLIP_VERTICAL | SDL_FLIP_HORIZONTAL));
	//COLLIDER
	//left
	new StaticObject(_scene, RectF(x, y, 2, 5 + 0.4), spriteLoader->get("empty"));
	new StaticObject(_scene, RectF(x, y + 7 - 0.75, 2, 5), spriteLoader->get("empty"));
	//up
	new StaticObject(_scene, RectF(x + 2, y, 5 + 0.6, 2), spriteLoader->get("empty"));
	new StaticObject(_scene, RectF(x + 2 + 7 - 0.6, y, 5 + 1.2, 2), spriteLoader->get("empty"));
	//down
	new StaticObject(_scene, RectF(x + 2, y + 10, 5 + 0.6, 2), spriteLoader->get("empty"));
	new StaticObject(_scene, RectF(x + 2 + 7 - 0.6, y + 10, 5 + 1.2, 2), spriteLoader->get("empty"));
	//right
	new StaticObject(_scene, RectF(x + 14, y, 2, 5 + 0.4), spriteLoader->get("empty"));
	new StaticObject(_scene, RectF(x + 14, y + 7 - 0.75, 2, 5), spriteLoader->get("empty"));

	// DOOR (viene inserita la porta senza collider oppure il collider sopra il muro)

	RectF rect = RectF(x + 7, y, 2, 2);
	if (_roomTypeUp == RoomType::BOSS || _roomTypeUp == RoomType::TREASURE )
		new Door(_scene, rect, _roomTypeUp, DoorPosition::TOP);
	else if (_roomTypeUp == RoomType::NORMAL || _roomTypeUp == RoomType::INITIAL || _roomTypeUp == RoomType::SHOP)
		new Door(_scene, rect, _roomType, DoorPosition::TOP);
	else if (_roomTypeUp == RoomType::EMPTY)
		new StaticObject(_scene, rect, spriteLoader->get("empty"));

	rect = RectF(x + 7, y + 10, 2, 2);
	if (_roomTypeDown == RoomType::BOSS || _roomTypeDown == RoomType::TREASURE)
		new Door(_scene, rect, _roomTypeDown, DoorPosition::BOTTOM, 0, SDL_FLIP_VERTICAL);
	else if (_roomTypeDown == RoomType::NORMAL || _roomTypeDown == RoomType::INITIAL || _roomTypeDown == RoomType::SHOP)
		new Door(_scene, rect, _roomType, DoorPosition::BOTTOM, 0, SDL_FLIP_VERTICAL);
	else if (_roomTypeDown == RoomType::EMPTY)
		new StaticObject(_scene, rect, spriteLoader->get("empty"));

	rect = RectF(x + 14, y + 5 + 0.12, 2, 2);
	if (_roomTypeRight == RoomType::BOSS || _roomTypeRight == RoomType::TREASURE)
		new Door(_scene, rect, _roomTypeRight, DoorPosition::RIGHT, 270);
	else if (_roomTypeRight == RoomType::NORMAL || _roomTypeRight == RoomType::INITIAL || _roomTypeRight == RoomType::SHOP)
		new Door(_scene, rect, _roomType, DoorPosition::RIGHT, 270);
	else if (_roomTypeRight == RoomType::EMPTY)
		new StaticObject(_scene, rect, spriteLoader->get("empty"));

	rect = RectF(x, y + 5 + 0.12, 2, 2);
	if (_roomTypeLeft == RoomType::BOSS || _roomTypeLeft == RoomType::TREASURE)
		new Door(_scene, rect, _roomTypeLeft, DoorPosition::LEFT, 90);
	else if (_roomTypeLeft == RoomType::NORMAL || _roomTypeLeft == RoomType::INITIAL || _roomTypeLeft == RoomType::SHOP)
		new Door(_scene, rect, _roomType, DoorPosition::LEFT, 90);
	else if (_roomTypeLeft == RoomType::EMPTY)
		new StaticObject(_scene, rect, spriteLoader->get("empty"));


	if (_roomType == RoomType::INITIAL)
		new RenderableObject(_scene, RectF(2.25, 4.4, 11.5, 3.2), spriteLoader->get("controls"));
	else if (_roomType == RoomType::NORMAL)
	{
		for (int i = 0; i < 5; i++)
		{
			int pos_x = rand() % 7 + 3;
			int pos_y = rand() % 6 + 3;
			new StaticObject(_scene, RectF(x + pos_x, y + pos_y, 1, 1), spriteLoader->get("rock"));
		}
	}

	new RenderableObject(_scene, RectF(x, y, 16, 12), spriteLoader->get("shading"));
}



void Basement::generateRooms(Scene* world)
{
	int typeRoomIndex = 1;
	int typeRoom[18] = { 1,2,2,3,2,2,4,2,2,5}; // 1 init // 2 normal // 3 treasure // 4 shop // 5 boss

	int SIZE = std::size(typeRoom);
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
				if (!(tempDirRoom.x >= 0 && tempDirRoom.x <= SIZE && tempDirRoom.y >= 0 && tempDirRoom.y <= SIZE))
					continue;
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
				
				if ((typeRoom[typeRoomIndex] != 3) || (typeRoom[typeRoomIndex] != 4))
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

				RoomType roomTypeUp = (j >= 0) ? RoomType(matrix[i - 1][j]) : RoomType::EMPTY;
				RoomType roomTypeDown = (j < SIZE) ? RoomType(matrix[i + 1][j]) : RoomType::EMPTY;
				RoomType roomTypeRight = (i < SIZE) ? RoomType(matrix[i][j + 1]) : RoomType::EMPTY;
				RoomType roomTypeLeft = (i >= 0) ? RoomType(matrix[i][j - 1]) : RoomType::EMPTY;

				Room* room = new Room(world, RectF(j - START_COORD, i - START_COORD, 16, 12), roomType, roomTypeUp, roomTypeDown, roomTypeRight, roomTypeLeft);
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
