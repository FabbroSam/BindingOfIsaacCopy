// ----------------------------------------------------------------
// From "Algorithms and Game Programming" in C++ by Alessandro Bria
// Copyright (C) 2024 Alessandro Bria (a.bria@unicas.it). 
// All rights reserved.
// 
// Released under the BSD License
// See LICENSE in root directory for full details.
// ----------------------------------------------------------------

#pragma once
#include "UIScene.h"
#include "RenderableObject.h"
#include "MovableObject.h"
#include <iostream>

namespace agp
{
	class HUD;
}

class agp::HUD : public Scene
{
		bool _event = false;

		int _coins;
		int _bombs;
		int _halfHearts;
		int _heartsCapacity;	
		RenderableObject* _fpsText;	
		RenderableObject* _coinsText;
		RenderableObject* _bombsText;
		RenderableObject* _heartIcons[5];

		std::map< std::pair<int, int>, RenderableObject*> _roomsMinimap;
		MovableObject* _roomSelected;
		Vec2D<int> _pos;

		std::map<std::string, Sprite*> _sprites;

		void refreshHearts();

	public:

		HUD();
		virtual ~HUD() {};


		void setEvent() { _event = true; }
		// getters/setters (to be completed)
		void setCoins(int newCoins);
		void addCoins() { setCoins(_coins + 1); }
		void setBombs(int newBombs);
		void setHalfHearts(int newHalfHearts);
		void subHalfHearts() { setHalfHearts(_halfHearts - 1); }
		void setHeartsCapacity(int newCapacity);
		void setFPS(int fps);
		int coins() { return _coins; }
		int bombs() { return _bombs; }
		int halfHearts() { return _halfHearts; }


		// logic minimap
		void drawMinimap(RectF rect, RoomType roomType);
		void selectMinimapRoom(float x, float y);
		void showMinimap();

		// extends update logic (+time management)
		virtual void update(float timeToSimulate) override;

		// extends event handler (+item selection)
		virtual void event(SDL_Event& evt) override;
};