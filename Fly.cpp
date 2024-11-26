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

using namespace agp;

Fly::Fly(Scene* scene, const PointF& pos, float spawnDelay)
	:Enemy(scene, RectF(pos.x, pos.y, 30/16, 30/16), nullptr, spawnDelay, 5)
{
	_accumulator = 0;
	_x_acc = 1.0f;
	_y_acc = 1.0f;

	_x_dec_rel = 0;
	_y_dec_rel = 0;

	_x_dir = Direction::NONE;
	_y_dir = Direction::NONE;

	_sprites["fly"] = SpriteFactory::instance()->get("fly");
	_sprites["dyingFly"] = SpriteFactory::instance()->get("dyingFly");
	_sprite = _sprites["fly"];

	//_collider.adjust(0.4f,0.28f,0,0.1f);
	//setCollider(_rect);

	//_visible = false;
	_collidable = true;
	_compenetrable = false;

	_vel.y = 1;
	_vel.x = 1;
	_x_vel_max = 0.5f;
	_y_vel_max = 0.5f;

	// game parameters
	_life = 1.5f;

}



void::Fly::update(float dt)
{
	MovableObject::update(dt);

	_shadow->setRect(_rect * Vec2Df(0.35f, 0.15f) + Vec2Df(0.4f, 0.9f));

	if (_movable)
		move();
}


bool Fly::collision(CollidableObject* with, Direction fromDir)
{
	Enemy::collision(with, fromDir);
	return true;
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

	schedule("movement", 0.2f, [this]() 
		{
			_x_vel_max = 0.5f;
			_y_vel_max = 0.5f;
			_x_acc = 1;
			_y_acc = 1;
			_x_dec_rel = 0;
			_y_dec_rel = 0;

			_x_dir = rand() % 100 > 30 ? Direction::NONE : (rand() % 100 > 50 ? Direction::LEFT : Direction::RIGHT);
			_y_dir = rand() % 100 > 30 ? Direction::NONE : (rand() % 100 > 50 ? Direction::UP : Direction::DOWN);


		},0,false);


	schedule("impulse", 0.1f, [this]()
		{

			_x_acc = 1;
			_y_acc = 1;
			_x_dec_rel = 0;
			_y_dec_rel = 0;

			_x_dir = _x_dir == Direction::NONE ? (rand() % 100 > 50 ? Direction::LEFT : Direction::RIGHT) : Direction::NONE;
			_y_dir = _y_dir == Direction::NONE ? (rand() % 100 > 50 ? Direction::LEFT : Direction::RIGHT) : Direction::NONE;


		}, 0, false);
}


void Fly::hit(float damage, Vec2Df _dir)
{	
	_movable = false;

	_vel = { 0,0 };

	_x_vel_max = 2;
	_y_vel_max = 2;
	_x_acc = 3.0f;
	_y_acc = 3.0f;

	if (_dir.x < 0)
		_x_dir = Direction::LEFT;
	else
		_x_dir = Direction::RIGHT;

	if (_dir.y < 0)
		_y_dir = Direction::UP;
	else
		_y_dir = Direction::DOWN;

	schedule("friction", 0.1f, [this]() { _movable = true; });

	
}


void Fly::die()
{
	_collidable = false;
	_sprite = _sprites["dyingFly"];

	_x_dir = Direction::NONE;
	_y_dir = Direction::NONE;

	schedule("dyingFlyAnimation", 0.25f, [this]() {
		_scene->killObject(_shadow);
		_scene->killObject(this);

		}, 0, false);
}