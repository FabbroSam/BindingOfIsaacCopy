// ----------------------------------------------------------------
// From "Algorithms and Game Programming" in C++ by Alessandro Bria
// Copyright (C) 2024 Alessandro Bria (a.bria@unicas.it). 
// All rights reserved.
// 
// Released under the BSD License
// See LICENSE in root directory for full details.
// ----------------------------------------------------------------

#pragma once
#include "RenderableObject.h"

namespace agp
{
	class Scene;
	class MovableObject;
	class Sprite;
}

// MovableObject class.
// - implements free physics (no collisions)
// - provides physics state queries (falling, midair, ...)
// - provides basic physics actions (e.g. move, jump, ...)
class agp::MovableObject : public RenderableObject
{
	protected:

		// physics parameters (expressed in scene units/s)

		float _x_acc;			// horizontal acceleration due to movement
		float _x_dec_rel;		// horizontal deceleration due to movement release
		float _x_dec_skd;		// horizontal deceleration due to skidding (movement change)
		float _x_vel_max;		// maximum horizontal velocity
		float _x_vel_min;		// minimum horizontal velocity

		float _y_acc;			// horizontal acceleration due to movement
		float _y_dec_rel;		// horizontal deceleration due to movement release
		float _y_dec_skd;		// horizontal deceleration due to skidding (movement change)
		float _y_vel_max;		// maximum vertical velocity
		float _y_vel_min;		// minimum vertical velocity

		virtual void defaultPhysics();

		// attributes
		Direction _x_dir;		// current horizontal movement direction
		Direction _y_dir;       // current vertical movement direction
		Vec2Df _vel;			// current velocity
		Vec2Df _prev_vel;		// velocity in the previous iteration

	public:

		MovableObject(Scene* scene, const RectF& rect, Sprite* sprite, int layer = 0, float angle = 0, SDL_RendererFlip flip = SDL_FLIP_NONE);
		virtual ~MovableObject() {}

		// getters / setters
		Vec2Df vel() const { return _vel; }
		void velAdd(Vec2Df amount);
		void velClip(float vx, float vy);
		void setVelX(float vx) { _vel.x = vx; }
		void setVelY(float vy) { _vel.y = vy; }
		void moveBy(Vec2Df amount) { _rect.pos += amount;}

		// state queries
		bool skidding() const;

		// actions
		virtual void move_x(Direction dir);
		virtual void move_y(Direction dir);
		
		// extends game logic (+physics)
		virtual void update(float dt) override;

		virtual std::string name() override {
			return strprintf("MovableObject[%d]", _id);
		}
};