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

HUD::HUD()
	: UIScene(RectF(0, 0, 16, 12))
{
	setBackgroundColor(Color(0, 0, 0, 0));


	_heart1 = new RenderableObject(this, RectF(1, 1, 0.7, 0.8), SpriteFactory::instance()->get("hud_heart_red"));
	_heart2 = new RenderableObject(this, RectF(1.5, 1, 0.7, 0.8), SpriteFactory::instance()->get("hud_heart_half_red"));
	_heart3 = new RenderableObject(this, RectF(2.0, 1, 0.7, 0.8), SpriteFactory::instance()->get("hud_heart_empty"));
	_hearts[_heart1] = 1;
	_hearts[_heart2] = 1;
	_hearts[_heart3] = 1;

	// setup view (specific for super mario bros)
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

	for (const auto& pair : _hearts) { 

		if (pair.second == 1.0f) {
			pair.first->setSprite(SpriteFactory::instance()->get("hud_heart_red"));
		}
		else if (pair.second == 0.5f) {
			pair.first->setSprite(SpriteFactory::instance()->get("hud_heart_half_red"));
		}
		else if (pair.second == 0.0f) {
			pair.first->setSprite(SpriteFactory::instance()->get("hud_heart_empty"));
		}
	}


}

void HUD::setFPS(int fps) 
{ 

}