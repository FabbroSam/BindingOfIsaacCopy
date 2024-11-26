// ----------------------------------------------------------------
// From "Algorithms and Game Programming" in C++ by Alessandro Bria
// Copyright (C) 2024 Alessandro Bria (a.bria@unicas.it). 
// All rights reserved.
// 
// Released under the BSD License
// See LICENSE in root directory for full details.
// ----------------------------------------------------------------

#pragma once

#include "DynamicObject.h"

namespace agp
{
	class Enemy;
}

// Enemy
// - base class for all enemies
class agp::Enemy : public DynamicObject
{
protected:

	float _life;
	bool _hitable;
	bool _dying;
	bool _movable;
	float _spawnDelay;
	RenderableObject* _shadow;
	RenderableObject* _poof;


public:

	Enemy(Scene* scene, const RectF& rect, Sprite* sprite, float _spawnDelay, int layer = 0);

	// actions
	virtual void spawn() = 0;
	virtual void move() = 0;
	virtual void hit(float damage, Vec2Df _dir = {0,0}) = 0;
	virtual void die() = 0;


	// extends logic collision (+smashed, +hurt Isaac)
	virtual bool collision(CollidableObject* with, Direction fromDir) override;

	virtual std::string name() override { return strprintf("Enemy[%d]", _id); }
};