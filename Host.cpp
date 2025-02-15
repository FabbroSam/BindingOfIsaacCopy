#include "SpriteFactory.h"
#include "StaticObject.h"
#include "GameScene.h"
#include <random>
#include <iostream>
#include "Room.h"
#include "Tear.h"
#include "Host.h"
#include "Rock.h"
#include "Poop.h"
#include "Isaac.h"
#include "Fly.h"
#include "Audio.h"
#include "Bomb.h"
#include <math.h>
#include <iostream>
using namespace agp;

Host::Host(Scene* scene, const PointF& pos, float spawnDelay)
	:Enemy(scene, RectF(pos.x, pos.y, 1.3f, 1.7f), nullptr, spawnDelay, 5)
{
	schedule("sound", 0.8f, [this]() 
		{
		Audio::instance()->playSound("boss lite roar 3");
		});
	_sprites["host_0"] = SpriteFactory::instance()->get("host_0");
	_sprites["host_1"] = SpriteFactory::instance()->get("host_1");
	_sprites["host_2"] = SpriteFactory::instance()->get("host_2");
	_sprites["bloodExplotion"] = SpriteFactory::instance()->get("bloodExplotion");
	_sprites["blood"] = SpriteFactory::instance()->get("blood");

	_rect = _rect - Vec2Df({ 0.65f, 1.1f });
	setCollider(RectF(0.2f, 0.95f, 0.9f, 0.6f)); // collider abbassato
	// setCollider(RectF(0.4f, 0.95f, 0.5f, 0.6f)); // collider alzato
	_visible = false;
	_collidable = true;
	_compenetrable = false;
	_movable = false;
	_shadow->setVisible(false);

	//logic
	_canShoot = true;
	_shooting = false;
	_hitable = false;
	_accumulator = 0;
	_wobbling = false;
	_trigger = true;
	_bounceCycle = 1;
	_acc = 0;
	_wobbling = false;
	_fix_pos_x = _rect.pos.x;
	_fix_pos_y = _rect.pos.y;
	_wobbleTakeVar = true;
	//physics


	// game parameters
	_life = 1.5f;
}

void Host::update(float dt)
{
	Enemy::update(dt);

	if (!_freezed)
	{
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
				trigger();
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

			if (_wobbling)
				wobble(dt);
		}
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
	Audio::instance()->playSound("blood fire 4", 0);
	rand() % 2 == 1 ? Audio::instance()->playSound("boss lite roar 3") : Audio::instance()->playSound("boss lite roar 4");
	PointF spawnPoint;
	spawnPoint.x = _rect.pos.x + 0.3f;
	spawnPoint.y = _rect.pos.y + 0.5f;

	if (_canShoot)
	{
		Isaac* isaac = static_cast<GameScene*>(_scene)->player();
		Vec2Df dir = (isaac->rect().center() - spawnPoint).norm();
		float angle = atan2(dir.y, dir.x);
		// Tear Center
		new Tear(_scene, spawnPoint, dir, 5.0f, 5.0f, true, false, 5);
		// Tear Up
		dir = Vec2Df(cos(angle + 0.3f), sin(angle + 0.3f));
		new Tear(_scene, spawnPoint, dir, 0.1, 0.1, true, false, 5);
		// Tear Down
		dir = Vec2Df(cos(angle - 0.3f), sin(angle - 0.3f));
		new Tear(_scene, spawnPoint, dir, 0.1, 0.1, true, false, 5);
	}
}

void Host::hit(float damage, Vec2Df _dir)
{
	Enemy::hit(damage);
}

void Host::trigger()
{
	_wobbling = true;
	_trigger = true;
}

void Host::wobble(float dt)
{
	if (_wobbling)
	{

		if (_wobbleTakeVar)
		{
			_fix_pos_x = _rect.pos.x;
			_fix_pos_y = _rect.pos.y;
			_x_prev_dir = _x_dir;
			_y_prev_dir = _y_dir;
			_prev_vel = _vel;
			_wobbleTakeVar = false;
		}

		if (!_wobbleTakeVar)
		{
			_vel = { 0,0 };
			_x_dir = Direction::NONE;
			_y_dir = Direction::NONE;
		}
		_acc += dt;

		if (_acc <= 0.4f)
		{
			_rect.size.x =_acc + _fixSize.x;
			_rect.pos.x = -0.5f * _acc + _fix_pos_x;
			_rect.size.y =_acc + _fixSize.y;
			_rect.pos.y = -0.5f * _acc + _fix_pos_y;
		}
		else if (_acc <= 1.2f)
		{
			_rect.size.x = 0.06f * std::sin(45.0f * (_acc - 0.5)) * std::exp(-(25.0f * (_acc - 0.5))) + _fixSize.x;
			_rect.pos.x = -0.03f * std::sin(45.0f * (_acc - 0.5)) * std::exp(-(25.0f * (_acc - 0.5))) + _fix_pos_x;
			_rect.size.y = -0.06f * std::sin(45.0f * (_acc - 0.5)) * std::exp(-(25.0f * (_acc - 0.5))) + _fixSize.y;
			_rect.pos.y = 0.03f * std::sin(45.0f * (_acc - 0.5)) * std::exp(-(25.0f * (_acc - 0.5))) + _fix_pos_y;

		}
		else
		{
			_x_dir = _x_prev_dir;
			_y_dir = _y_prev_dir;
			_vel = _prev_vel;
			_rect.size.x = _fixSize.x;
			_rect.pos.x = _fix_pos_x;
			_rect.size.y = _fixSize.y;
			_rect.pos.y = _fix_pos_y;
			_acc = 0;
			_wobbling = false;
			_wobbleTakeVar = true;
		}

	}
}

void Host::die()
{
	if (rand() % 2 == 0)
		new Bomb(_scene, PointF(_rect.pos.x, _rect.pos.y), 8);

	_dying = true;
	_collidable = false;
	
	_shadow->setVisible(false);
	_sprite = _sprites["bloodExplotion"];

	if (!isSchedule("dyingHostAnimation"))
		schedule("dyingHostAnimation", 0.37f, [this]()
			{
				setVisible(false);
				new RenderableObject(_scene, _rect, _sprites["blood"], 1);
				_scene->killObject(this);

			}, 0, false);
}