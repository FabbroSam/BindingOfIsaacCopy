#include "Gusher.h"
#include <cmath>
#include "SpriteFactory.h"
#include "GameScene.h"
#include <random>
#include <iostream>
#include "Room.h"
#include "Isaac.h"
#include "Host.h"
#include "Tear.h"
#include "StaticObject.h"
#include "Audio.h"


using namespace agp;

Gusher::Gusher(Scene* scene, const PointF& pos, float spawnDelay)
	:Enemy(scene, RectF(pos.x, pos.y, 1.4f, 1.4f), nullptr, spawnDelay, 10)
{

	_visible = true;
	_collidable = true;
	_compenetrable = false;

	_collider.adjust(0.45f, 0.45f, -0.4f, -0.4f);
	_sprites["gusher_front"] = SpriteFactory::instance()->get("gusher_front");
	_sprites["gusher_back"] = SpriteFactory::instance()->get("gusher_back");
	_sprites["gusher_right"] = SpriteFactory::instance()->get("gusher_right");
	_sprites["blood"] = SpriteFactory::instance()->get("blood");
	_sprites["blood_walking"] = SpriteFactory::instance()->get("blood_walking");

	_blood_walk = new RenderableObject(_scene, _rect + Vec2Df({ 0.3f, -0.4f }), _sprites["blood_walking"], 12);
	schedule("blood", 0.7f, [this]() {
		if(rand()%2 == 0)
			new RenderableObject(_scene, _rect * Vec2Df(0.2f, 0.2f) + Vec2Df(0.5f, 0.7f), _sprites["blood"], 1);
		new RenderableObject(_scene, _rect * Vec2Df(0.2f , 0.2f) + Vec2Df (0.8f, 0.2f), _sprites["blood"], 1);
		},-1);
	_x_vel_max = 0.7f;
	_y_vel_max = 0.7f;
	_x_dir = rand() % 10 > 5 ? Direction::LEFT : Direction::RIGHT;
	_y_dir = rand() % 10 > 5 ? Direction::UP : Direction::DOWN;
	_x_acc = 15.0f;
	_y_acc = 15.0f;
	_x_dec_rel = 0;
	_y_dec_rel = 0;

	//game parameters
	_life = 3.0f;
	_accumulator = 0;
	_blood = false;
	
}

void Gusher::update(float dt)
{
	Enemy::update(dt);

	_accumulator += dt;
	_shadow->setVisible(true);
	_shadow->setRect(_rect * Vec2Df(0.3f, 0.08f) + Vec2Df(0.42f, 1.0f));
	
	_blood_walk->setRect(_rect);
	
	if (_accumulator >= 1 && _blood)
	{
		//new RenderableObject(_scene, RectF(_rect.pos.x + 1.0f, _rect.pos.y + 1.0f, (rand() % 20 + 10) / 100.0f, (rand() % 20 + 10) / 100.0f), _sprites["blood"], 2);
		_blood = false;
		if (_accumulator >= 2.0f)
		{
			//shoot();
			move();
			_accumulator = 0;
			/*_blood = true;*/
		}
	}

	if (_x_dir == Direction::RIGHT &&  _y_dir == Direction::DOWN)
	{
		setFlip(SDL_FLIP_NONE);
		_sprite = _sprites["gusher_right"];
	}
	else if (_x_dir == Direction::LEFT && _y_dir == Direction::DOWN)
	{
		setFlip(SDL_FLIP_HORIZONTAL);
		_sprite = _sprites["gusher_right"];
	}
	else if (_y_dir == Direction::UP)
	{
		setFlip(SDL_FLIP_NONE);
		_sprite = _sprites["gusher_back"];
	}
}

bool Gusher::collision(CollidableObject* with, Direction fromDir)
{
	Enemy::collision(with, fromDir);

	if (with->to<StaticObject*>())
	{
		if (fromDir == Direction::LEFT || fromDir == Direction::RIGHT)
			_x_dir = inverse(fromDir);

		if (fromDir == Direction::UP || fromDir == Direction::DOWN)
			_y_dir = inverse(fromDir);
	}
	return true;
}

bool Gusher::collidableWith(CollidableObject* obj)
{
	return true;
}

void Gusher::move()
{
	_x_vel_max = 0.9f;
	_y_vel_max = 0.9f;
	_x_acc = 7.0f;
	_y_acc = 3.0f;

	_x_dir = rand() % 10 > 5 ? Direction::LEFT : Direction::RIGHT;
	_y_dir = rand() % 10 > 5 ? Direction::UP : Direction::DOWN;

}

void Gusher::shoot()
{
	PointF spawnPoint;
	spawnPoint.x = _rect.pos.x + 0.3f;
	spawnPoint.y = _rect.pos.y + 0.5f;
	Vec2Df dir = dir2vec(_x_dir) + dir2vec(_y_dir);             
	new Tear(_scene, spawnPoint, dir, 0.05f, 0.05f, true, true, 8);

}