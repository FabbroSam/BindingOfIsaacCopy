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

namespace agp
{
	class HUD;
}

// HUD
// - implements (as an example) the Super Mario Bros's HUD
class agp::HUD : public UIScene
{
	protected:

		// raw data
		std::map<RenderableObject*, float> _hearts;

		// rendering
		RenderableObject* _heart1;
		RenderableObject* _heart2;
		RenderableObject* _heart3;

	public:

		HUD();
		virtual ~HUD() {};

		// getters/setters (to be completed)
		void setFPS(int fps);

		// extends update logic (+time management)
		virtual void update(float timeToSimulate) override;
};