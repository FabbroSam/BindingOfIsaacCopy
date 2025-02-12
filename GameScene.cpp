// ----------------------------------------------------------------
// From "Algorithms and Game Programming" in C++ by Alessandro Bria
// Copyright (C) 2024 Alessandro Bria (a.bria@unicas.it). 
// All rights reserved.
// 
// Released under the BSD License
// See LICENSE in root directory for full details.
// ----------------------------------------------------------------

#include "GameScene.h"
#include "RenderableObject.h"
#include "CollidableObject.h"
#include "View.h"
#include "SpriteFactory.h"
#include "Game.h"
#include "Menu.h"
#include "Audio.h"
#include "Isaac.h"
#include "Room.h"
#include "Heart.h"
#include "Door.h"
#include "HUD.h"
#include "UIMonster.h"
#include "Duke.h"
#include "Fly.h"
#include "Host.h"
#include "Gusher.h"
#include <iostream>
using namespace agp;

GameScene::GameScene(const RectF& r, float dt)
	: Scene(r)
{
	_dt = dt;
	_timeToSimulate = 0;
	_isaac = nullptr;
	_duke = nullptr;
	_cameraZoomVel = 0.1f;
	_a_pressed = false;
	_d_pressed = false;
	_w_pressed = false;
	_s_pressed = false;
	_run_pressed = false;
	_collidersVisible = false;
	_right_pressed = false;
	_left_pressed = false;
	_up_pressed = false;
	_down_pressed = false;

	// move view free
	_moveView = false;
	_moveIsaac = true;

	_vsMonster = true;

	_room = nullptr;
	_mapRooms = nullptr;
	//logic
	int _occ = 0;

	// setup view (specific for super isaac bros)
	_view = new View(this, _rect);
	_view->setFixedAspectRatio(Game::instance()->aspectRatio());
	_view->setRect(RectF(0, 0, 16, 12));

	_schedule = new Scheduler();

	_name = "gamescene";
}

