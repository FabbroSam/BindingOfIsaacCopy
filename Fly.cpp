#include "Fly.h"
#include "SpriteFactory.h"
#include "GameScene.h"
#include "Mario.h"
#include <random>
#include <iostream>
#include "Room.h"
#include "Tear.h"

using namespace agp;

Fly::Fly(Scene* scene, const PointF& pos, float spawnDelay)
	:Enemy(scene, RectF(pos.x, pos.y, 30/16, 30/16), nullptr, spawnDelay, 5)
{
	heart = 0;

	_x_dir = Direction::NONE;
	_y_dir = Direction::NONE;

	_sprites["fly"] = SpriteFactory::instance()->get("fly");
	_sprites["dyingFly"] = SpriteFactory::instance()->get("dyingFly");
	_sprite = _sprites["fly"];

	_shadow = new RenderableObject(_scene, _rect, SpriteFactory::instance()->get("shadow"), 4);

	_collider.adjust(0.4f,0.58f,-0.3f,-0.58f);

	_visible = true;
	_collidable = true;

	_x_vel_max = 1.5f;
	_y_vel_max = 1.5f;

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

	_shadow->setRect(_rect * Vec2Df(0.35f, 0.15f) + Vec2Df(0.44f, 1.5f));

	schedule("randomMovement", 0, [this]() {

		if (rand() % 7 == 0)
			_x_dir = Direction::RIGHT;

		else if (rand() % 7 == 0)
			_x_dir = Direction::DOWN;

		else if (rand() % 7 == 0)
			_x_dir = Direction::LEFT;

		else if (rand() % 7 == 0)
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

bool Fly::collidableWith(CollidableObject* obj)
{
	return true;
}

void Fly::hurt()
{
	heart += 1;
	if (heart > 2)
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
}