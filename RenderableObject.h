// ----------------------------------------------------------------
// From "Algorithms and Game Programming" in C++ by Alessandro Bria
// Copyright (C) 2024 Alessandro Bria (a.bria@unicas.it). 
// All rights reserved.
// 
// Released under the BSD License
// See LICENSE in root directory for full details.
// ----------------------------------------------------------------

#pragma once
#include "graphicsUtils.h"
#include "Object.h"
#include "Sprite.h"
#include <iostream>

namespace agp
{
	class Scene;
	class RenderableObject;
}

// RenderableObject class.
// - stores render data (e.g. color, texture, ...)
// - offers draw method for graphics rendering
class agp::RenderableObject : public Object
{
	protected:

		Color _color;
		Sprite* _sprite;
		bool _visible;			// if false, does not draw
		bool _focused;			// if true, fills with _focusColor color
		SDL_RendererFlip _flip;	// horizontal/vertical flipping
		float _angle;			// generic rotation (degrees/s, clockwise)
		float _angularVelocity;	// generic rotation (degrees/s, clockwise)
		Color _focusColor;
		Color _borderColor;

	public:

		RenderableObject(Scene* scene, const RectF& rect, const Color& color, int layer = 0);
		RenderableObject(Scene* scene, const RectF& rect, Sprite* sprite, int layer = 0, float angle = 0, SDL_RendererFlip flip = SDL_FLIP_NONE);
		virtual ~RenderableObject() {}

		// getters/setters
		const Color& color() { return _color; }
		void setBorderColor(const Color& borderColor) { _borderColor = borderColor; }
		void setVisible(bool visible) { _visible = visible; }
		virtual void setSprite(Sprite* sprite, bool deallocateSprite = false, bool resetOnChange = true);
		void setFlip(SDL_RendererFlip flip) { _flip = flip; }
		void setFocused(bool focused) { _focused = focused; };
		void setAngularVelocity(float angular) { _angularVelocity = angular; }
		Sprite* sprite() { return _sprite; }

		// extends game logic (+animation)
		virtual void update(float dt) override;

		// core game rendering
		virtual void draw(SDL_Renderer* renderer, Transform camera);

		virtual std::string name() override;
};