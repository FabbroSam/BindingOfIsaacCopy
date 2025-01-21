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
	:Enemy(scene, RectF(pos.x, pos.y, 1.5f, 1.9f), nullptr, spawnDelay, 5)
{

	_sprites["host_0"] = SpriteFactory::instance()->get("host_0");
	_sprites["host_1"] = SpriteFactory::instance()->get("host_1");
	_sprites["host_2"] = SpriteFactory::instance()->get("host_2");
	_sprites["bloodExplotion"] = SpriteFactory::instance()->get("bloodExplotion");
	_sprites["blood"] = SpriteFactory::instance()->get("blood");

	//setRect(_rect * Vec2Df(1.0f, 1.1f));
	_collider.adjust(0.4f, 1.3f, -0.4f, 0.0f);
	_visible = false;
	_collidable = true;
	_compenetrable = false;
	_movable = false;

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
	Isaac* isaac = static_cast<GameScene*>(_scene)->player();

	_shadow->setRect(_rect * Vec2Df(0.8f, 0.3f) + Vec2Df(0.18f, 1.6f));

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
			_canShoot = true;
			_hitable = true;
			_sprite = _sprites["host_1"];
			
			if (!_shooting)
			{
				schedule("shoot", 0.1f, [this]() 
				{
					rand() % 2 ? shoot(Direction::RIGHT) : shoot(Direction::LEFT);
				});
				_shooting = true;
				_canShoot = false;
			}
		}

		else if (_accumulator <= 4.85f)
		{
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

void Host::shoot(Direction dir)
{
	PointF spawnPoint;
	spawnPoint.x = _rect.pos.x + 0.5f;
	spawnPoint.y = _rect.pos.y - 0.2f;

	if (_canShoot)
	{
		switch (dir) 
		{
			case Direction::LEFT:
				new Tear(_scene, spawnPoint, dir, 0.1, 0.1, true);
				new Tear(_scene, spawnPoint + PointF(0, 0.5f), dir, 0.1, 0.1, true);
				new Tear(_scene, spawnPoint + PointF(0, -0.5f), dir, 0.1, 0.1, true);
				break;
			case Direction::RIGHT:
				new Tear(_scene, spawnPoint, dir, 0.1, 0.1, true);
				new Tear(_scene, spawnPoint + PointF(0, 0.5f), dir, 0.1, 0.1, true);
				new Tear(_scene, spawnPoint + PointF(0, -0.5f), dir, 0.1, 0.1, true);
				break;
			case Direction::UP:
				new Tear(_scene, spawnPoint, dir, 0.1, 0.1, true);
				new Tear(_scene, spawnPoint + PointF(0.5f, 0), dir, 0.1, 0.1, true);
				new Tear(_scene, spawnPoint + PointF(-0.5f, 0), dir, 0.1, 0.1, true);
				break;
			case Direction::DOWN:
				new Tear(_scene, spawnPoint, dir, 0.1, 0.1, true);
				new Tear(_scene, spawnPoint + PointF(0.5f, 0), dir, 0.1, 0.1, true);
				new Tear(_scene, spawnPoint + PointF(-0.5f, 0), dir, 0.1, 0.1, true);
				break;
			default:
				break;
		}
	}
}

void Host::hit(float damage, Vec2Df _dir)
{
	Enemy::hit(damage);
}

void Host::die()
{
	_dying = true;
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