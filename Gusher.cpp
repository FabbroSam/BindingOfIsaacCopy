#include "Gusher.h"
#include <cmath>
#include "SpriteFactory.h"
#include "GameScene.h"
#include <random>
#include <iostream>
#include "Room.h"
#include "Isaac.h"
#include "Host.h"
#include "Bomb.h"
#include "Tear.h"
#include "StaticObject.h"
#include "Audio.h"


using namespace agp;

Gusher::Gusher(Scene* scene, const PointF& pos, float spawnDelay)
	:Enemy(scene, RectF(pos.x, pos.y, 1.4f, 1.4f), nullptr, spawnDelay, 10)
{

	_visible = false;
	_collidable = true;
	_compenetrable = false;

	_collider.adjust(0.45f, 0.45f, -0.4f, -0.4f);
	_sprites["gusher_front"] = SpriteFactory::instance()->get("gusher_front");
	_sprites["gusher_back"] = SpriteFactory::instance()->get("gusher_back");
	_sprites["gusher_right"] = SpriteFactory::instance()->get("gusher_right");
	_sprites["blood"] = SpriteFactory::instance()->get("blood");
	_sprites["blood_walking"] = SpriteFactory::instance()->get("blood_walking");
	_sprites["bloodExplotion"] = SpriteFactory::instance()->get("bloodExplotion");

	_shadow->setVisible(false);

	_blood_walk = new RenderableObject(_scene, _rect + Vec2Df({ 0.3f, -0.4f }), _sprites["blood_walking"], 12);
	_blood_walk->setVisible(false);

	schedule("blood_walk_on", 1.5f, [this]() {
		_blood_walk->setVisible(true);
		});

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

	if(!_dying)
	{
		_accumulator += dt;
		_shadow->setRect(_rect * Vec2Df(0.3f, 0.08f) + Vec2Df(0.42f, 1.0f));

		_blood_walk->setRect(_rect);

		if (_accumulator >= 1.5f)
		{
			shoot();
			move();
			_accumulator = 0;
		}

		if (_x_dir == Direction::RIGHT && _y_dir == Direction::DOWN)
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
	spawnPoint.y = _rect.pos.y;
	Vec2Df dir = dir2vec(_x_dir) + dir2vec(_y_dir);             
	new Tear(_scene, spawnPoint, dir, 5, 5, true, true, 11);
}

void Gusher::hit(float damage, Vec2Df _dir)
{
	Enemy::hit(damage);
}

void Gusher::die()
{
	if (rand() % 5 == 0)
		new Bomb(_scene, PointF(_rect.pos.x, _rect.pos.y), 8);
	_blood_walk->setVisible(false);
	_shadow->setVisible(false);
	_dying = true;
	_collidable = false;
	_movable = false;
	_sprite = _sprites["bloodExplotion"];

	if (!isSchedule("dyingGusherAnimation"))
		schedule("dyingGusherAnimation", 0.37f, [this]()
			{
				setVisible(false);
				new RenderableObject(_scene, _rect, _sprites["blood"], 1);
				_scene->killObject(this);

			}, 0, false);
}