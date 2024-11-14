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

namespace agp
{
	class HUD;
}

// HUD
// - implements (as an example) the Super Mario Bros's HUD
class agp::HUD : public UIScene
{
	protected:

		std::map<std::string, Sprite*> _sprites;

		// raw data
		std::vector<RenderableObject*> _hearts;
		float _totalHearts;

		// rendering
		RenderableObject* _heart1;
		RenderableObject* _heart2;
		RenderableObject* _heart3;

		RenderableObject* _coin;
		RenderableObject* _bomb;

		std::map< std::pair<int, int>, RenderableObject*> _roomsMinimap;
		MovableObject* _roomSelected;
		Vec2D<int> _pos;

		HUD();

	public:

		// singleton
		static HUD* instance();

		// getters/setters (to be completed)
		void setFPS(float fps);
		void setHearts(float amount);
		void drawMinimap(RectF rect, RoomType roomType);
		void selectMinimapRoom(float x, float y);
		void showMinimap();

		// extends update logic (+time management)
		virtual void update(float timeToSimulate) override;
};