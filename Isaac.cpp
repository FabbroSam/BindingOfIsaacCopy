﻿// ----------------------------------------------------------------
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
#include "GameScene.h"
#include "Tear.h"
#include "Scene.h"
#include "Coin.h"
#include "Bone.h"
#include <iostream>
#include <cstdlib>

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
	_sprites["shadow"] = SpriteFactory::instance()->get("shadow");
	_sprites["hurt"] = SpriteFactory::instance()->get("isaac_hurt");
	_sprites["isaac_carry_bomb"] = SpriteFactory::instance()->get("isaac_carry_bomb");
	_sprites["new_item"] = SpriteFactory::instance()->get("isaac_new_item");
	_sprite = _sprites["headFront"];

	_body = new RenderableObject(_scene, RectF(0, 0, 0, 0), _sprites["bodyFront"], 8);
	_shadow = new RenderableObject(_scene, RectF(0, 0, 0, 0), _sprites["shadow"], 7);
	_collider.adjust(0.3f, 1.1f, -0.3f, 0.2f);
	_walking = false;
	_running = false;
	_dying = false;
	_blinking = false;
	_hurt = false;
	_dead = false;
	_invincible = false;
	_compenetrable = false;

	_blinkTimeElapsed = 0.0f;
	_blinkCount = 20;

	_x_acc = 50.0f;
	_x_dec_rel = 0;
	_x_vel_max = 5.5f;
	_x_vel_min = 0.2f;

	_y_acc = 50.0f;
	_y_dec_rel = 0; 
	_y_vel_max = 5.5f;
	_y_vel_min = 0.2f;

	_x_vel_last_nonzero = 0;
	_y_vel_last_nonzero = 0;

	_compenetrable = false;

	_shootAnimationTime = 0.5f;
	_shootTimer = 0.5f;
	_shootCooldown = 0.4f;
	_isShooting = false;
	_isShootingRight = true;
	_shootingDirection = Direction::NONE;
	_canShoot = true;
	_isShootingAnimation = false;
	_bombCarry = false;
	_state = SDL_GetKeyboardState(0);

	_shootAnimationTimer = 0.0f;
	_shootAnimationInterval = 0.4f;
	_swapShootAnimation = false;

	_itemBone = false;
	_itemBoneSprite = false;
}

void Isaac::update(float dt) 
{
	// physics and overrides
	DynamicObject::update(dt);

	_body->setRect(_rect + Vec2Df({ 0, 0.45f }));
	_shadow->setRect(RectF(_rect.pos.x+0.3f, _rect.pos.y+1.4f, 0.6f, 0.22f));

	if (_blinking) {
		_blinkTimeElapsed += dt;
		if (_blinkTimeElapsed >= 0.02f) {
			_blinkTimeElapsed = 0.0f;
			_body->setVisible(!_visible);
			_visible = !_visible;
			_blinkCount--;

			if (_blinkCount <= 0) {
				_blinking = false;
				_body->setVisible(true);
				_visible = true;
				_blinkCount = 20;
			}
		}
	}
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
		;
	else if (skidding())
		;
	else if (_isShooting) {
		_shootAnimationTimer += dt;

		if (_shootAnimationTimer < _shootAnimationInterval / 3.0f) {
			_swapShootAnimation = true;
		}
		else {
			_swapShootAnimation = false;
		}

		if (_shootAnimationTimer >= _shootAnimationInterval) {
			_shootAnimationTimer = 0.0f;
		}

		if (!_state[SDL_SCANCODE_RIGHT] && !_state[SDL_SCANCODE_LEFT] &&
			!_state[SDL_SCANCODE_UP] && !_state[SDL_SCANCODE_DOWN])
		{
			_shootTimer -= dt;
			_shootAnimationTimer = 0.0f;
			if (_shootTimer <= 0.0f) {
				_isShooting = false;
			}
		}
	}

	if(!_bombCarry)
	{
		setSprite();
	}

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

void Isaac::die()
{
	if (_dying)
		return;

	_dying = true;
	_collidable = false;
	_vel = { 0,0 };
	_x_dir = Direction::NONE;
	_y_dir = _x_dir = Direction::NONE;

	schedule("sound", 0.3f, [this]() {
		Audio::instance()->playSound("isaac dies new");
		});

	schedule("gameover", 0.5f, [this]() {

		Game::instance()->gameover();
		Game::instance()->freeze(true);

	});
}

void Isaac::hurt()
{
	if (_hurt)
		return;
	_hurt = true;
	_blinking = true;

	int soundChoice = rand() % 3 + 1;
	if (soundChoice == 1) {
		Audio::instance()->playSound("isaac_hurt");
	}
	else if (soundChoice == 2) {
		Audio::instance()->playSound("isaac_hurt_2");
	}
	else {
		Audio::instance()->playSound("isaac_hurt_3");
	}

	if (!_invincible)
	{
		HUD* hud = Game::instance()->hud();
		hud->subHalfHearts();
		if (!hud->halfHearts())
			die();
	}

	schedule("hurt_isaac", 0.2f, [this]() {_hurt = false; });
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
		break;
	case Direction::RIGHT:
		spawnPoint.y += (_isShootingRight ? -0.2f : -0.5f);
		spawnPoint.x += (_isShootingRight ? 0.0f : 0.5f);
		break;
	case Direction::UP:
		spawnPoint.x += (_isShootingRight ? 0.2f : -0.05f);
		spawnPoint.y -= 1.0f;
		break;
	case Direction::DOWN:
		spawnPoint.x += (_isShootingRight ? -0.05f : 0.2f);
		break;
	default:
		break;
	}

	// LAYER TEAR
	int layerTear = 12;
	if (dir == Direction::UP)
		layerTear = 10;

	if(_itemBone)
		Bone* newTear = new Bone(_scene, spawnPoint, dir, _vel.x, _vel.y, layerTear);
	else
		Tear* newTear = new Tear(_scene, spawnPoint, dir, _vel.x, _vel.y, false, layerTear);
	
	// alternate between left and right eye
	_isShootingRight = !_isShootingRight;
}


