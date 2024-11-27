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
#include "Coin.h"
#include <iostream>

using namespace agp;

Isaac::Isaac(Scene* scene, const PointF& pos)
	: DynamicObject(scene, RectF(pos.x + 1 / 16.0f, pos.y, 1.2f, 1.4f), nullptr, 9)
{

	_sprites["headFront"] = SpriteFactory::instance()->get("isaac_headFront");
	_sprites["headFrontShoot"] = SpriteFactory::instance()->get("isaac_headFrontShoot");
	_sprites["headBack"] = SpriteFactory::instance()->get("isaac_headBack");
	_sprites["headBackShoot"] = SpriteFactory::instance()->get("isaac_headBackShoot");
	_sprites["headRight"] = SpriteFactory::instance()->get("isaac_headRight");
	_sprites["headRightShoot"] = SpriteFactory::instance()->get("isaac_headRightShoot");
	_sprites["bodyFront"] = SpriteFactory::instance()->get("isaac_bodyFront");
	_sprites["walkDown"] = SpriteFactory::instance()->get("isaac_walkDown");
	_sprites["walkRight"] = SpriteFactory::instance()->get("isaac_walkRight");
	_sprites["skid"] = SpriteFactory::instance()->get("isaac_skid");
	_sprites["jump"] = SpriteFactory::instance()->get("isaac_jump");
	_sprites["die"] = SpriteFactory::instance()->get("isaac_die");
	_sprites["shadow"] = SpriteFactory::instance()->get("shadow");
	_sprite = _sprites["headFront"];


	_body = new RenderableObject(_scene, RectF(0, 0, 0, 0), _sprites["bodyFront"], 8);
	_shadow = new RenderableObject(_scene, RectF(0, 0, 0, 0), _sprites["shadow"], 7);
	_collider.adjust(0.3f, 1.1f, -0.3f, 0.2f);
	_walking = false;
	_running = false;
	_dying = false;
	_dead = false;
	_invincible = true;
	_compenetrable = false;


	_x_acc = 50.0f;
	_x_dec_rel = 5.0f;
	_x_vel_max = 5.5f;
	_x_vel_min = 0.2f;

	_y_acc = 50.0f;
	_y_dec_rel = 5.0f; 
	_y_vel_max = 5.5f;
	_y_vel_min = 0.2f;

	_x_vel_last_nonzero = 0;
	_y_vel_last_nonzero = 0;


}

void Isaac::update(float dt) {
	// physics and overrides
	DynamicObject::update(dt);

	_body->setRect(_rect + Vec2Df({ 0, 0.45f }));
	_shadow->setRect(RectF(_rect.pos.x+0.3f, _rect.pos.y+1.4f, 0.6f, 0.22f));

	// state logic
	if (_vel.x != 0)
		_x_vel_last_nonzero = _vel.x;
	if (_vel.y != 0)
		_y_vel_last_nonzero = _vel.y;
	_walking = (_vel.x != 0 || _vel.y != 0);

	if (_x_dir != _prev_x_dir)
		if (_x_dir != Direction::NONE)
			_vel.x = 0;

	if (_y_dir != _prev_y_dir)
		if (_y_dir != Direction::NONE)
			_vel.y = 0;


	// animations
	if (_dying)
		_sprite = _sprites["die"];
	else if (skidding())
		_sprite = _sprites["skid"];
	else if (_isShooting) {
		_shootTimer -= dt;
		if (_shootTimer <= 0.0f) {
			//_sprite = _sprites["headFront"];
			_isShooting = false;
		}
	}

	setSprite();

	_prev_x_dir = _x_dir;
	_prev_y_dir = _y_dir;

	// game logics

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
	schedule("hurt_isaac", 0.2f, [this]()
		{
			HUD* hud = Game::instance()->hud();
			hud->subHalfHearts();
			if (!hud->halfHearts())
				die();
		}, 0, false);
}

void Isaac::shoot(Direction dir) {
	PointF spawnPoint;
	spawnPoint.x = _rect.pos.x;
	spawnPoint.y = _rect.pos.y + 0.3f;
	if (!_canShoot) return;

	schedule("_canShoot", _shootCooldown, [this]() 
		{
		_canShoot = true;
		},0, true);

	_shootTimer = _shootAnimationTime;
	_isShooting = true;
	_canShoot = false;

	switch (dir) {
	case Direction::LEFT:
		spawnPoint.y += (_isShootingRight ? -0.5f : -0.2f);
		spawnPoint.x += (_isShootingRight ? -0.5f : 0.0f);
		_sprite = _sprites["headRightShoot"];
		break;
	case Direction::RIGHT:
		spawnPoint.y += (_isShootingRight ? -0.2f : -0.5f);
		spawnPoint.x += (_isShootingRight ? 0.0f : 0.5f);
		_sprite = _sprites["headRightShoot"];
		break;
	case Direction::UP:
		spawnPoint.x += (_isShootingRight ? 0.2f : -0.05f);
		spawnPoint.y -= 1.0f;
		_sprite = _sprites["headBackShoot"];
		break;
	case Direction::DOWN:
		spawnPoint.x += (_isShootingRight ? -0.05f : 0.2f);
		_sprite = _sprites["headFrontShoot"];
		break;
	default:
		break;
	}

	Tear* newTear = new Tear(_scene, spawnPoint, dir, _vel.x, _vel.y);
	if (_isShootingRight)
		_isShootingRight = false;
	else
		_isShootingRight = true;
}

void Isaac::setSprite()
{
	if (_walking)
	{
		if (_vel.y > 0) {
			_sprite = _sprites["headFront"];
			_body->setSprite(_sprites["walkDown"]);
		}
		else if (_vel.y < 0) {
			_sprite = _sprites["headBack"];
			_body->setSprite(_sprites["walkDown"]);
		}
		else if (_vel.x != 0) {
			_sprite = _sprites["headRight"];
			_body->setSprite(_sprites["walkRight"]);
		}
	}
	else
	{
		_sprite = _sprites["headFront"];
		_body->setSprite(_sprites["bodyFront"]);
	}
	// x-mirroring (move)
	if ((_vel.x < 0) || _x_vel_last_nonzero < 0)
	{
		_flip = SDL_FLIP_HORIZONTAL;
		_body->setFlip(SDL_FLIP_HORIZONTAL);
	}
	else
	{
		_flip = SDL_FLIP_NONE;
		_body->setFlip(SDL_FLIP_NONE);
	}


	if (_state[SDL_SCANCODE_UP])
		_sprite = _sprites["headBack"];
	else if (_state[SDL_SCANCODE_RIGHT])
		_sprite = _sprites["headRight"];
	else if (_state[SDL_SCANCODE_LEFT])
		_sprite = _sprites["headRight"];
	else
		_sprite = _sprites["headFront"];

	// x-mirroring
	if (_state[SDL_SCANCODE_LEFT])
		_flip = SDL_FLIP_HORIZONTAL;
	else
		_flip = SDL_FLIP_NONE;
}