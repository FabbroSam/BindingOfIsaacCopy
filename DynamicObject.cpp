// ----------------------------------------------------------------
// From "Algorithms and Game Programming" in C++ by Alessandro Bria
// Copyright (C) 2024 Alessandro Bria (a.bria@unicas.it). 
// All rights reserved.
// 
// Released under the BSD License
// See LICENSE in root directory for full details.
// ----------------------------------------------------------------

#include "DynamicObject.h"
#include "StaticObject.h"
using namespace agp;

DynamicObject::DynamicObject(Scene* scene, const RectF& rect, Sprite* sprite, int layer) :
	CollidableObject(scene, rect, sprite, layer)
{
	// dynamic objects are compenetrable vs. each other by default
	// (e.g. player vs. spanwable, collectibles vs. enemies, ...)
	// compenetration does not need to be resolved in these cases
	// since the collision is resolved "logically" by the collision method
	_compenetrable = true;
}

void DynamicObject::resolveCollisions()
{
		if(!_compenetrable)
			for (int i = 0; i < _collisions.size(); i++)
			{
			
				DynamicObject* dynObj = _collisions[i]->to<DynamicObject*>();
				StaticObject* staticObj = _collisions[i]->to<StaticObject*>();

				// Dynamic vs. Static: hard non-compenetration constraint
				if (staticObj && !staticObj->compenetrable())
					_rect.pos += -_collisionAxes[i] * _collisionDepths[i];

				// Dynamic vs. Dynamic: soft non-compenetration constraint
				if (dynObj && !dynObj->compenetrable())
					_rect.pos += -_collisionAxes[i] * _collisionDepths[i] / 10.0f;
			}
}