//cambia sprite ad isaac quando raccoglie un item
void Isaac::carryItem(bool on)
{
	_bombCarry = on;
	if (_bombCarry)
	{
		_sprite = _sprites["isaac_carry_bomb"];
		_body->setSprite(nullptr);
	}
	else
	{
		_sprite = _sprites["headFront"];
		_body->setSprite(_sprites["bodyFront"]);
		setSprite();
	}
}

void Isaac::setSprite()
{
	if (_hurt)
	{	
		_sprite = _sprites["hurt"];
		_body->setSprite(nullptr);
		return;
	}

	if (_itemBoneSprite)
	{
		_sprite = _sprites["isaac_carry_bomb"];
		_body->setSprite(nullptr);
		Audio::instance()->playSound("holy!");
		schedule("isaac_item_bone", 1.0f, [this]() {
			_itemBoneSprite = false;
			_sprite = _sprites["headFront"];
			_body->setSprite(_sprites["bodyFront"]);
			}, 0, false);
		return;
	}

	if (_walking && !_bombCarry)
	{
		if (_state[SDL_SCANCODE_D])
		{
			setFlip(SDL_FLIP_NONE);
			_body->setFlip(SDL_FLIP_NONE);
			_sprite = _sprites["headRight"];
			_body->setSprite(_sprites["walkRight"]);
		}
		else if (_state[SDL_SCANCODE_A])
		{
			setFlip(SDL_FLIP_HORIZONTAL);
			_body->setFlip(SDL_FLIP_HORIZONTAL);
			_sprite = _sprites["headRight"];
			_body->setSprite(_sprites["walkRight"]);
		}
		else if (_state[SDL_SCANCODE_S])
		{
			_sprite = _sprites["headFront"];
			_body->setSprite(_sprites["walkDown"]);
		}
		else if (_state[SDL_SCANCODE_W])
		{
			_sprite = _sprites["headBack"];
			_body->setSprite(_sprites["walkDown"]);
		}
		
	}

	if (!_walking)
	{
		setFlip(SDL_FLIP_NONE);
		_sprite = _sprites["headFront"];
		_body->setSprite(_sprites["bodyFront"]);
	}

	if (_isShooting)
	{
		if (_state[SDL_SCANCODE_UP]){
			if (_swapShootAnimation) {
				_sprite = _sprites["headBack"];
				_swapShootAnimation = false;
			}
			else {
				_sprite = _sprites["headBackShoot"];
			}
		}
		else if (_state[SDL_SCANCODE_DOWN]) {
			if (_swapShootAnimation) {
				_sprite = _sprites["headFront"];
				_swapShootAnimation = false;
			}
			else {
				_sprite = _sprites["headFrontShoot"];
			}
		}
		else if (_state[SDL_SCANCODE_RIGHT]) {
			setFlip(SDL_FLIP_NONE);
			if (_swapShootAnimation) {
				_sprite = _sprites["headRight"];
				_swapShootAnimation = false;
			}
			else {
				_sprite = _sprites["headRightShoot"];
			}
		}
		else if (_state[SDL_SCANCODE_LEFT]) {
			setFlip(SDL_FLIP_HORIZONTAL);
			if (_swapShootAnimation) {
				_sprite = _sprites["headRight"];
				_swapShootAnimation = false;
			}
			else {
				_sprite = _sprites["headRightShoot"];
			}
		}
	}
}


