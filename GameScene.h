// ----------------------------------------------------------------
// From "Algorithms and Game Programming" in C++ by Alessandro Bria
// Copyright (C) 2024 Alessandro Bria (a.bria@unicas.it). 
// All rights reserved.
// 
// Released under the BSD License
// See LICENSE in root directory for full details.
// ----------------------------------------------------------------

#pragma once
#include "Scene.h"
#include "Room.h"


namespace agp
{
	class GameScene;
	class Isaac;
	class Door;
	class Basement;
}

// GameScene (or World) class
// - specialized update(dt) to semifixed timestep
// - may provide more efficient access to game objects (e.g. quadtree)
// - can/should be customized for the specific game to implement (here, superisaacbros-like)
// - stores the main player and implements player controls
class agp::GameScene : public Scene
{
	private:

		float _dt;					// time integration step
		float _timeToSimulate;		// time integration extent

		Isaac* _isaac;
		bool _d_pressed;
		bool _a_pressed;
		bool _w_pressed;
		bool _s_pressed;
		bool _run_pressed;

		bool _collidersVisible;
		bool _bordersVisible;

		float _cameraZoomVel;		// camera zoom velocity (in [0,1] relative scale units)
		bool _right_pressed;
		bool _left_pressed;
		bool _up_pressed;
		bool _down_pressed;

		bool _moveView;
		bool _moveIsaac;

		bool _vsMonster;

		Room* _room;
		Basement* _mapRooms;

		std::list<Object*> _enemiesInRoom;

		Scheduler* _schedule;

	public:

		GameScene(const RectF& r, float dt);
		virtual ~GameScene() {};

		Isaac* player() { return _isaac; }
		void setPlayer(Isaac* isaac) { _isaac = isaac; }
		void setMapRooms(Basement* mapRooms) { _mapRooms = mapRooms; }
		void setRooms(Room* room) { _room = room; }
		void setRooms(std::pair<int, int> coords) { _room = _mapRooms->room({ _room->coords().first + coords.first, _room->coords().second + coords.second}); }
		Room* room() { return _room; }
		bool collidersVisible() { return _collidersVisible; }
		void toggleColliders() { _collidersVisible = !_collidersVisible; }


		//action Game
		void spawnMobs();

		// overrides scene object selection (+octree, NOT implemented)
		//virtual std::list<Object*> objects(const RectF& cullingRect) override;

		// implements game scene update logic (+semifixed timestep)
		virtual void update(float timeToSimulate) override;

		// extends event handler (+camera translate/zoom, +brick creation/destroy, +pause menu)
		virtual void event(SDL_Event& evt) override;

};