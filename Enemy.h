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

		bool _smashable;
		bool _dying;
		Direction _facingDir;
		float _spawnDelay;
		RenderableObject* _shadow;
		RenderableObject* _poof;

	public:

		Enemy(Scene* scene, const RectF& rect, Sprite* sprite,float _spawnDelay, int layer = 0);

		Direction facingDir() { return _facingDir; }

		// actions
		//virtual void stomp();					// isaac jumps on top of the enemy
		//virtual void kick(bool right = true);	// isaac kicks from one side
		//virtual void impulse(float dt, Direction fromDir);
		virtual void smash();					// hit by invincible isaac, fireball, shell, or block bump
		virtual void hurt()=0;
		// extends logic collision (+smashed, +hurt Isaac)
		virtual bool collision(CollidableObject* with, Direction fromDir) override;

		virtual std::string name() override { return strprintf("Enemy[%d]", _id); }
};






