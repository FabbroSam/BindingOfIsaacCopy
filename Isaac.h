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
#include <map>
#include <string>

namespace agp
{
	class Isaac;
}

class agp::Isaac : public DynamicObject
{
	private:

		RenderableObject* _body;
		bool _walking;
		bool _running;
		bool _jumping;
		bool _invincible;
		bool _dying;
		bool _dead;
		
		float _shootAnimationTime = 0.15f;
		float _shootTimer = 0.0f;
		float _shootCooldown = 0.4f;
		bool _isShooting = false;
		bool _canShoot = true;
		bool _isShootingAnimation = false;

		float _x_dec_rel = 20;
		float _y_dec_rel = 20;
		Direction _prev_x_dir = Direction::NONE;
		Direction _prev_y_dir = Direction::NONE;

		double _x_vel_last_nonzero;	// last nonzero horizontal velocity
		double _y_vel_last_nonzero;	// last nonzero horizontal velocity

		std::map<std::string, Sprite*> _sprites;

	public:

		Isaac(Scene* scene, const PointF& pos);

		// getters/setters
		bool invincible() { return _invincible; }

		// extends game logic (+isaac logic)
		virtual void update(float dt) override;

		// player actions
		virtual void move_x(Direction dir) override;
		virtual void move_y(Direction dir) override;
		virtual void run(bool on = true);

		// scripted actions
		virtual void die();
		virtual void hurt();

		void shoot(Direction dir);

		virtual std::string name() override { return strprintf("Isaac[%d]", _id); }
};