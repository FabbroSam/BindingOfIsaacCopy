// ----------------------------------------------------------------
// From "Algorithms and Game Programming" in C++ by Alessandro Bria
// Copyright (C) 2024 Alessandro Bria (a.bria@unicas.it). 
// All rights reserved.
// 
// Released under the BSD License
// See LICENSE in root directory for full details.
// ----------------------------------------------------------------

#include "HUD.h"
#include "SpriteFactory.h"
#include "View.h"
#include "Game.h"
#include <iostream>
#include <cmath> 

using namespace agp;

HUD::HUD()
	: Scene(RectF(0, 0, 16, 12))
{

	// setup view 
	_view = new View(this, _rect);
	_view->setFixedAspectRatio(Game::instance()->aspectRatio());
	_view->setRect(RectF(0, 0, 16, 12));

	setBackgroundColor(Color(0, 0, 0, 0));

	_sprites["heart_red"] = SpriteFactory::instance()->get("hud_heart_red");
	_sprites["heart_half_red"] = SpriteFactory::instance()->get("hud_heart_half_red");
	_sprites["heart_empty"] = SpriteFactory::instance()->get("hud_heart_empty");
	_sprites["coin"] = SpriteFactory::instance()->get("hud_coin");
	_sprites["bomb"] = SpriteFactory::instance()->get("hud_bomb");
	_sprites["minimap_back"] = SpriteFactory::instance()->get("hud_minimap_back");
	_sprites["minimap_room"] = SpriteFactory::instance()->get("hud_minimap_room");
	_sprites["minimap_boss"] = SpriteFactory::instance()->get("hud_minimap_boss");
	_sprites["minimap_treasure"] = SpriteFactory::instance()->get("hud_minimap_treasure");
	_sprites["minimap_shop"] = SpriteFactory::instance()->get("hud_minimap_shop");
	_sprites["minimap_room_select"] = SpriteFactory::instance()->get("hud_minimap_room_select");

	_bombs = -1;
	_coins = -1;
	_halfHearts = -1;
	_heartsCapacity = 3;

	// _FPS
	_fpsText = new RenderableObject(this, RectF(13.0f, 0.5f, 2.0f, 0.5f), SpriteFactory::instance()->getNumber(0, 4));

	// HEARTS
	for (int i = 0; i < 5; i++)
		_heartIcons[i] = new RenderableObject(this, RectF(1.8f + i*0.55f, 0.9f, 0.7f, 0.7f), nullptr);

	// COIN - ITEM
	new RenderableObject(this, RectF(0.25f, 2.0f, 0.7f, 0.8f), _sprites["coin"]);
	_coinsText = new RenderableObject(this, RectF(0.9f, 2.25f, 0.8f, 0.5f), nullptr);
	new RenderableObject(this, RectF(0.25f, 2.6f, 0.7f, 0.8f), _sprites["bomb"]);
	_bombsText = new RenderableObject(this, RectF(0.9f, 2.85f, 0.8f, 0.5f), nullptr);
	
	// MINIMAP
	new RenderableObject(this, RectF(13.0f, 1.0f, 2.5f, 2.5f), _sprites["minimap_back"]);
	_roomSelected = new MovableObject(this, RectF(0, 0, 2.5f / 7.0f, 2.5f / 7.0f), _sprites["minimap_room_select"],1);

	_pos = { -1,-1 };
	_name = "HUD";

	setCoins(0);
	setBombs(0);
	setHalfHearts(6);

}

// extends update logic (+time management)
void HUD::update(float timeToSimulate)
{
	Scene::update(timeToSimulate);
}

void HUD::refreshHearts()
{
	int fullHearts = _halfHearts / 2;
	int halfHearts = _halfHearts % 2;
	for (int i = 0; i < 3; i++)
	{
		if (i < fullHearts)
			_heartIcons[i]->setSprite(SpriteFactory::instance()->get("hud_heart_red"));
		else if (halfHearts && i == fullHearts)
			_heartIcons[i]->setSprite(SpriteFactory::instance()->get("hud_heart_half_red"));
		else if (i < _heartsCapacity)
			_heartIcons[i]->setSprite(SpriteFactory::instance()->get("hud_heart_empty"));
		else
			_heartIcons[i]->setSprite(nullptr);
	}
}

void HUD::setCoins(int newCoins)
{
	if (newCoins == _coins)
		return;

	_coins = newCoins;
	_coinsText->setSprite(SpriteFactory::instance()->getNumber(_coins, 2));
}

void HUD::setBombs(int _newBomb)
{
	_bombs += _newBomb;
	_bombsText->setSprite(SpriteFactory::instance()->getNumber(_bombs+1, 2));
}


void HUD::setHalfHearts(int newHalfHearts)
{
	if (newHalfHearts == _halfHearts)
		return;

	if (newHalfHearts > _heartsCapacity * 2)
		_halfHearts = _heartsCapacity * 2;
	else
		_halfHearts = newHalfHearts;
	refreshHearts();
}

void HUD::setHeartsCapacity(int newCapacity)
{
	if (_heartsCapacity == newCapacity)
		return;

	_heartsCapacity = newCapacity;
	refreshHearts();
}

void HUD::setFPS(int fps)
{
	_fpsText->setSprite(SpriteFactory::instance()->getNumber(fps, 4));
}

void HUD::drawMinimap(RectF rect, RoomType roomType)
{
	RectF rectTransformed(
		rect.pos.x * 2.5f / 7.0f + 13.0f + 2.5f * 3.0f / 7.0f,
		rect.pos.y * 2.5f / 7.0f + 1.0f + 2.5f * 3.0f / 7.0f,
		2.5f / 7.0f,
		2.5f / 7.0f
	);

	const std::string& sprite =
		(roomType == RoomType::BOSS) ? "minimap_boss" :
		(roomType == RoomType::TREASURE) ? "minimap_treasure" :
		(roomType == RoomType::SHOP) ? "minimap_shop" :
		"minimap_room";

	RenderableObject* temp = new RenderableObject(this, rectTransformed, _sprites[sprite]);

	temp->setVisible(false);
	_roomsMinimap[{int(rect.pos.x), int(rect.pos.y)}] = temp;
}

void HUD::selectMinimapRoom(float x, float y)
{
	int xx = static_cast<int>(std::floor(x/16));
	int yy = static_cast<int>(std::floor(y/12));
	
	if (Vec2D<int>({ xx,yy }) != _pos)
	{
		_pos = Vec2D<int>({ xx,yy });
		if (_roomSelected)
			_roomSelected->setRect(RectF(_pos.x * 2.5f / 7.0f + 13.0f + 2.5f * 3.0f / 7.0f, _pos.y * 2.5f / 7.0f + 1.0f + 2.5f * 3.0f / 7.0f, 2.5f / 7.0f, 2.5f / 7.0f));

		if (_roomsMinimap[{_pos.x, _pos.y}])
			_roomsMinimap[{_pos.x, _pos.y}]->setVisible(true);
	}
}

void HUD::showMinimap()
{
	for (const auto& room : _roomsMinimap)
		if(room.second)
			room.second->setVisible(true);
}

void HUD::event(SDL_Event& evt)
{
	Scene::event(evt);

	if (evt.type == SDL_MOUSEBUTTONDOWN)
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
}