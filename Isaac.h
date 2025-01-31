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
		RenderableObject* _shadow;
		bool _walking;
		bool _running;
		bool _jumping;
		bool _invincible;
		bool _hurt;
		bool _blinking;
		float _blinkTimeElapsed;
		int _blinkCount;
		bool _dying;
		bool _dead;
		

		float _shootAnimationTime;
		float _shootTimer;
		float _shootCooldown;
		bool _isShooting;
		bool _isShootingRight;
		Direction _shootingDirection;
		bool _canShoot;
		bool _isShootingAnimation;
		const Uint8* _state;
		float _shootAnimationTimer;
		float _shootAnimationInterval;
		bool _swapShootAnimation;
		bool _bombCarry;

		float _x_dec_rel;
		float _y_dec_rel;
		Direction _prev_x_dir = Direction::NONE;
		Direction _prev_y_dir = Direction::NONE;

		double _x_vel_last_nonzero;	// last nonzero horizontal velocity
		double _y_vel_last_nonzero;	// last nonzero horizontal velocity

		std::map<std::string, Sprite*> _sprites;

	public:

		Isaac(Scene* scene, const PointF& pos);

		// getters/setters
		bool invincible() { return _invincible; }
		void setInvincible() { _invincible = !_invincible; }
		void setSprite();
		void goTo(PointF pos) { _rect = RectF(pos.x, pos.y, _rect.size.x, _rect.size.y); }

		Vec2Df vel() { return _vel; }

		// extends game logic (+isaac logic)
		virtual void update(float dt) override;

		// player actions
		virtual void move_x(Direction dir) override;
		virtual void move_y(Direction dir) override;

		// scripted actions
		virtual void die();
		virtual bool died()const { return _dying; }
		virtual void hurt();
		void blink();
		void setBombCarry(bool on);

		void shoot(Direction dir);

		virtual std::string name() override { return strprintf("Isaac[%d]", _id); }
};