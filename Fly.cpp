#include "Fly.h"
#include "SpriteFactory.h"
#include "StaticObject.h"
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
	_heart = 3;
	_ampl = 0.01f;
	_freq = 3*.0f;
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

	_collider.adjust(0.4f,0.28f,-0.3f,0.1f);

	//_visible = false;
	_collidable = true;
	_compenetrable = false;

	_vel.y = 1;
	_vel.x = 1;
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

	_collidable = true;
	//_x_dir = Direction::RIGHT;
	//_y_dir = Direction::UP;
}


void::Fly::update(float dt)
{
	_accumulator += dt;
	CollidableObject::update(dt);

	_shadow->setRect(_rect * Vec2Df(0.35f, 0.15f) + Vec2Df(0.4f, 0.9f));

	_rect.pos.x += 0.01f * cos(_accumulator)+_vel.x * dt;
	//_rect.pos.y += 20*dt*dt/2 +_vel.y * dt;
	_rect.pos.y += 0.01f * sin(dt);
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
	StaticObject* robj = dynamic_cast<StaticObject*>(obj);
	if (robj)
	{
		std::cout << "ok";
		_vel = -1*_vel;
	}
	return true;
}

void Fly::hurt()
{
	_heart -= 1;
	if (_heart == 0)
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