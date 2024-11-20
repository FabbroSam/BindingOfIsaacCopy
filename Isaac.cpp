// ----------------------------------------------------------------
// From "Algorithms and Game Programming" in C++ by Alessandro Bria
// Copyright (C) 2024 Alessandro Bria (a.bria@unicas.it). 
// All rights reserved.
// 
// Released under the BSD License
// See LICENSE in root directory for full details.
// ----------------------------------------------------------------

#include "Isaac.h"
#include "SpriteFactory.h"
#include "Audio.h"
#include "AnimatedSprite.h"
#include "Game.h"
#include "Tear.h"
#include "Scene.h"
#include <iostream>

using namespace agp;

Isaac::Isaac(Scene* scene, const PointF& pos)
	: DynamicObject(scene, RectF( pos.x + 1 / 16.0f, pos.y, 1, 1 ), nullptr, 6)
{
	_collider.adjust(0.2f, 0.45f, -0.2f, 0);

	_walking = false;
	_jumping = false;
	_running = false;
	_dying = false;
	_dead = false;
	_invincible = true;

	_x_vel_last_nonzero = 0;
	_y_vel_last_nonzero = 0;

	_sprites["headFront"] = SpriteFactory::instance()->get("isaac_headFront");
	_sprites["headFrontShoot"] = SpriteFactory::instance()->get("isaac_headFrontShoot");
	_sprites["headBack"] = SpriteFactory::instance()->get("isaac_headBack");
	_sprites["headRight"] = SpriteFactory::instance()->get("isaac_headRight");
	_sprites["bodyFront"] = SpriteFactory::instance()->get("isaac_bodyFront");
	_sprites["walkDown"] = SpriteFactory::instance()->get("isaac_walkDown");
	_sprites["walkRight"] = SpriteFactory::instance()->get("isaac_walkRight");
	_sprites["skid"] = SpriteFactory::instance()->get("isaac_skid");
	_sprites["jump"] = SpriteFactory::instance()->get("isaac_jump");
	_sprites["die"] = SpriteFactory::instance()->get("isaac_die");
	_sprite = _sprites["headFront"];
	_bodySprite = _sprites["bodyFront"];
}

void Isaac::update(float dt) {
	if (!_canShoot) {
		_shootCooldown -= dt;
		if (_shootCooldown <= 0.0f) {
			_canShoot = true;
			_shootCooldown = 0.5f;
		}
	}

	// physics and overrides
	DynamicObject::update(dt);
	if (_bodySprite) {
		_bodySprite->update(dt);
	}

	// state logic
	if (_vel.x != 0)
		_x_vel_last_nonzero = _vel.x;
	if (_vel.y != 0)
		_y_vel_last_nonzero = _vel.y;
	_walking = (_vel.x != 0 || _vel.y != 0);

	if (_x_dir != _prev_x_dir) {
		if (_x_dir != Direction::NONE) {
			_vel.x = 0;
		}
	}

	if (_y_dir != _prev_y_dir) {
		if (_y_dir != Direction::NONE) {
			_vel.y = 0;
		}
	}

	// animations
	if (_dying)
		_sprite = _sprites["die"];
	else if (_jumping)
		_sprite = _sprites["jump"];
	else if (skidding())
		_sprite = _sprites["skid"];
	else if (_isShooting) {
		_shootTimer -= dt;
		if (_shootTimer <= 0.0f) {
			_sprite = _sprites["headFront"];
			_isShooting = false;
		}
	}
	else if (_walking) {
		if (_vel.y > 0) {
			_sprite = _sprites["headFront"];
			_bodySprite = _sprites["walkDown"];
		}
		else if (_vel.y < 0) {
			_sprite = _sprites["headBack"];
			_bodySprite = _sprites["walkDown"];
		}
		else if (_vel.x > 0) {
			_sprite = _sprites["headRight"];
			_bodySprite = _sprites["walkRight"];
		}
		else if (_vel.x < 0) {
			_sprite = _sprites["headRight"];
			_bodySprite = _sprites["walkRight"];
		}
	}
	else {
		_sprite = _sprites["headFront"];
		_bodySprite = _sprites["bodyFront"];
	}

	// x-mirroring
	if ((_vel.x < 0 && !_jumping) || _x_vel_last_nonzero < 0)
		_flip = SDL_FLIP_HORIZONTAL;
	else
		_flip = SDL_FLIP_NONE;

	_prev_x_dir = _x_dir;
	_prev_y_dir = _y_dir;
}
void Isaac::move_x(Direction dir)
{
	if (_dying || _dead)
		return;

	MovableObject::move_x(dir);
}
void Isaac::move_y(Direction dir)
{
	if (_dying || _dead)
		return;

	MovableObject::move_y(dir);
}

