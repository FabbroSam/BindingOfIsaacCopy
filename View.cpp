// ----------------------------------------------------------------
// From "Algorithms and Game Programming" in C++ by Alessandro Bria
// Copyright (C) 2024 Alessandro Bria (a.bria@unicas.it). 
// All rights reserved.
// 
// Released under the BSD License
// See LICENSE in root directory for full details.
// ----------------------------------------------------------------

#include "SDL.h"
#include "View.h"
#include "Window.h"
#include "Game.h"
#include "Scene.h"
#include "Object.h"
#include "RenderableObject.h"
#include "timeUtils.h"

using namespace agp;

View::View(Scene* scene, const RectF& rect)
{
	_scene = scene;	
	_rect = rect;
	_viewport = RectF(0.0f, 0.0f, 1.0f, 1.0f);
	_magf = PointF(1, 1);
	_aspectRatio = 0;
	updateViewport();

	_to_x = 0;
	_to_y = 0;
	_dir = Direction::NONE;
	_move_to = false;
}

void View::setScene(Scene* scene) 
{ 
	_scene = scene; 
	_rect = scene->rect(); 
}

void View::move(const Vec2Df& ds)
{
	_rect.pos += ds;
	updateViewport();
}

void View::moveTransition(Direction dir)
{
	if (_dir == Direction::NONE)
	{
		_move_to = true;
		if (dir == Direction::RIGHT)
			_to_x = _rect.pos.x + 16;
		else if (dir == Direction::LEFT)
			_to_x = _rect.pos.x - 16;
		else if (dir == Direction::UP)
			_to_y = _rect.pos.y - 12;
		else if	(dir == Direction::DOWN)
			_to_y = _rect.pos.y + 12;
		_dir = dir;
	}

}

void View::move(float dx, float dy)
{
	_rect.pos.x += dx;
	_rect.pos.y += dy;
	updateViewport();
}

void View::scale(float f)
{
	_rect *= f;
	updateViewport();
}

void View::render()
{
	SDL_Renderer* renderer = Game::instance()->window()->renderer();

	// viewport clipping
	SDL_Rect viewport_r = _viewportAbs.toSDL();
	SDL_RenderSetClipRect(renderer, &viewport_r); 
	
	// viewport background
	SDL_SetRenderDrawColor(renderer, _scene->backgroundColor().r, _scene->backgroundColor().g, _scene->backgroundColor().b, _scene->backgroundColor().a);
	SDL_RenderFillRect(renderer, &viewport_r);

	// render objects
	auto objects = _scene->objects(_rect);
	for (auto& obj : objects)
	{
		RenderableObject* robj = dynamic_cast<RenderableObject*>(obj);
		if(robj)
			robj->draw(renderer, _scene2view);
	}
}

void View::update(float dt)
{
	float step = 0.68f; // velocit� con cui si sposta la view

	if (_move_to)
	{
		if (_dir != Direction::NONE)
		{
			if (_dir == Direction::RIGHT)
				_rect.pos.x += step;
			else if (_dir == Direction::LEFT)
				_rect.pos.x += -step;
			else if (_dir == Direction::UP)
				_rect.pos.y += -step;
			else if (_dir == Direction::DOWN)
				_rect.pos.y += step;
		}

		if (_to_x - 2 * step <= _rect.pos.x && _to_x + 2 * step >= _rect.pos.x && _to_y - 2 * step <= +_rect.pos.y && _to_y + 2 * step >= _rect.pos.y)
		{
			_rect.pos.x = _to_x;
			_rect.pos.y = _to_y;
			_dir = Direction::NONE;
			_move_to = false;
		}
	}

	updateViewport();
}

void View::updateViewport()
{
	SDL_Renderer* renderer = Game::instance()->window()->renderer();

	// get renderer size on screen
	int rendWidth, rendHeight;
	SDL_GetRendererOutputSize(renderer, &rendWidth, &rendHeight);

	// update viewport absolute coordinates
	_viewportAbs = RectF(
		_viewport.pos.x * rendWidth,
		_viewport.pos.y * rendHeight,
		_viewport.size.x * rendWidth,
		_viewport.size.y * rendHeight);

	// correct aspect ratio
	if (_aspectRatio)
	{
		float currentRatio = _viewportAbs.size.x / _viewportAbs.size.y;
		if (currentRatio > _aspectRatio) // too wide
		{
			float newWidth = _viewportAbs.size.y * _aspectRatio;
			_viewportAbs.pos.x += (_viewportAbs.size.x - newWidth) / 2;
			_viewportAbs.size.x = newWidth;
		}
		else // too tall
		{
			float newHeight = _viewportAbs.size.x / _aspectRatio;
			_viewportAbs.pos.y += (_viewportAbs.size.y - newHeight) / 2;
			_viewportAbs.size.y = newHeight;
		}
	}

	// update magnification factor
	_magf.x = _viewportAbs.size.x / _rect.size.x;
	_magf.y = _viewportAbs.size.y / _rect.size.y;

	// update transforms
	_scene2view = [this](const PointF& p)
	{
		return PointF(
			_viewportAbs.pos.x + (p.x - _rect.pos.x) * _magf.x,
			_viewportAbs.pos.y + (p.y - _rect.pos.y) * _magf.y);
	};
	_view2scene = [this](const PointF& p)
	{
		return PointF(
			(p.x - _viewportAbs.pos.x) / _magf.x + _rect.pos.x,
			(p.y - _viewportAbs.pos.y) / _magf.y + _rect.pos.y);
	};
}

PointF View::mapToScene(const PointF& p)
{
	return _view2scene(p);
}

PointF View::mapFromScene(const PointF& p)
{
	return _scene2view(p);
}

PointF View::mapToScene(float x, float y)
{
	return _view2scene(PointF(x, y));
}

PointF View::mapToScene(int x, int y)
{
	return mapToScene(float(x), float(y));
}

PointF View::mapFromScene(float x, float y)
{
	return _scene2view(PointF(x, y));
}

RectF View::mapToScene(const RectF& r)
{
	return RectF(
		(r.pos.x - _viewportAbs.pos.x) / _magf.x + _rect.pos.x,
		(r.pos.y - _viewportAbs.pos.y) / _magf.y + _rect.pos.y,
		r.size.x / _magf.x,
		r.size.y / _magf.y);
}

RectF View::mapFromScene(const RectF& r)
{
	return RectF(
		_viewportAbs.pos.x + (r.pos.x - _rect.pos.x) * _magf.x,
		_viewportAbs.pos.y + (r.pos.y - _rect.pos.y) * _magf.y,
		r.size.x * _magf.x,
		r.size.y * _magf.y);
}