#include "Fly.h"
#include "SpriteFactory.h"
#include "GameScene.h"
#include "Mario.h"
#include <random>
#include <iostream>
#include "Room.h"


using namespace agp;

Fly::Fly(Scene* scene, const PointF& pos, float spawnDelay)
	:Enemy(scene, RectF(pos.x, pos.y, 30/16, 30/12), nullptr, spawnDelay, 5)
{
	_x_dir = Direction::NONE;
	_y_dir = Direction::NONE;

	_sprites["fly"] = SpriteFactory::instance()->get("fly");
	_sprite = _sprites["fly"];

	_collider.adjust(0.5f,0.7f,-0.2f,-0.9f);

	_visible = true;
	_collidable = false;

	_x_vel_max = 1.0f;
	_y_vel_max = 1.0f;

	schedule("flySpawn", _spawnDelay, [this]() 
		{
		set_schedule_param();
		}
	);

}


void Fly::set_schedule_param()
{
	_visible = true;
	_collidable = true;
	_x_dir = Direction::RIGHT;
	_y_dir = Direction::UP;
}


void::Fly::update(float dt)
{
	Enemy::update(dt);

	schedule("randomMov", 3.5f, [this]() {

		if (rand() % 5 == 0)
			_x_dir = Direction::RIGHT;

		else if (rand() % 5 == 0)
			_x_dir = Direction::DOWN;

		else if (rand() % 5 == 0)
			_x_dir = Direction::LEFT;

		else if (rand() % 5 == 0)
			_y_dir = Direction::UP;

		}, 0,false);
}


bool Fly::collision(CollidableObject* with, Direction fromDir)
{
	if (with->name().find("Static") == 0) {

		if (fromDir == Direction::RIGHT || fromDir == Direction::LEFT)
			_x_dir = inverse(fromDir);
		else
			_y_dir = inverse(fromDir);

	}

	return true;
}