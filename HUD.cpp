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

HUD* HUD::instance()
{
	static HUD uniqueInstance;
	return &uniqueInstance;
}

HUD::HUD()
	: UIScene(RectF(0, 0, 16, 12))
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

	// _FPS
	_FPS = new RenderableObject(this, RectF(5, 1.1f, 2.0f, 0.5f), SpriteFactory::instance()->getNumber(0, { 0.4,0.4 }));

	// HEARTS
	_heart1 = new RenderableObject(this, RectF(1.0f, 1.0f, 0.7f, 0.7f), _sprites["heart_red"]);
	_heart2 = new RenderableObject(this, RectF(1.5f, 1.0f, 0.7f, 0.7f), _sprites["heart_half_red"]);
	_heart3 = new RenderableObject(this, RectF(2.0f, 1.0f, 0.7f, 0.7f), _sprites["heart_empty"]);
	_hearts.push_back(_heart1);
	_hearts.push_back(_heart2);
	_hearts.push_back(_heart3);
	_totalHearts = 3.0f;

	//// COIN - ITEM
	_coin = new RenderableObject(this, RectF(0.25f, 2, 0.7f, 0.8f), _sprites["coin"]);
	new RenderableObject(this, RectF(1, 2.25f, 0.8f, 0.5f), SpriteFactory::instance()->getNumber(98,{0.4,0.4}));
	_bomb = new RenderableObject(this, RectF(0.25f, 2.6f, 0.7f, 0.8f), _sprites["bomb"]);
	new RenderableObject(this, RectF(1, 2.85f, 0.8f, 0.5f), SpriteFactory::instance()->getNumber(50,{ 0.4,0.4 }));
	
	// MINIMAP
	new RenderableObject(this, RectF(13, 1, 2.5f, 2.5f), _sprites["minimap_back"]);
	_roomSelected = new MovableObject(this, RectF(0, 0, 2.5f / 7.0f, 2.5f / 7.0f), _sprites["minimap_room_select"],1);
	std::cout << "hud.cpp obj: " << _roomSelected->name() << std::endl;

	_pos = { -1,-1 };
	_name = "hud";
}

// extends update logic (+time management)
void HUD::update(float timeToSimulate)
{
	UIScene::update(timeToSimulate);

	if (!_active)
		return;

	float tempHearts = _totalHearts;
	for (const auto& heart : _hearts) {

		if (tempHearts >= 1) {
			heart->setSprite(SpriteFactory::instance()->get("hud_heart_red"));
			tempHearts -= 1;
		}
		else if (tempHearts >= 0.5) {
			heart->setSprite(SpriteFactory::instance()->get("hud_heart_half_red"));
			tempHearts -= 0.5;
		}
		else 
			heart->setSprite(SpriteFactory::instance()->get("hud_heart_empty"));
	}

	_roomSelected->setRect(RectF(_pos.x * 2.5f / 7.0f + 13.0f + 2.5f * 3.0f / 7.0f, _pos.y * 2.5f / 7.0f + 1.0f + 2.5f * 3.0f / 7.0f, 2.5f / 7.0f, 2.5f / 7.0f));
	_roomsMinimap[{_pos.x, _pos.y}]->setVisible(true);

}

void HUD::setHearts(float amount)
{ 
	// amount può essere negativa o positiva (toglie o aggiunge vita)
	_totalHearts += amount;
}

void HUD::setFPS(float fps)
{
	_FPS->setSprite(SpriteFactory::instance()->getNumber(fps, { 0.4,0.4 }));
}

void HUD::drawMinimap(RectF rect, RoomType roomType)
{
	RenderableObject* temp;
	if (roomType == RoomType::BOSS)
		temp = new RenderableObject(this, RectF(rect.pos.x * 2.5f / 7.0f + 13.0f + 2.5f * 3.0f / 7.0f, rect.pos.y * 2.5f / 7.0f + 1.0f + 2.5f * 3.0f / 7.0f, 2.5f / 7.0f, 2.5f / 7.0f), _sprites["minimap_boss"]);
	else if (roomType == RoomType::TREASURE)
		temp = new RenderableObject(this, RectF(rect.pos.x * 2.5f / 7.0f + 13.0f + 2.5f * 3.0f / 7.0f, rect.pos.y * 2.5f / 7.0f + 1.0f + 2.5f * 3.0f / 7.0f, 2.5f / 7.0f, 2.5f / 7.0f), _sprites["minimap_treasure"]);
	else if (roomType == RoomType::SHOP)
		temp = new RenderableObject(this, RectF(rect.pos.x * 2.5f / 7.0f + 13.0f + 2.5f * 3.0f / 7.0f, rect.pos.y * 2.5f / 7.0f + 1.0f + 2.5f * 3.0f / 7.0f, 2.5f / 7.0f, 2.5f / 7.0f), _sprites["minimap_shop"]);
	else 
		temp = new RenderableObject(this, RectF(rect.pos.x * 2.5f / 7.0f + 13.0f + 2.5f * 3.0f / 7.0f, rect.pos.y * 2.5f / 7.0f + 1.0f + 2.5f * 3.0f / 7.0f, 2.5f / 7.0f, 2.5f / 7.0f), _sprites["minimap_room"]);
	
	temp->setVisible(false);
	_roomsMinimap[{rect.pos.x, rect.pos.y}] = temp;
}

void HUD::selectMinimapRoom(float x, float y)
{
	int xx = static_cast<int>(std::floor(x/16));
	int yy = static_cast<int>(std::floor(y/12));
	
	if (Vec2D<int>({ xx,yy }) != _pos)
	{
		_pos = Vec2D<int>({ xx,yy });
	}
}

void HUD::showMinimap()
{
	for (const auto& room : _roomsMinimap)
	{
		room.second->setVisible(true);
	}
}

