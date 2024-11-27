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


using namespace agp;

Duke::Duke(Scene* scene, const PointF& pos, float spawnDelay)
	:Enemy(scene, RectF(pos.x, pos.y, 77 / 16 * 0.8f, 66 / 16 * 0.8f), nullptr, spawnDelay, 5)
{
	//_rect = RectF(pos.x, pos.y, 77/16, 66/12);
	//setCollider(pos.x, pos.y, 77/16, 66/12);

	
	_collider.adjust(0.6f, 0.3f, -0.6f, -0.6f);
	_visible = false;
	_collidable = true;
	_compenetrable = false;

	_sprites["duke_1"] = SpriteFactory::instance()->get("duke_1");
	_sprites["duke_2"] = SpriteFactory::instance()->get("duke_2");
	_sprites["duke_3"] = SpriteFactory::instance()->get("duke_3");
	_sprites["duke_4"] = SpriteFactory::instance()->get("duke_4");

	_sprites["blood"] = SpriteFactory::instance()->get("blood");
	_sprites["bloodExplotion"] = SpriteFactory::instance()->get("bloodExplotion");

	//phisic
	_x_vel_max = 1.2f;
	_y_vel_max = 1.2f;
	_x_dir = rand() %2 ? Direction::RIGHT : Direction::LEFT;
	_y_dir = rand() % 2 ? Direction::DOWN : Direction::UP;
	_x_acc = 10.0f;
	_y_acc = 10.0f;
	_x_dec_rel = 0;
	_y_dec_rel = 0;

	//game parameters
	_life = 3.0f;

	_accumulator = 0;

}


void Duke::wobble(float dt)
{
	float wMax = 77 / 16 * 0.8f;
	float hMax = 66 / 16 * 0.8f;

	if (!_wobbling)
		return;

	_wobbleAccumulator += dt;
	float conv = _wobbleAccumulator;


	float frequency = 14.0f;   // Oscillazioni al secondo
	float initialAmplitude = 16.0f; // Intensità iniziale del wobble
	float convDuration = 1.0f; // Durata dello smorzamento 

	
	float dampeningFactor = 1.0f - (conv / convDuration);
	if (dampeningFactor < 0.0f)
		dampeningFactor = 0.0f;

	float amplitude = initialAmplitude * dampeningFactor;

	float wobbleFactor = amplitude * sin(_wobbleAccumulator * frequency);

	// Applica la trasformazione solo se c'è un effetto visibile
	if (dampeningFactor > 0.0f)
		_rect.adjust(0, 0, wobbleFactor * dt, -wobbleFactor * dt);
	
	if (dampeningFactor <= 0.0f)
	{
		if (wobbleFactor * dt < wMax && wobbleFactor * dt < hMax)
		{
			_rect.size = { wMax, hMax };
		}
	}

	if (_wobbleAccumulator >= convDuration)
	{
		_wobbleAccumulator = 0; 
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
	_sprite = _sprites["bloodExplotion"];
	_dying = true;

	_vel = { 0,0 };

	new RenderableObject(_scene, _rect, _sprites["blood"]);

	if (!isSchedule("dyingDukeAnimation"))
		schedule("dyingDukeAnimation", 0.45f, [this]()
			{
				_scene->killObject(this);
				_scene->killObject(_shadow);

			}, 0, false);
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

	_shadow->setRect(_rect * Vec2Df(0.5f, 0.3f) + Vec2Df(0.85f, 2.9f));


	if (!_dying)
	{
		
		if (_accumulator <= index[0]) {
			//_wobbling = true;
			_sprite = _sprites["duke_1"];
		}

		else if (_accumulator <= (index[0] + index[1]))
		{
			_wobbling = true; 
			//wobble(dt);
			_sprite = _sprites["duke_2"];
		}
		else if (_accumulator <= (index[0] + index[1] + index[2]))
		{
			_wobbling = true;
			//wobble(dt);
			_sprite = _sprites["duke_3"];
		}
		else if (_accumulator <= (index[0] + 4 * index[1] + index[2]))
		{
			_wobbling = true;
			//wobble(dt);
			_sprite = _sprites["duke_2"];
			
		}
		else if (_accumulator <= (index[0] + index[1] + index[2] + index[3]))
		{
			_wobbling = true;
			//wobble(dt);
			_sprite = _sprites["duke_4"];
			
		}
		else
		{
			_accumulator = 0;
			_wobbling = false;
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

		else if (fromDir == Direction::LEFT || fromDir == Direction::RIGHT)
			_x_dir = inverse(_x_dir);
	}
	return true;
}