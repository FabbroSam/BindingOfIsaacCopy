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
	_sprites["blackglow"] = SpriteFactory::instance()->get("blackglow");

	//phisic
	_x_vel_max = 1.3f;
	_y_vel_max = 1.3f;
	_x_dir = rand() %2 ? Direction::RIGHT : Direction::LEFT;
	_y_dir = rand() % 2 ? Direction::DOWN : Direction::UP;
	_x_acc = 25.0f;
	_y_acc = 25.0f;
	_x_dec_rel = 0;
	_y_dec_rel = 0;

	//game parameters
	_life = 20.0f;

	_accumulator = 0;


	spawnFly();
	schedule("spawn_fly", 10.0f, [this]() {spawnFly();}, -1);
}

void Duke::spawnFly()
{
	_x_prev_dir = _x_dir;
	_y_prev_dir = _y_dir;
	_vel = { 0,0 };
	_x_dir = Direction::NONE;
	_y_dir = Direction::NONE;

	//messi fuori così spawna, si ferma e riparte dopo l'animazione dello spawn
	new Fly(_scene, PointF(_rect.pos.x, _rect.pos.y), 4.5f);
	new Fly(_scene, PointF(_rect.pos.x + _rect.size.x - 1, _rect.pos.y), 4.5f);
	new Fly(_scene, PointF(_rect.pos.x, _rect.pos.y + _rect.size.y - 1), 4.5f);
	new Fly(_scene, PointF(_rect.pos.x + _rect.size.x - 1, _rect.pos.y + _rect.size.y - 1), 4.5f);
	

	schedule("change_dir", 0.7f, [this]() 
		{ 	
			_x_dir = _x_prev_dir;
			_y_dir = _y_prev_dir;
		});
}

void Duke::wobble()
{
	if (_trigger)
	{
		if (_wobbling)
		{
			_rect.size.x += 0.05f * _bounceDirection;
			_rect.size.y += 0.2f * _bounceDirection;

			if (_rect.size.y >= _fixSize.y + 1.2f && _rect.size.x >= _fixSize.x + 0.1f)
			{
				_bounceDirection *= -1;
			}

			if (_rect.size.x <= _fixSize.x || _rect.size.y <= _fixSize.y)
			{
				_rect.size = _fixSize;
				_bounceDirection = 0;
			}
		}
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


void Duke::update(float dt)
{
	Enemy::update(dt);

	_accumulator += dt;
	float index[4];

	//inserimento secondi per ogni animazione nell'array
	//l'indice i rappresenta il tempo di animazione della sprite duke_i+1 in spriteFactory
	index[0] = 5.0f;
	index[1] = 0.2f;
	index[2] = 4.0f;
	index[3] = 3.0f;

	_shadow->setRect(_rect * Vec2Df(0.5f, 0.3f) + Vec2Df(0.85f, 2.7f));

	if (!_dying)
	{

		if (_accumulator <= index[0])
		{
			_sprite = _sprites["duke_1"];
		}

		else if (_accumulator <= (index[0] + index[1]))
		{
			trigger();
			wobble();
			_sprite = _sprites["duke_2"];
		}
		else if (_accumulator <= (index[0] + index[1] + index[2]))
		{
			trigger();
			wobble();
			_sprite = _sprites["duke_3"];
		}
		else if (_accumulator <= (index[0] + 4 * index[1] + index[2]))
		{
			trigger();
			_sprite = _sprites["duke_2"];
		}
		else if (_accumulator <= (index[0] + index[1] + index[2] + index[3]))
		{
			trigger();
			_sprite = _sprites["duke_4"];
		}
		else
		{
			_accumulator = 0;
		}
	}
}

bool Duke::collision(CollidableObject* with, Direction fromDir)
{
	Enemy::collision(with,fromDir);

	if (with->to<StaticObject*>())
	{
		if (fromDir == Direction::UP || fromDir == Direction::DOWN)
			_y_dir = inverse(_y_dir);

		/*if (fromDir == Direction::LEFT || fromDir == Direction::RIGHT)*/
			_x_dir = inverse(_x_dir);
	}
	return true;
}

bool Duke::collidableWith(CollidableObject* obj)
{
	if (obj->to<Fly*>())
		return false;
	return true;
}