#include <cmath>
#include "Duke.h"
#include "SpriteFactory.h"
#include "GameScene.h"
#include "Mario.h"
#include <random>
#include <iostream>
#include "Room.h"
#include "Isaac.h"
#include "Tear.h"
#include "StaticObject.h"
#include "Fly.h"


using namespace agp;

Duke::Duke(Scene* scene, const PointF& pos, float spawnDelay)
	:Enemy(scene, RectF(pos.x, pos.y, 77 / 16 * 0.8f, 66 / 16 * 0.8f), nullptr, spawnDelay, 5)
{

	_collider.adjust(0.48f, 0.3f, -0.5f, 0.5f);
	_visible = false;
	_collidable = true;
	_compenetrable = false;

	_sprites["duke_1"] = SpriteFactory::instance()->get("duke_1");
	_sprites["duke_2"] = SpriteFactory::instance()->get("duke_2");
	_sprites["duke_3"] = SpriteFactory::instance()->get("duke_3");
	_sprites["duke_4"] = SpriteFactory::instance()->get("duke_4");

	_sprites["blood"] = SpriteFactory::instance()->get("blood");
	_sprites["bloodExplotion"] = SpriteFactory::instance()->get("bloodExplotion");

	_sprite = _sprites["duke_1"];

	_x_vel_max = 1.4f;
	_y_vel_max = 1.4f;
	_x_dir = rand() % 2 ? Direction::RIGHT : Direction::LEFT;
	_y_dir = rand() % 2 ? Direction::DOWN : Direction::UP;
	_x_acc = 25.0f;
	_y_acc = 25.0f;
	_x_dec_rel = 0;
	_y_dec_rel = 0;

	//game parameters
	_life = 14.0f;
	_n_fly = 0;
	_max_flies = 4;
	_accumulator = 0;
	_bounceCycle = 1;
	_acc = 0;
	_wobbling = false;
	_fix_pos_x = _rect.pos.x;
	_fix_pos_y = _rect.pos.y;
	_wobbleTakeVar = true;
}

void Duke::spawnFly()
{
	int _n_fly = 0;
	auto& objects = _scene->objects();
	for (auto& obj : objects)
		if (obj->to<Fly*>())
			_n_fly++;

	int _toSpawn = _max_flies - _n_fly;

	std::cout << "n fly " << _n_fly << std::endl << "to spawn " << _toSpawn << std::endl;


	for (int i = 0; i < _toSpawn; i++)
	{
		new Fly(_scene,{ _rect.pos.x + 0.3f * i, _rect.pos.y +0.3f * i }, 0, true);
	}
}

void Duke::hit(float damage, Vec2Df _dir)
{
	Enemy::hit(damage);

	if (_dir.x < 0)
		_x_dir = Direction::LEFT;
	else
		_x_dir = Direction::RIGHT;

	if (_dir.y < 0)
		_y_dir = Direction::UP;
	else
		_y_dir = Direction::DOWN;
}


void Duke::die()
{
	_dying = true;

	_vel = { 0,0 };

	_shadow->setVisible(false);
	_sprite = _sprites["bloodExplotion"];

	if (!isSchedule("dyingDukeAnimation"))
		schedule("dyingDukeAnimation", 0.37f, [this]()
			{
				setVisible(false);
				new RenderableObject(_scene, _rect, _sprites["blood"], 6);
				_scene->killObject(this);

			}, 0, false);
}

void Duke::trigger()
{
	_wobbling = true;
	_trigger = true;
}

void Duke::wobble(float dt)
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

		if (_acc <= 0.5f)
		{
			_rect.size.x = 2.0f * _acc + _fixSize.x;
			_rect.pos.x = -1.0f * _acc + _fix_pos_x;
			_rect.size.y = 2.0f * _acc + _fixSize.y;
			_rect.pos.y = -1.0f * _acc + _fix_pos_y;
		}
		else if (_acc <= 1.01f)
		{
			_rect.size.x = 2.0f* std::sin(30.0f * (_acc - 0.5)) * std::exp(-(10.0f * (_acc - 0.5))) + _fixSize.x;
			_rect.pos.x = -1.0f* std::sin(30.0f * (_acc - 0.5)) * std::exp(-(10.0f * (_acc - 0.5))) + _fix_pos_x;
			_rect.size.y = -2.0f* std::sin(30.0f * (_acc - 0.5)) * std::exp(-(10.0f * (_acc - 0.5))) + _fixSize.y;
			_rect.pos.y = 1.0f* std::sin(30.0f * (_acc - 0.5)) * std::exp(-(10.0f * (_acc - 0.5))) + _fix_pos_y;
			
		}
		else
		{
			std::cout << "_prev_vel: " << _prev_vel << std::endl;
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


void Duke::update(float dt)
{
	Enemy::update(dt);

	_accumulator += dt;

	// Inserimento secondi per ogni animazione nell'array
	index[0] = 5.0f;
	index[1] = 0.6f;
	index[2] = 4.0f;
	index[3] = 1.0f;
	index[4] = 2.0f;

	_shadow->setRect(_rect * Vec2Df(0.5f, 0.3f) + Vec2Df(0.85f, 2.7f));

	if (!_dying)
	{
		if (_accumulator <= index[0])
		{
			if (_sprite != _sprites["duke_1"])
			{
				std::cout << "duke1\n";

				_sprite = _sprites["duke_1"];
			}
		}
		else if (_accumulator <= (index[0] + index[1]))
		{
			if (_sprite != _sprites["duke_2"])
			{
				std::cout << "duke2\n";
				_sprite = _sprites["duke_2"];
			}
		}
		else if (_accumulator <= (index[0] + index[1] + index[2]))
		{
			if (_sprite != _sprites["duke_3"])
			{
				std::cout << "duke3\n";
				_sprite = _sprites["duke_3"];
			}
		}
		else if (_accumulator <= (index[0] + index[1] + index[2] + index[3]))
		{
			if (_sprite != _sprites["duke_2"] && !isSchedule("delay_sprite"))
			{
				trigger();
				//schedule("spawn_fly", 0.f, [this]()
				//	{
				//		spawnFly();
				//	}, 0, false);
				spawnFly();
				std::cout << "duke2.2\n";
				schedule("delay_sprite", 0.01f, [this]()
					{
						_sprite = _sprites["duke_2"];
					}, 0, false);

			}
		}
		else if (_accumulator <= (index[0] + 4 * index[1] + index[2] + index[3] + index[4]))
		{
			if (_sprite != _sprites["duke_4"])
			{
				std::cout << "duke4\n";
				_sprite = _sprites["duke_4"];
			}
		}
		else
		{
			_accumulator = 0;
		}

		if(_wobbling)
			wobble(dt);
	}
}


bool Duke::collision(CollidableObject* with, Direction fromDir)
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

bool Duke::collidableWith(CollidableObject* obj)
{
	if (obj->to<Fly*>())
		return false;
	return true;
}