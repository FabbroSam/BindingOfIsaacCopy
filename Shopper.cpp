#include "Shopper.h"
#include "SpriteFactory.h"
#include "HUD.h"
#include "Isaac.h"
#include "Heart.h"
#include "Game.h"
#include "Audio.h"

using namespace agp;

Shopper::Shopper(Scene* scene, const PointF& pos, int layer)
	: StaticObject(scene, RectF(pos.x, pos.y, 1.8f, 1.8f), nullptr, 5)
{

	_rect = RectF(pos.x + 8 - 1.0f, pos.y + 6 - 1.5f, 2.2f, 2.2f);
	_scene = scene;
	_sprites["shopper"] = SpriteFactory::instance()->get("shopper");
	_sprites["shopper_use"] = SpriteFactory::instance()->get("shopper_use");
	_sprite = _sprites["shopper"];

	_use = false;
}

void Shopper::update(float dt)
{
	StaticObject::update(dt);
}

bool Shopper::collision(CollidableObject* with, Direction fromDir)
{
	HUD* hud = Game::instance()->hud();
	Isaac* isaac = dynamic_cast<Isaac*>(with);
	if (isaac && hud->coins() >= 1 && !_use)
	{
		_use = true;
		_sprite = _sprites["shopper_use"];
		Audio::instance()->playSound("coin slot");
		hud->decreaseCoins();

		schedule("shopper2", 1.0f, [this]() {
			_sprite = _sprites["shopper"];
			_use = false;
			new Heart(_scene, PointF(_rect.center().x - 0.9, _rect.center().y + 0.8), 1);
			}, 0, false);
		return true;
	}

	return false;
}