#include "Duke.h"
#include "SpriteFactory.h"
#include "GameScene.h"
#include "Mario.h"
#include <random>
#include <iostream>
#include "Room.h"


using namespace agp;

Duke::Duke(Scene* scene, const PointF& pos, float spawnDelay)
	:Enemy(scene, RectF(pos.x, pos.y, 77 / 16 * 0.8f, 66 / 16 * 0.8f), nullptr, spawnDelay, 5)
{
	//_rect = RectF(pos.x, pos.y, 77/16, 66/12);
	//setCollider(pos.x, pos.y, 77/16, 66/12);

	_heart = 3;

	_x_dir = Direction:: NONE;
	_y_dir = Direction::NONE;
	
	_collider.adjust(0.6, 0.3f, -0.6f, -0.6);

	_sprites["duke_1"] = SpriteFactory::instance()->get("duke_1");
	_sprites["duke_2"] = SpriteFactory::instance()->get("duke_2");
	_sprites["duke_3"] = SpriteFactory::instance()->get("duke_3");
	_sprites["duke_4"] = SpriteFactory::instance()->get("duke_4");

	_sprites["bloodExplotion"] = SpriteFactory::instance()->get("bloodExplotion");

	_visible = false;
	_collidable = false;
	_compenetrable = false;

	_x_vel_max = 1.2f;
	_y_vel_max = 1.2f;

	_accumulator = 0;


	schedule("dukeSpawn", _spawnDelay, [this]() {
		set_schedule_param();
		}
	);

}

void Duke::set_schedule_param() 
{
	_visible = true;
	_collidable = true;
	_compenetrable = true;
	_x_dir = Direction::RIGHT;
	_y_dir = Direction::UP;
	//_blood->setVisible(false);
}

bool Duke::collision(CollidableObject* with, Direction fromDir)
{
	if (with->name().find("Static")==0) {
		
		if(fromDir==Direction::RIGHT||fromDir==Direction::LEFT)
			_x_dir = inverse(fromDir);
		else
			_y_dir = inverse(fromDir);

	}
	
	return true;
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
		_wobbleAccumulator = convDuration; 
	}
}


bool Duke::collidableWith(CollidableObject* obj)
{
	return true;
}

void Duke::hurt()
{
	_heart -= 1;
	if (_heart == 0)
	{
		_sprite = _sprites["bloodExplotion"];
		_dying = true;
		_x_dir = Direction::NONE;
		_y_dir = Direction::NONE;
		_scene->killObject(_shadow);

		if (!isSchedule("dyingDukeAnimation")) 
			schedule("dyingDukeAnimation", 0.45f, [this]() 
			{
				
				_scene->killObject(this);

			}, 0, false);
	}
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
			_wobbling = true;
			_sprite = _sprites["duke_1"];
		}

		else if (_accumulator <= (index[0] + index[1]))
		{
			_wobbling = true; 
			wobble(dt);
			_sprite = _sprites["duke_2"];
		}
		else if (_accumulator <= (index[0] + index[1] + index[2]))
		{
			_wobbling = true;
			wobble(dt);
			_sprite = _sprites["duke_3"];
		}
		else if (_accumulator <= (index[0] + 4 * index[1] + index[2]))
		{
			_wobbling = true;
			wobble(dt);
			_sprite = _sprites["duke_2"];
			
		}
		else if (_accumulator <= (index[0] + index[1] + index[2] + index[3]))
		{
			_wobbling = true;
			wobble(dt);
			_sprite = _sprites["duke_4"];
			
		}
		else
		{
			_accumulator = 0;
			_wobbling = false;
		}
	}

}