void GameScene::update(float timeToSimulate)
{

	Scene::update(timeToSimulate);
	_schedule->update(timeToSimulate);

	if (!_active)
		return;

	// physics
	_timeToSimulate += timeToSimulate;
	while (_timeToSimulate >= _dt)
	{
		for (auto& layer : _sortedObjects)
			for (auto& obj : layer.second)
				if (!obj->freezed())
					obj->update(_dt);

		_timeToSimulate -= _dt;
	}

	_view->update(timeToSimulate); //aggiunta la modalità update a view, per i movimenti precisi da una stanza all'altra

	// _moveView
	if (_moveView)
	{
		float pos_x = _view->rect().pos.x;
		float pos_y = _view->rect().pos.y;
		if (_right_pressed && !_left_pressed)
			_view->move({ 1,0 });
		if (_left_pressed && !_right_pressed)
			_view->move({ -1,0 });
		if (_up_pressed && !_down_pressed)
			_view->move({ 0,-1 });
		if (_down_pressed && !_up_pressed)
			_view->move({ 0,1 });
		return;
	}


	// is _viewMoving   or    showMonster?
	if (_view->getDir() != Direction::NONE || Game::instance()->uiMonster()->show())
		return;
	else {
		_moveIsaac = true;
	}

	// Isaac
	if (_moveIsaac)
	{
		if (!_isaac)
			return;

		// move
		if (_d_pressed && !_a_pressed)
			_isaac->move_x(Direction::RIGHT);
		else if (_a_pressed && !_d_pressed)
			_isaac->move_x(Direction::LEFT);
		else
			_isaac->move_x(Direction::NONE);

		if (_w_pressed && !_s_pressed)
			_isaac->move_y(Direction::UP);
		else if (_s_pressed && !_w_pressed)
			_isaac->move_y(Direction::DOWN);
		else
			_isaac->move_y(Direction::NONE);

		// shoot
		if (_right_pressed && !_left_pressed)
			_isaac->shoot(Direction::RIGHT);
		else if (_left_pressed && !_right_pressed)
			_isaac->shoot(Direction::LEFT);
		else if (_up_pressed && !_down_pressed)
			_isaac->shoot(Direction::UP);
		else if (_down_pressed && !_up_pressed)
			_isaac->shoot(Direction::DOWN);
	}

	float _view_x = _view->rect().pos.x;
	float _view_y = _view->rect().pos.y;
	float _isaac_x = _isaac->rect().pos.x;
	float _isaac_y = _isaac->rect().pos.y;

	if (_isaac_x > 13.8f + _view_x || _isaac_x < 1.1f + _view_x || _isaac_y > 9.5f + _view_y || _isaac_y < 0.8f + _view_y)
	{
		_moveIsaac = false;
		_isaac->setVelX(0);
		_isaac->setVelY(0);
		_isaac->move_x(Direction::NONE);
		_isaac->move_y(Direction::NONE);

		//_room->openCloseDoor();

		if (_isaac_x > 13.8f + _view_x)
		{
			_view->moveTransition(Direction::RIGHT); //cambiamenti dentro view per permettere la transizione della vista tra una stanza e l'altra
			_isaac->moveBy({ 4.3f,0 });
			setRooms(std::make_pair( 1,0 ));
		}
		else if (_isaac_x < 1.1f + _view_x)
		{
			_view->moveTransition(Direction::LEFT);
			_isaac->moveBy({ -4.2f,0 });
			setRooms(std::make_pair(-1, 0));
		}
		else if (_isaac_y > 9.5f + _view_y)
		{
			_view->moveTransition(Direction::DOWN);
			_isaac->moveBy({ 0,4.0f });
			setRooms(std::make_pair(0, 1));
		}
		else if (_isaac_y < 0.8f + _view_y)
		{
			_view->moveTransition(Direction::UP);
			_isaac->moveBy({ 0,-4.2f });
			setRooms(std::make_pair(0, -1));
		}

		// SPAWN ENEMIES IN NEW ROOM
		spawnMobs();
	}

	//CHECK NOT ENEMIES IN ROOM
	_enemiesInRoom.clear();
	for (auto& obj : objects(_view->rect()))
		if (auto enemy = obj->to<Enemy*>())
			_enemiesInRoom.push_back(enemy);
	if (_enemiesInRoom.size() == 0 && _room->state() == RoomState::COMBAT) 
		_room->changeStateRoom();
	if (_enemiesInRoom.empty() && _room->state() == RoomState::COMBAT)
		_room->changeStateRoom();
	for (auto& obj : objects(_view->rect()))
	{
		if (auto enemy = obj->to<Enemy*>())
		{
			if (!_view->rect().intersects(enemy->rect()))
			{
				std::cout << "intersezione\n";	
			}
		}
	}
	Game::instance()->hud()->selectMinimapRoom(_isaac_x, _isaac_y);
}

void GameScene::spawnMobs()
{
	if (_room->type() == RoomType::NORMAL && _room->state() == RoomState::ACTIVE)
	{
		if (rand()%2 == 0)
		{
			new Gusher(this, PointF(this->room()->rect().center().x, this->room()->rect().center().y), 2.5f);
			int amount = rand() % 6;
			if (amount)
				_room->changeStateRoom();
			else
				_room->setState(RoomState::INACTIVE);
		}
		else
		{
			Host* newHost = new Host(this, PointF(this->room()->rect().center().x, this->room()->rect().center().y), 1.5f);

			_room->changeStateRoom();
		}
		}
		else if (_room->type() == RoomType::TREASURE && _room->state() == RoomState::ACTIVE)
		{

		}
		else if (_room->type() == RoomType::SHOP)
		{
			new Heart(this, PointF(this->room()->rect().pos.x + 8 - 1.4f / 2, this->room()->rect().pos.y + 6.0f), 1);
		}
		else if (_room->type() == RoomType::BOSS && _room->state() == RoomState::ACTIVE)
		{
			if (_vsMonster)
			{
				Game::instance()->uiMonster()->setActiveUIMonster();
				_vsMonster = false;

				_schedule = new Scheduler(1.8f, [this]()
					{
						_room->changeStateRoom();
						//_room->offLightDoor(); non funge
						_duke = new Duke(this, PointF(this->room()->rect().center().x - 1, this->room()->rect().center().y - 1), 6.5f);
					});
			}
		}
}

