// ----------------------------------------------------------------
// From "Algorithms and Game Programming" in C++ by Alessandro Bria
// Copyright (C) 2024 Alessandro Bria (a.bria@unicas.it). 
// All rights reserved.
// 
// Released under the BSD License
// See LICENSE in root directory for full details.
// ----------------------------------------------------------------

#pragma once
#include "CollidableObject.h"

namespace agp
{
	class StaticObject;
}

// StaticObject class.
// - provides base class for all objects that do not generally update physics
//   nor detect/resolve collisions
class agp::StaticObject : public CollidableObject
{
	protected:


	public:

		StaticObject(Scene* scene, const RectF& rect, Sprite* sprite, int layer = 0, float angle = 0, SDL_RendererFlip flip = SDL_FLIP_NONE);
		virtual ~StaticObject() {}

		// implements CollidableObject's abstract method (do nothing)
		virtual void resolveCollisions() override { }

		virtual std::string name() override {
			if (_sprite)
				return strprintf(_sprite->name());
			else
				return strprintf("StaticObject[%d]", _id);
		}
};