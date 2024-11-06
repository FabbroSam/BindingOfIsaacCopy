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
#include "Mario.h"
#include "Door.h"
#include "HUD.h"
#include <iostream>
using namespace agp;

GameScene::GameScene(const RectF& r, float dt)
	: Scene(r)
{
	_dt = dt;
	_timeToSimulate = 0;
	_mario = nullptr;
	_cameraZoomVel = 0.1f;
	_a_pressed = false;
	_d_pressed = false;
	_w_pressed = false;
	_s_pressed = false;
	_run_pressed = false;
	_collidersVisible = false;

	// setup view (specific for super mario bros)
	_view = new View(this, _rect);
	_view->setFixedAspectRatio(Game::instance()->aspectRatio());
	_view->setRect(RectF(0, 0, 16, 12));

	_right_pressed = false;
	_left_pressed = false;
	_up_pressed = false;
	_down_pressed = false;


	// move view free
	_moveView = false;
}

void GameScene::update(float timeToSimulate)
{
	Scene::update(timeToSimulate);

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

	// move Mario
	if (_d_pressed && !_a_pressed)
		_mario->move_x(Direction::RIGHT);
	else if (_a_pressed && !_d_pressed)
		_mario->move_x(Direction::LEFT);
	else
		_mario->move_x(Direction::NONE);

	if(_w_pressed && !_s_pressed)
		_mario->move_y(Direction::UP);
	else if (_s_pressed && !_w_pressed)
		_mario->move_y(Direction::DOWN);
	else
		_mario->move_y(Direction::NONE);
	_mario->run(_run_pressed);


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
	}
	else
	{
		// move _view with mario position
		float _view_x = _view->rect().pos.x;
		float _view_y = _view->rect().pos.y;
		float _mario_x = _mario->rect().pos.x;
		float _mario_y = _mario->rect().pos.y;
		if (_mario_x > 13.8 + _view_x)
		{
			_view->move({ 16,0 });
			_mario->moveBy({ 3.6,0 });
		}
		else if (_mario_x < 1.1 + _view_x)
		{
			_view->move({ -16,0 });
			_mario->moveBy({ -3.6,0 });
		}
		else if (_mario_y > 9.5 + _view_y)
		{
			_view->move({ 0,12 });
			_mario->moveBy({ 0,3.6 });
		}
		else if (_mario_y < 0.8 + _view_y)
		{
			_view->move({ 0,-12 });
			_mario->moveBy({ 0,-3.6 });
		}

		HUD::instance()->selectMinimapRoom(_mario_x,_mario_y);
	}
}

void GameScene::event(SDL_Event& evt)
{
	Scene::event(evt);

	if (evt.type == SDL_KEYDOWN && (evt.key.keysym.scancode == SDL_SCANCODE_RETURN || evt.key.keysym.scancode == SDL_SCANCODE_ESCAPE))
		Game::instance()->pushScene(Menu::pauseMenu());
	else if (evt.type == SDL_KEYDOWN && evt.key.keysym.scancode == SDL_SCANCODE_H)
		_mario->die();
	else if (evt.type == SDL_KEYDOWN && evt.key.keysym.scancode == SDL_SCANCODE_C && !evt.key.repeat)
		toggleColliders();
	else if (evt.type == SDL_MOUSEBUTTONDOWN)
	{
		PointF mousePoint(float(evt.button.x), float(evt.button.y));
		mousePoint = _view->mapToScene(mousePoint);

		if (evt.button.button == SDL_BUTTON_RIGHT)
		{
			for (auto& obj : objects(_view->rect()))
				if (obj->contains(mousePoint))
					killObject(obj);
		}
		else if (evt.button.button == SDL_BUTTON_LEFT)
			new RenderableObject(this, RectF(floor(mousePoint.x), floor(mousePoint.y), 1, 1), SpriteFactory::instance()->get("brick"));
	}
	else if (evt.type == SDL_KEYDOWN && evt.key.keysym.scancode == SDL_SCANCODE_O)
	{
		for (auto& obj : objects(_view->rect()))
		{
			if (obj->name().rfind("Door", 0) == 0)
				dynamic_cast<Door*>(obj)->Trigger();
		}
	}
	else if (evt.type == SDL_KEYDOWN && evt.key.keysym.scancode == SDL_SCANCODE_P)
	{
		if (_moveView)
			_moveView = false;
		else
			_moveView = true;
	}
	else if (evt.type == SDL_KEYDOWN && evt.key.keysym.scancode == SDL_SCANCODE_L)
	{
		HUD::instance()->showMinimap();
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