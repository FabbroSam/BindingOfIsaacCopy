// ----------------------------------------------------------------
// From "Algorithms and Game Programming" in C++ by Alessandro Bria
// Copyright (C) 2024 Alessandro Bria (a.bria@unicas.it). 
// All rights reserved.
// 
// Released under the BSD License
// See LICENSE in root directory for full details.
// ----------------------------------------------------------------

#include "Sprite.h"
#include <iostream>

using namespace agp;

Sprite::Sprite(SDL_Texture* spritesheet, const RectI& rect, std::string name, SDL_RendererFlip flip, Uint8 alpha)
{
	_spritesheet = spritesheet;
	_rect = rect;
	_name = name;
	_alpha = alpha;

}

void Sprite::render(SDL_Renderer* renderer, const RectF& drawRect, Transform camera, float angle,			// rotation in degrees, clockwise
	SDL_RendererFlip flip)
{
	SDL_Rect srcRect = _rect.toSDL();
	
	SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
	SDL_SetTextureAlphaMod(_spritesheet, _alpha);

	auto vertices = drawRect.vertices();
	SDL_FRect drawRect_sdl = RectF(camera(vertices[0]), camera(vertices[2])).toSDLf();

	SDL_RenderCopyExF(renderer, _spritesheet, &srcRect, &drawRect_sdl, -angle, 0, flip);
}