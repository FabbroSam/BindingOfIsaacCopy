#include "Duke.h"
#include "SpriteFactory.h"
#include "GameScene.h"
#include "Mario.h"
#include <random>
#include <iostream>
#include "Room.h"


using namespace agp;

Duke::Duke(Scene* scene, const PointF& pos, float spawnDelay)
	:Enemy(scene, RectF(pos.x, pos.y, 77 / 16 * 0.85f, 66 / 16 * 0.85f), nullptr, spawnDelay, 5)
{
	//_rect = RectF(pos.x, pos.y, 77/16, 66/12);
	//setCollider(pos.x, pos.y, 77/16, 66/12);

	_x_dir = Direction:: NONE;
	_y_dir = Direction::NONE;

	_shadow = new RenderableObject(_scene,_rect, SpriteFactory::instance()->get("shadow"), 4);

	_collider.adjust(1, 0.3f, -0.8f, -1);

	_sprites["duke_1"] = SpriteFactory::instance()->get("duke_1");
	_sprites["duke_2"] = SpriteFactory::instance()->get("duke_2");
	_sprites["duke_3"] = SpriteFactory::instance()->get("duke_3");
	_sprites["duke_4"] = SpriteFactory::instance()->get("duke_4");

	_visible = false;
	_collidable = false;
	_compenetrable = false;

	_x_vel_max = 1.0f;
	_y_vel_max = 1.0f;

	float accumulator = 0;


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

void Duke::update(float dt)
{
	Enemy::update(dt);

	accumulator += dt;
	float index[4];

	//inserimento secondi per ogni animazione nell'array
	//l'indice i rappresenta il tempo di animazione della sprite duke_i+1 in spriteFactory
	index[0] = 5.0f;
	index[1] = 0.2f;
	index[2] = 4.0f;
	index[3] = 3.0f;

	_shadow->setRect(_rect * Vec2Df(0.5f, 0.3f) + Vec2Df(0.85f, 2.9f));
	

	if (accumulator <= index[0])
		_sprite = _sprites["duke_1"];
	else if (accumulator <= (index[0] + index[1]))
		_sprite = _sprites["duke_2"];
	else if (accumulator <= (index[0] + index[1] + index[2]))
		_sprite = _sprites["duke_3"];
	else if (accumulator <= (index[0] + 4 * index[1] + index[2]))
		_sprite = _sprites["duke_2"];
	else if (accumulator <= (index[0] + index[1] + index[2] + index[3]))
		_sprite = _sprites["duke_4"];
	else
		accumulator = 0;
}
