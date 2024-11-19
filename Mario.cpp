// ----------------------------------------------------------------
// From "Algorithms and Game Programming" in C++ by Alessandro Bria
// Copyright (C) 2024 Alessandro Bria (a.bria@unicas.it). 
// All rights reserved.
// 
// Released under the BSD License
// See LICENSE in root directory for full details.
// ----------------------------------------------------------------

#include "Mario.h"
#include "SpriteFactory.h"
#include "Audio.h"
#include "AnimatedSprite.h"
#include "Game.h"
#include "GameScene.h"
#include "Door.h"

using namespace agp;

Mario::Mario(Scene* scene, const PointF& pos)
	: DynamicObject(scene, RectF( pos.x + 1 / 16.0f, pos.y, 1, 1 ), nullptr, 2)
{
	_collider.adjust(0.2f, 0.45f, -0.2f, 0);

	_y_vel_min = 0.3f;
	_y_vel_max = 8;
	_y_acc = 10;
	_y_dec_rel = 20;

	_x_vel_min = 0.3f;
	_x_vel_max = 8;
	_x_acc = 10;
	_x_dec_rel = 20;

	_walking = false;
	_jumping = false;
	_running = false;
	_dying = false;
	_dead = false;
	_invincible = true;

	_x_vel_last_nonzero = 0;
	_y_vel_last_nonzero = 0;

	_sprites["stand"] = SpriteFactory::instance()->get("mario_stand");
	_sprites["walk"] = SpriteFactory::instance()->get("mario_walk");
	_sprites["run"] = SpriteFactory::instance()->get("mario_run");
	_sprites["skid"] = SpriteFactory::instance()->get("mario_skid");
	_sprites["jump"] = SpriteFactory::instance()->get("mario_jump");
	_sprites["die"] = SpriteFactory::instance()->get("mario_die");
	_sprite = _sprites["stand"];
}

void Mario::update(float dt)
{

	// physics
	DynamicObject::update(dt);

	// state logic
	if (_vel.x != 0)
		_x_vel_last_nonzero = _vel.x;
	if (_vel.y != 0)
		_y_vel_last_nonzero = _vel.y;
	_walking = (_vel.x != 0 || _vel.y != 0);
	_running = (std::abs(_vel.x) > 6 || std::abs(_vel.y) > 6);

	// animations
	if(_dying)
		_sprite = _sprites["die"];
	else if (_jumping)
		_sprite = _sprites["jump"];
	else if (skidding())
		_sprite = _sprites["skid"];
	else if (_running)
		_sprite = _sprites["run"];
	else if(_walking)
		_sprite = _sprites["walk"];
	else
		_sprite = _sprites["stand"];

	// x-mirroring
	if ((_vel.x < 0 && !_jumping) || _x_vel_last_nonzero < 0)
		_flip = SDL_FLIP_HORIZONTAL;
	else
		_flip = SDL_FLIP_NONE;
}

void Mario::move_x(Direction dir)
{
	if (_dying || _dead)
		return;

	MovableObject::move_x(dir);
}
void Mario::move_y(Direction dir)
{
	if (_dying || _dead)
		return;

	MovableObject::move_y(dir);
}

void Mario::run(bool on)
{

}

void Mario::die()
{

}

void Mario::hurt()
{
	// TODO: powerdown (e.g. if Mario is big, becomes small)
	//die();
}

bool Mario::collision(CollidableObject* with, Direction fromDir)
{
	//Door* door = dynamic_cast<Door*>(with);
	//if (door)
	//	door->Trigger();

	return true;
}