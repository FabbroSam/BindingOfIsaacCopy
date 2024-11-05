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

using namespace agp;

HUD* HUD::instance()
{
	static HUD uniqueInstance;
	return &uniqueInstance;
}

HUD::HUD()
	: UIScene(RectF(0, 0, 16, 12))
{
	setBackgroundColor(Color(0, 0, 0, 0));


	_heart1 = new RenderableObject(this, RectF(1, 1, 0.7, 0.8), SpriteFactory::instance()->get("hud_heart_red"));
	_heart2 = new RenderableObject(this, RectF(1.5, 1, 0.7, 0.8), SpriteFactory::instance()->get("hud_heart_half_red"));
	_heart3 = new RenderableObject(this, RectF(2.0, 1, 0.7, 0.8), SpriteFactory::instance()->get("hud_heart_empty"));
	_hearts.push_back(_heart1);
	_hearts.push_back(_heart2);
	_hearts.push_back(_heart3);
	_totalHearts = 1.5;

	_coin = new RenderableObject(this, RectF(0.25, 2, 0.7, 0.8), SpriteFactory::instance()->get("hud_coin"));
	_bomb = new RenderableObject(this, RectF(0.25, 2.6, 0.7, 0.8), SpriteFactory::instance()->get("hud_bomb"));

	// setup view 
	_view = new View(this, _rect);
	_view->setFixedAspectRatio(Game::instance()->aspectRatio());
	_view->setRect(RectF(0, 0, 16, 15));
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


}

void HUD::setHearts(float amount)
{ 
	// amount può essere negativa o positiva (toglie o aggiunge vita)
	_totalHearts += amount;
}

void HUD::setFPS(float fps)
{

}