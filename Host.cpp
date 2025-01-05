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
#include <math.h>
#include <iostream>
using namespace agp;

Host::Host(Scene* scene, const PointF& pos, float spawnDelay)
	:Enemy(scene, RectF(pos.x, pos.y, 1.3f, 1.7f), nullptr, spawnDelay, 5)
{
	

	_sprites["host_0"] = SpriteFactory::instance()->get("host_0");
	_sprites["host_1"] = SpriteFactory::instance()->get("host_1");
	_sprites["host_2"] = SpriteFactory::instance()->get("host_2");
	_sprites["bloodExplotion"] = SpriteFactory::instance()->get("bloodExplotion");

	_rect = _rect - Vec2Df({ 0.65f, 1.1f });
	setCollider(RectF(0.2f, 0.95f, 0.9f, 0.6f)); // collider abbassato
	// setCollider(RectF(0.4f, 0.95f, 0.5f, 0.6f)); // collider alzato
	std::cout << _collider << std::endl;
	_visible = false;
	_collidable = true;
	_compenetrable = false;
	_movable = false;
	_shadow->setVisible(false);

	//logic
	_accumulator = 0;
	_canShoot = true;
	_shooting = false;
	_hitable = false;
	_wobbling = false;
	_trigger = true;
	//physics


	// game parameters
	_life = 1.0f;
}

void Host::update(float dt)
{
	Enemy::update(dt);

	_shadow->setRect(_rect * Vec2Df(0.8f, 0.3f) + Vec2Df(0.13f, 1.2f));

	_accumulator += dt;

	if (!_dying)
	{

		if (_accumulator <= 3.0f)
		{
			_sprite = _sprites["host_0"];
		}
		else if (_accumulator <= 3.2f)
		{
			_sprite = _sprites["host_2"];
		}
		else if (_accumulator <= 4.8f)
		{
			setCollider(RectF(0.4f, 0.95f, 0.5f, 0.6f)); // collider alzato
			_canShoot = true;
			_hitable = true;
			_sprite = _sprites["host_1"];
			
			if (!_shooting)
			{
				schedule("shoot", 0.1f, [this]() 
				{
					shoot();
				});
				_shooting = true;
				_canShoot = false;
			}
		}

		else if (_accumulator <= 4.85f)
		{
			setCollider(RectF(0.2f, 0.95f, 0.9f, 0.6f)); // collider abbassato
			_hitable = false;
			_shooting = false;
			_sprite = _sprites["host_0"];
		}
		else if (_accumulator <= 4.95f)
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

void Host::shoot()
{
	PointF spawnPoint;
	spawnPoint.x = _rect.pos.x + 0.3f;
	spawnPoint.y = _rect.pos.y + 0.5f;

	if (_canShoot)
	{
		Isaac* isaac = static_cast<GameScene*>(_scene)->player();
		Vec2Df dir = (isaac->rect().center() - spawnPoint).norm();
		float angle = atan2(dir.y, dir.x);
		// Tear Center
		new Tear(_scene, spawnPoint, dir, 0.1f, 0.1f, true, 5);
		// Tear Up
		dir = Vec2Df(cos(angle + 0.3f), sin(angle + 0.3f));
		new Tear(_scene, spawnPoint, dir, 0.1, 0.1, true, 5);
		// Tear Down
		dir = Vec2Df(cos(angle - 0.3f), sin(angle - 0.3f));
		new Tear(_scene, spawnPoint, dir, 0.1, 0.1, true, 5);
	}
}

void Host::hit(float damage, Vec2Df _dir)
{
	Enemy::hit(damage);
}

void Host::die()
{
	_collidable = false;

	_shadow->setVisible(false);
	_sprite = _sprites["bloodExplotion"];

	if (!isSchedule("dyingHostAnimation"))
		schedule("dyingHostAnimation", 0.37f, [this]()
			{
				setVisible(false);
				new RenderableObject(_scene, _rect, _sprites["blood"], 6);
				_scene->killObject(this);

			}, 0, false);
}