// ----------------------------------------------------------------
// From "Algorithms and Game Programming" in C++ by Alessandro Bria
// Copyright (C) 2024 Alessandro Bria (a.bria@unicas.it). 
// All rights reserved.
// 
// Released under the BSD License
// See LICENSE in root directory for full details.
// ----------------------------------------------------------------

#include "RenderableObject.h"
#include "Scene.h"
#include "Game.h"
#include <iostream>
using namespace agp;

RenderableObject::RenderableObject(Scene* scene, const RectF& rect, const Color& color, int layer)
	: Object(scene, rect, layer)
{
	_color = color;
	_flip = SDL_FLIP_NONE;
	_angle = 0;
	_angularVelocity = 0;
	_focused = false;
	_visible = true;
	_sprite = nullptr;
	_focusColor = { 255, 0, 0, 255 };
	_borderColor = { 0, 0, 0, 0 };
}

RenderableObject::RenderableObject(Scene* scene, const RectF& rect, Sprite* sprite, int layer, float angle, SDL_RendererFlip flip)
	: Object(scene, rect, layer)
{
	_color = { 0,0,0,0 };
	_flip = flip;
	_angle = angle;
	_angularVelocity = 0;
	_sprite = sprite;
	_focused = false;
	_visible = true;
	_focusColor = { 255, 0, 0, 255 };
	_borderColor = { 0, 0, 0, 0 };
}

void RenderableObject::draw(SDL_Renderer* renderer, Transform camera)
{
	if (!_visible)
		return;

	SDL_FRect drawRect = RectF(camera(_rect.tl()), camera(_rect.br())).toSDLf();

	if (_sprite)
		_sprite->render(renderer, _rect, camera, _angle, _flip);
	else
	{
		SDL_SetRenderDrawColor(renderer, _color.r, _color.g, _color.b, _color.a);
		SDL_RenderFillRectF(renderer, &drawRect);
	}

	if (Game::instance()->bordersVisible())
	{
		SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
		SDL_RenderDrawRectF(renderer, &drawRect);
	}

}

void RenderableObject::update(float dt)
{
	Object::update(dt);

	_angle += _angularVelocity * dt;
	if (_angle >= 360)
		_angle -= 360;

	if (_sprite)
		_sprite->update(dt);

}

void RenderableObject::setSprite(Sprite* sprite, bool deallocateSprite, bool resetOnChange)
{
	if (_sprite)
	{
		if (resetOnChange && sprite != _sprite)
			_sprite->reset();
		if (deallocateSprite)
			delete _sprite;
	}

	_sprite = sprite;
}

std::string RenderableObject::name()
{
	if(_sprite)
		return strprintf("RenderableObject[%10s]",_sprite->name());
	else
		return strprintf("RenderableObject[%d]", _id);
}