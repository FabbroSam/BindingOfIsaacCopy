#include "Fly.h"
#include "SpriteFactory.h"
#include "StaticObject.h"
#include "GameScene.h"
#include "Mario.h"
#include <random>
#include <iostream>
#include "Room.h"
#include "Tear.h"
#include "Rock.h"
#include "Poop.h"
#include "Isaac.h"

using namespace agp;

Fly::Fly(Scene* scene, const PointF& pos, float spawnDelay)
	:Enemy(scene, RectF(pos.x, pos.y, 1.1f, 1.1f), nullptr, spawnDelay, 10)
{

	_sprites["fly_black"] = SpriteFactory::instance()->get("fly_black");
	_sprites["fly_red"] = SpriteFactory::instance()->get("fly_red");
	_sprites["dyingFly"] = SpriteFactory::instance()->get("dyingFly");

	_collider.adjust(0.35f, 0.25f, -0.32f, 0.3f);
	_visible = false;
	_collidable = true;
	_compenetrable = false;

	//physics
	_x_acc = 1.0f;
	_y_acc = 1.0f;
	_x_dec_rel = 0;
	_y_dec_rel = 0;

	_vel.y = 1;
	_vel.x = 1;
	_x_vel_max = 0.5f;
	_y_vel_max = 0.5f;

	_x_dir = Direction::NONE;
	_y_dir = Direction::NONE;

	// game parameters
	_life = 1.2f;

	_black = rand() % 2;
	if (_black)
	{
		_sprite = _sprites["fly_black"];
		_distFromIsaac = 2;
	}
		
	else
	{
		_sprite = _sprites["fly_red"];
		_distFromIsaac = 16;
	}

}

void::Fly::update(float dt)
{
	Enemy::update(dt);

	_shadow->setRect(_rect * Vec2Df(0.35f, 0.15f) + Vec2Df(0.4f, 1.1f));

	Isaac* isaac = static_cast<GameScene*>(_scene)->player();
	
	Vec2Df centerIsaac = isaac->rect().center();
	Vec2Df centerFly = _rect.center();

	float dist = centerFly.distance(centerIsaac);
	if (dist < _distFromIsaac )
	{
		followIsaac(centerIsaac);
	}
	else
	{
		move();
	}

}

void Fly::followIsaac(Vec2Df pos)
{
	if (!_movable)
	{
		return;
	}

	_x_acc = 5;
	_y_acc = 5;

	if (_black)
	{
		_x_vel_max = 1.3f;
		_y_vel_max = 1.3f;
	}
	else
	{
		_x_vel_max = 2.2f;
		_y_vel_max = 2.2f;
	}

	Direction x_dir;
	Direction y_dir;
	
	if (pos.x - 0.5f < _rect.pos.x)
	{
		x_dir = Direction::LEFT;
	}
	else
		x_dir = Direction::RIGHT;

	if (pos.y - 0.7f < _rect.pos.y)
	{
		y_dir = Direction::UP;
	}
	else
		y_dir = Direction::DOWN;

	schedule("dirchange", 0.05f,[this,x_dir,y_dir]() {
		if (x_dir != _x_dir)
		{
			_y_dir = Direction::NONE;
			_x_dir = x_dir;
		}
		else if (y_dir != _y_dir)
		{
			_x_dir = Direction::NONE;
			_y_dir = y_dir;
		}
		}, 0, false);

}

bool Fly::collidableWith(CollidableObject* obj)
{
	if (obj->to<Rock*>() || obj->to<Poop*>())
		return false;
	return true;
}

void Fly::move()
{
	if (!_movable)
	{
		unschedule("movement");
		unschedule("impulse");
		return;
	}

	_x_vel_max = 0.9f;
	_y_vel_max = 0.9f;
	_x_acc = 3;
	_y_acc = 3;

	schedule("movement", 0.07f, [this]() 
		{
			_x_dir = rand() % 100 > 20 ? Direction::NONE : (rand() % 100 > 50 ? Direction::LEFT : Direction::RIGHT);
			_y_dir = rand() % 100 > 20 ? Direction::NONE : (rand() % 100 > 50 ? Direction::UP : Direction::DOWN);
		},0,false);
	//schedule("impulse", 0.1f, [this]()
	//	{
	//		_x_dir = _x_dir != Direction::NONE ? (rand() % 100 > 50 ? Direction::LEFT : Direction::RIGHT) : Direction::NONE;
	//		_y_dir = _y_dir != Direction::NONE ? (rand() % 100 > 50 ? Direction::LEFT : Direction::RIGHT) : Direction::NONE;
	//	}, 0, false);

}


void Fly::hit(float damage, Vec2Df _dir)
{	

	_movable = false;
	_vel = { 0,0 };

	_x_vel_max = 10;
	_y_vel_max = 10;
	_x_acc = 50.0f;
	_y_acc = 50.0f;

	if (_dir.x < 0)
		_x_dir = Direction::LEFT;
	else
		_x_dir = Direction::RIGHT;

	if (_dir.y < 0)
		_y_dir = Direction::UP;
	else
		_y_dir = Direction::DOWN;

	schedule("friction", 0.05f, [this]() { _movable = true; });

	Enemy::hit(damage);
}


void Fly::die()
{
	_collidable = false;
	_vel = { 0,0 };

	_sprite = _sprites["dyingFly"];

	schedule("dyingFlyAnimation", 0.25f, [this]() {
		_scene->killObject(_shadow);
		_scene->killObject(this);
		}, 0, false);
}