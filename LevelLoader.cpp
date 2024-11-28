// ----------------------------------------------------------------
// From "Algorithms and Game Programming" in C++ by Alessandro Bria
// Copyright (C) 2024 Alessandro Bria (a.bria@unicas.it). 
// All rights reserved.
// 
// Released under the BSD License
// See LICENSE in root directory for full details.
// ----------------------------------------------------------------


#include "LevelLoader.h"
#include "SpriteFactory.h"
#include "RenderableObject.h"
#include "StaticObject.h"
#include "GameScene.h"
#include "Isaac.h"
#include "Lift.h"
#include "Trigger.h"
#include "sdlUtils.h"
#include "geometryUtils.h"
#include "Room.h"

#include <iostream>

using namespace agp;

LevelLoader* LevelLoader::instance()
{
	static LevelLoader uniqueInstance;
	return &uniqueInstance;
}

LevelLoader::LevelLoader()
{
	// e.g. load level maps from disk
}

GameScene* LevelLoader::load(const std::string& name)
{
	SpriteFactory* spriteLoader = SpriteFactory::instance();

	if (name == "basement")
	{
		GameScene* world = new GameScene(RectF(-50, -50, 50, 50), 1 / 60.0f);
		world->setBackgroundColor(Color(0, 0, 0));

		Basement* mapRooms = new Basement();
		mapRooms->generateRooms(world);
		world->setMapRooms(mapRooms);
		world->setRooms(mapRooms->room({0,0}));




		// isaac
		Isaac* isaac = new Isaac(world, PointF(7.3f, 4.5f));
		world->setPlayer(isaac);

		return world;
	}
	else
	{
		std::cerr << "Unrecognized game scene name \"" << name << "\"\n";
		return nullptr;
	}
}