void Isaac::run(bool on)
{
	if (on)
	{
		_x_vel_max = 10;
		_x_acc = 13;
	}
	else
	{
		_x_vel_max = 6;	
		_x_acc = 8;
	}
}

void Isaac::die()
{
	if (_dying)
		return;

	_dying = true;
	_collidable = false;
	_vel = { 0,0 };
	_x_dir = Direction::NONE;
	_y_dir = _x_dir = Direction::NONE;
	Audio::instance()->haltMusic();
	Audio::instance()->playSound("death");
	Game::instance()->freeze(true);

	schedule("dying", 0.5f, [this]()
		{
			schedule("die", 3, [this]()
				{
					_dead = true;
					Game::instance()->gameover();
				});
		});
}

void Isaac::hurt()
{
	// TODO: powerdown (e.g. if Isaac is big, becomes small)
	//die();
}

void Isaac::draw(SDL_Renderer* renderer, Transform camera)
{
	if (!_visible)
		return;

	SDL_FRect drawRect = RectF(camera(_rect.tl()), camera(_rect.br())).toSDLf();

	_bodyRect = _rect;
	_bodyRect.size.x *= 0.8f;
	_bodyRect.size.y *= 0.8f;
	_bodyRect.pos.y += 0.85f;
	_bodyRect.pos.x = _rect.pos.x + (_rect.size.x - _bodyRect.size.x) / 2;

	SDL_FRect drawBodyRect = RectF(camera(_bodyRect.tl()), camera(_bodyRect.br())).toSDLf();

	if (_bodySprite)
		_bodySprite->render(renderer, _bodyRect, camera, _angle, _flip);
	else
	{
		SDL_SetRenderDrawColor(renderer, _color.r, _color.g, _color.b, _color.a);
		SDL_RenderFillRectF(renderer, &drawBodyRect);
	}

	if (_sprite)
		_sprite->render(renderer, _rect, camera, _angle, _flip);
	else
	{
		SDL_SetRenderDrawColor(renderer, _color.r, _color.g, _color.b, _color.a);
		SDL_RenderFillRectF(renderer, &drawRect);
	}

	if (_borderColor.a)
	{
		SDL_SetRenderDrawColor(renderer, _borderColor.r, _borderColor.g, _borderColor.b, _borderColor.a);
		SDL_RenderDrawRectF(renderer, &drawRect);
	}

	if (_focused)
	{
		SDL_SetRenderDrawColor(renderer, _focusColor.r, _focusColor.g, _focusColor.b, _focusColor.a);
		SDL_RenderFillRectF(renderer, &drawRect);
		_focused = false;
	}
}

void Isaac::shoot(Direction dir) {
	if (!_canShoot) return;

	_shootTimer = _shootAnimationTime;
	_isShooting = true;
	_canShoot = false;

	switch (dir) {
	case Direction::LEFT:
		break;
	case Direction::RIGHT:
		break;
	case Direction::UP:
		break;
	case Direction::DOWN:
		_sprite = _sprites["headFrontShoot"];
		break;
	default:
		break;
	}

	PointF spawnPos = _rect.pos;
	Tear* newTear = new Tear(_scene, spawnPos, dir);
	_scene->newObject(newTear);
}