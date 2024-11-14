// ----------------------------------------------------------------
// From "Algorithms and Game Programming" in C++ by Alessandro Bria
// Copyright (C) 2024 Alessandro Bria (a.bria@unicas.it). 
// All rights reserved.
// 
// Released under the BSD License
// See LICENSE in root directory for full details.
// ----------------------------------------------------------------

#include <iostream>
#include "MovableObject.h"

using namespace agp;

MovableObject::MovableObject(Scene* scene, const RectF& rect, Sprite* sprite, int layer, float angle, SDL_RendererFlip flip) :
	RenderableObject(scene, rect, sprite, layer, angle, flip)
{
	// default movement (stand)
	_x_dir = Direction::NONE;
	_y_dir = Direction::NONE;
	_vel = { 0, 0 };

	defaultPhysics();
}

void MovableObject::defaultPhysics()
{

	_y_vel_min = 0.3f;
	_y_vel_max = 10;	
	_y_acc = 10;	
	_y_dec_rel = 20;

	_x_vel_min = 0.3f;
	_x_vel_max = 10;		
	_x_acc = 10;
	_x_dec_rel = 20;
}

void MovableObject::velClip(float vx, float vy)
{
	_vel.x = std::max(_vel.x, -vx);
	_vel.x = std::min(_vel.x, vx);
	_vel.y = std::max(_vel.y, -vy);
	_vel.y = std::min(_vel.y, vy);
}

void MovableObject::velAdd(Vec2Df amount)
{
	_vel += amount;

	// max velocity clipping
	velClip(_x_vel_max, _y_vel_max);

	// min velocity clipping
	if (_x_dir == Direction::NONE && std::abs(_vel.x) < _x_vel_min)
		_vel.x = 0; // checking _x_dir to allow skidding
	if (_y_dir == Direction::NONE && std::abs(_vel.y) < _y_vel_min)
		_vel.y = 0;
}

void MovableObject::move_x(Direction dir)
{
	_x_dir = dir;
}

void MovableObject::move_y(Direction dir)
{
	_y_dir = dir;
}

bool MovableObject::skidding() const
{
	return (_x_dir == Direction::RIGHT && _vel.x < 0) ||
		   (_x_dir == Direction::LEFT  && _vel.x > 0);
}

void MovableObject::update(float dt)
{
	RenderableObject::update(dt);

	_prev_vel = _vel;

	// apply forces
	if (_x_dir != Direction::NONE)
		velAdd(dir2vec(_x_dir) * _x_acc);
	if (_y_dir != Direction::NONE)
		velAdd(dir2vec(_y_dir) * _y_acc);
	if (_x_dir == Direction::NONE)
		velAdd({ -_vel.versX() * _x_dec_rel * dt, 0 });
	if (_y_dir == Direction::NONE)
		velAdd({ 0, -_vel.versY() * _y_dec_rel * dt });

	_rect.pos += _vel * dt;
}