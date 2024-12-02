// ----------------------------------------------------------------
// From "Algorithms and Game Programming" in C++ by Alessandro Bria
// Copyright (C) 2024 Alessandro Bria (a.bria@unicas.it). 
// All rights reserved.
// 
// Released under the BSD License
// See LICENSE in root directory for full details.
// ----------------------------------------------------------------

#include "CollidableObject.h"
#include "Scene.h"
#include <list>
#include <algorithm>
#include "timeUtils.h"
#include "collisionUtils.h"
#include "GameScene.h"
#include "HUD.h"
#include "DynamicObject.h"
#include "StaticObject.h"
#include <iostream>

using namespace agp;

CollidableObject::CollidableObject(Scene* scene, const RectF& rect, Sprite* sprite, int layer, float angle, SDL_RendererFlip flip) :
	MovableObject(scene, rect, sprite, layer, angle, flip)
{
	// default collider: object rect
	_collider = { 0, 0, _rect.size.x, _rect.size.y };

	// default collision: non compenetration
	_compenetrable = false;
	_collidable = true;
}

void CollidableObject::update(float dt)
{
	MovableObject::update(dt);

	detectCollisions();
	resolveCollisions();
}

RectF CollidableObject::sceneCollider() const
{
	return _collider + _rect.pos;
}

void CollidableObject::detectCollisions()
{
	if (!_collidable)
		return;
	
	_collisionsPrev = _collisions;
	_collisions.clear();
	_collisionAxes.clear();
	_collisionDepths.clear();

	auto objectsInRect = _scene->objects(sceneCollider());
	for (auto& obj : objectsInRect)
	{
		CollidableObject* collObj = obj->to<CollidableObject*>();
		if (collObj && collObj != this && collObj->collidable() && collidableWith(collObj) && collObj->collidableWith(this))
		{
			Vec2Df axis;
			float depth;
			if (checkCollisionSAT(sceneCollider().verticesVec(), collObj->sceneCollider().verticesVec(), axis, depth))
			{
				_collisions.push_back(collObj);
				_collisionAxes.push_back(axis);
				_collisionDepths.push_back(depth);
				collision(collObj, normal2dir(axis));
				collObj->collision(this, normal2dir(axis));
			}
		}
	}

	// remove objects marked 'to be killed' from collision list
	// since they will not be accessible in the next iteration
	//_collisions.erase(std::remove_if(_collisions.begin(), _collisions.end(), [](CollidableObject* obj) { return obj->_killed; }), _collisions.end());

	//for (auto collObj : _collisionsPrev)
	//{
	//	if (std::find(_collisions.begin(), _collisions.end(), collObj) == _collisions.end())
	//	{
	//		collision(collObj, normal2dir(Vec2Df()));
	//		collObj->collision(this, normal2dir(Vec2Df()));
	//	}
	//}

}

void CollidableObject::draw(SDL_Renderer* renderer, Transform camera)
{
	MovableObject::draw(renderer, camera);

	GameScene* gameScene = dynamic_cast<GameScene*>(_scene);
	if (gameScene && gameScene->collidersVisible())
	{
		auto vertices = sceneCollider().vertices();
		SDL_FRect drawRect = RectF(camera(vertices[0]), camera(vertices[2])).toSDLf();
		SDL_SetRenderDrawColor(renderer, _colliderColor.r, _colliderColor.g, _colliderColor.b, _colliderColor.a);
		SDL_RenderDrawRectF(renderer, &drawRect);
	}
	if (_focused)
	{
		auto vertices = sceneCollider().vertices();
		SDL_FRect drawRect = RectF(camera(vertices[0]), camera(vertices[2])).toSDLf();
		SDL_SetRenderDrawColor(renderer, _focusColor.r, _focusColor.g, _focusColor.b, _focusColor.a);
		SDL_RenderDrawRectF(renderer, &drawRect);
		schedule("focus", 1.0f, [this]()
			{
				_focused = false;
				printf("Stampa in CollidableObj: Shoot ---> ");
				if (_sprite)
					printf("%s[%d]\n", _sprite->name().c_str(), _id);
				else
					printf("%s\n", name().c_str());
			}, 0, false);
	}
}

float CollidableObject::distance(CollidableObject* obj) const
{
	return sceneCollider().center().distance(obj->sceneCollider().center());
}