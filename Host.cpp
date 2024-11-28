#include "SpriteFactory.h"
#include "StaticObject.h"
#include "GameScene.h"
#include "Mario.h"
#include <random>
#include <iostream>
#include "Room.h"
#include "Tear.h"
#include "Host.h"
#include "Rock.h"
#include "Poop.h"
#include "Isaac.h"
#include "Fly.h"

using namespace agp;

Host::Host(Scene* scene, const PointF& pos, float spawnDelay)
	:Enemy(scene, RectF(pos.x, pos.y, 2.0f, 2.0f), nullptr, spawnDelay, 5)
{

	_sprites["host_0"] = SpriteFactory::instance()->get("host_0");
	_sprites["host_1"] = SpriteFactory::instance()->get("host_1");
	_sprites["host_2"] = SpriteFactory::instance()->get("host_2");

	_shadow->setRect(_rect * Vec2Df(0.3f, 0.1f) + Vec2Df(0,-1));

	//setRect(_rect * Vec2Df(1.0f, 1.1f));
	_collider.adjust(0.4f, 1.05f, -0.4f, 0.2f);
	_visible = false;
	_collidable = true;
	_compenetrable = false;
	_movable = false;

	//logic
	_accumulator = 0;

	//physics


	// game parameters
	_life = 5.0f;
}

void Host::update(float dt)
{
	Enemy::update(dt);

	_accumulator += dt;

	if (!_dying)
	{

		if (_accumulator <= 6.0f)
		{
			_sprite = _sprites["host_0"];
		}
		else if (_accumulator <= 6.2f)
		{
			_sprite = _sprites["host_2"];
		}
		else if (_accumulator <= 12.1f)
		{	
			_hitable = true;
			_sprite = _sprites["host_1"];
		}
		else if (_accumulator <= 12.15f)
		{
			_hitable = false;
			_sprite = _sprites["host_0"];
		}
		else if (_accumulator <= 12.25f)
		{
			_sprite = _sprites["host_2"];
		}
		else
			_accumulator = 0;
	}
}

bool Host::collidableWith(CollidableObject* obj)
{
	if (obj->to<Fly*>())
		return false;
	return true;
}

