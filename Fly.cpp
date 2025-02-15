#include "Fly.h"
#include "Duke.h"
#include "SpriteFactory.h"
#include "StaticObject.h"
#include "GameScene.h"
#include <random>
#include <iostream>
#include "Room.h"
#include "Tear.h"
#include "Rock.h"
#include "Poop.h"
#include "Isaac.h"

using namespace agp;

Fly::Fly(Scene* scene, const PointF& pos, float spawnDelay, bool duke_fly)
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
	_x_acc = 0;
	_y_acc = 0;
	_x_dec_rel = 0;
	_y_dec_rel = 0;
	_x_vel_max = 6.0f;
	_y_vel_max = 6.0f;

	_x_dir = Direction::NONE;
	_y_dir = Direction::NONE;

	// game parameters
	_life = 1.2f;
	_duke_fly = duke_fly;
	_accumulator = 0;

	if (duke_fly)
	{
		_sprite = _sprites["fly_black"];
		_distFromIsaac = 0;
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
	
	Vec2Df centerFly = _rect.center();

	if (_duke_fly)
	{
		Duke* duke = static_cast<GameScene*>(_scene)->duke();
		Vec2Df centerDuke = duke->rect().center();
		moveAroundDuke(centerDuke, centerFly, dt);
	}
	else
	{
		Isaac* isaac = static_cast<GameScene*>(_scene)->player();

		Vec2Df centerIsaac = isaac->rect().center();


		float dist = centerFly.distance(centerIsaac);
		if (dist < _distFromIsaac)
			followIsaac(centerIsaac);
	}
}


void Fly::moveAroundDuke(Vec2Df pos, Vec2Df fly, float dt)
{

	if (!_movable)
	{
		return;
	}

	_x_acc = 45.0f;
	_y_acc = 45.0f;

	static float currentAngle = 0.0f;
	const float rotationRadius = 80.0f; 
	const float minDistance = 1.7f; 
	const float angleIncrement = 0.3f * M_PI;
	Vec2Df _next;

	float _distanceFromDuke = pos.distance(fly);

	if (_distanceFromDuke > minDistance) {
		if (pos.x > fly.x) {
			_x_dir = Direction::RIGHT;
		}
		else {
			_x_dir = Direction::LEFT;
		}
		if (pos.y > fly.y) {
			_y_dir = Direction::DOWN;
		}
		else {
			_y_dir = Direction::UP;
		}
	}
	else {

		_x_acc = 35.0f;
		_y_acc = 35.0f;

		currentAngle += angleIncrement * dt;
		
		if (currentAngle >= 2 * M_PI) {
			currentAngle -= 2 * M_PI;
		}

		_next.x = pos.x + rotationRadius * std::cos(currentAngle);
		_next.y = pos.y + rotationRadius * std::sin(currentAngle);

		// Direzione verso il nuovo target
		if (_next.x > fly.x) {
			_x_dir = Direction::RIGHT;
		}
		else {
			_x_dir = Direction::LEFT;
		}
		if (_next.y > fly.y) {
			_y_dir = Direction::DOWN;
		}
		else {
			_y_dir = Direction::UP;
		}
	}
}


void Fly::followIsaac(Vec2Df pos)
{
	if (!_movable)
	{
		return;
	}

	_x_acc = 8;
	_y_acc = 8;
	_x_vel_max = 3.0f;
	_y_vel_max = 3.0f;

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

	int randMove = rand() % 10000;
	if (randMove < 200)
	{
		move();
	}
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
		});
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