void GameScene::event(SDL_Event& evt)
{

	Scene::event(evt);

	if (_isaac->died() == true)
		if (evt.type == SDL_KEYDOWN && (evt.key.keysym.scancode == SDL_SCANCODE_ESCAPE))
			Game::instance()->quit();
		else if (evt.type == SDL_KEYDOWN && (evt.key.keysym.scancode == SDL_SCANCODE_SPACE))
			Game::instance()->reset();

	if (evt.type == SDL_KEYDOWN && (evt.key.keysym.scancode == SDL_SCANCODE_ESCAPE))
		Game::instance()->pushScene(Menu::pauseMenu());
	else if (evt.type == SDL_KEYDOWN && evt.key.keysym.scancode == SDL_SCANCODE_H)
		_isaac->die();
	else if (evt.type == SDL_KEYDOWN && evt.key.keysym.scancode == SDL_SCANCODE_C && !evt.key.repeat)
		toggleColliders();
	else if (evt.type == SDL_KEYDOWN && evt.key.keysym.scancode == SDL_SCANCODE_V && !evt.key.repeat)
		Game::instance()->toggleBorders();
	else if (evt.type == SDL_MOUSEBUTTONDOWN)
	{
		PointF mousePoint(float(evt.button.x), float(evt.button.y));
		mousePoint = _view->mapToScene(mousePoint);

		if (evt.button.button == SDL_BUTTON_RIGHT)
		{
			for (auto& obj : objects(_view->rect()))
				if (obj->contains(mousePoint))
				{
					std::cout << obj->name() << std::endl;
					killObject(obj);
				}
		}
		else if (evt.button.button == SDL_BUTTON_LEFT)
		{
			for (auto& obj : objects(_view->rect()))
				if (obj->contains(mousePoint))
				{
					std::cout << obj->name() << " " << obj->layer() << std::endl;
				}
		}
	}
	else if (evt.type == SDL_KEYDOWN && evt.key.keysym.scancode == SDL_SCANCODE_I)
	{
		_isaac->setInvincible();
		std::cout << "invincible\n";
	}
	else if (evt.type == SDL_KEYDOWN && evt.key.keysym.scancode == SDL_SCANCODE_O)
	{
		_room->openCloseDoor();
	}
	else if (evt.type == SDL_KEYDOWN && evt.key.keysym.scancode == SDL_SCANCODE_P)
	{
		if (_moveView)
		{
			_view->setRect(_room->rect());
			_moveView = false;
			_moveIsaac = true;
		}
		else
		{
			_moveView = true;
			_moveIsaac = false;
		}
	}
	else if (evt.type == SDL_KEYDOWN && evt.key.keysym.scancode == SDL_SCANCODE_L)
	{
		Game::instance()->hud()->showMinimap();
	}
	else if (evt.type == SDL_MOUSEWHEEL)
	{
		if (_moveView)
		{
			if (evt.wheel.y > 0)
				_view->scale(1 - _cameraZoomVel);
			else if (evt.wheel.y < 0)
				_view->scale(1 + _cameraZoomVel);
		}
	}

	// update control flags
	const Uint8* state = SDL_GetKeyboardState(0);
	_d_pressed = state[SDL_SCANCODE_D];
	_a_pressed = state[SDL_SCANCODE_A];
	_w_pressed = state[SDL_SCANCODE_W];
	_s_pressed = state[SDL_SCANCODE_S];
	_run_pressed = state[SDL_SCANCODE_Z];

	_right_pressed = state[SDL_SCANCODE_RIGHT];
	_left_pressed = state[SDL_SCANCODE_LEFT];
	_up_pressed = state[SDL_SCANCODE_UP];
	_down_pressed = state[SDL_SCANCODE_DOWN];
}