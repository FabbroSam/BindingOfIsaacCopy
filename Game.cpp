// ----------------------------------------------------------------
// From "Algorithms and Game Programming" in C++ by Alessandro Bria
// Copyright (C) 2024 Alessandro Bria (a.bria@unicas.it). 
// All rights reserved.
// 
// Released under the BSD License
// See LICENSE in root directory for full details.
// ----------------------------------------------------------------

#include <algorithm>
#include "Game.h"
#include "Window.h"
#include "GameScene.h"
#include "View.h"
#include "Object.h"
#include "timeUtils.h"
#include "stringUtils.h"
#include "SpriteFactory.h"
#include "LevelLoader.h"
#include "HUD.h"
#include "Menu.h"
#include "UIMonster.h"
#include "Audio.h"


using namespace agp;

Game* Game::instance()
{
	static Game uniqueInstance;
	return &uniqueInstance;
}

Game::Game()
{
	_scenesToPop = 0;
	_running = false;
	_reset = false;
	_window = new Window("Binding of Isaac", int(aspectRatio() * 800), 800);
}

void Game::init()
{
	std::cout << "**************************************************************" << std::endl;
	std::cout << "********************** BINDING OF ISAAC **********************" << std::endl;
	std::cout << "**************************************************************" << std::endl;
	std::cout << "************************** COMANDI ***************************" << std::endl;
	std::cout << "**************************************************************" << std::endl;
	std::cout << "KEY I : invincibilita'" << std::endl;
	std::cout << "KEY C : mostra collider" << std::endl;
	std::cout << "KEY V : mostra bordi di tutti i RenderableObject" << std::endl;
	std::cout << "KEY O : apri/chiudi porte" << std::endl;
	std::cout << "KEY L : mostra tutte le stanze dellaminimappa" << std::endl;
	std::cout << "KEY L : attiva/disattiva movimento ampio view" << std::endl;
	std::cout << "MOUSE WHEEL: se KEY P attivo -> zoom" << std::endl;
	std::cout << "**************************************************************" << std::endl;
	std::cout << "**************************************************************" << std::endl;
	std::cout << "**************************************************************" << std::endl;
	std::cout << "**************************************************************" << std::endl;
	std::cout << "**************************************************************" << std::endl;
	

	_hud = new HUD();
	_uiMonster = new UIMonster();
	pushScene(LevelLoader::instance()->load("basement"));
	pushScene(_hud);
	pushScene(_uiMonster);
	pushScene(Menu::startMenu());

}

void Game::run()
{
	init();
	_running = true;

	FPS fps;
	Timer <float> frameTimer;

	while (_running)
	{

		processEvents();

		float frameTime = frameTimer.restart();
		for (auto& layer : _scenes)
			layer->update(frameTime);

		_window->render(_scenes);

		if(fps.update(false))
			_hud->setFPS(int(round(fps.lastFPS())));
	}

	destroy();
}

void Game::destroy()
{
	for (auto scene : _scenes)
		delete scene;

	init();
	if(_window)
		delete _window;

	SDL_Quit();

}

void Game::processEvents()
{
	SDL_Event evt;
	while (SDL_PollEvent(&evt))
		dispatchEvent(evt);

	// if there are scenes to be deleted, better to do this after event dispatching
	for (; _scenesToPop > 0; _scenesToPop--)
		popScene();
	// same for reset
	if (_reset)
	{
		_reset = false;
		for (auto& scene : _scenes)
		{
			if(scene)
				delete scene;
		}
		_scenes.clear();
		
		init();
	}
}

void Game::dispatchEvent(SDL_Event& evt)
{
	// window events are dispatched to all scenes for their views adjustments
	if (evt.type == SDL_WINDOWEVENT)
		for (auto& scene : _scenes)
			scene->event(evt);

	// all other events are dispatched from top to down through the scene stack
	// if a blocking layer is encountered, event propagation stops
	else if (_scenes.size())
	{
		for (int i = int(_scenes.size()) - 1; i >= 0; i--)
		{
			_scenes[i]->event(evt);
			if (_scenes[i]->blocking())
				break;
		}
	}
}

void Game::pushScene(Scene* scene)
{
	if(scene->blocking() && _scenes.size())
		for(auto & scene : _scenes)
			scene->setActive(false);

	_scenes.push_back(scene);
}

void Game::popScene()
{
	if (_scenes.size())
	{
		// last scene is blocking lower scenes
		if (_scenes.back()->blocking())
		{
			// reactivate lower scenes down to first blocking scene
			for (int i = int(_scenes.size()) - 2; i >= 0; i--)
			{
				_scenes[i]->setActive(true);
				if (_scenes[i]->blocking())
					break;
			}
		}

		delete _scenes.back();
		_scenes.pop_back();
	}
}

void Game::freeze(bool on)
{
	GameScene* gameScene = dynamic_cast<GameScene*>(_scenes[0]);
	if (gameScene)
	{
		auto objects = gameScene->objects();
		for (auto& obj : objects)
			if (obj != reinterpret_cast<Object*>(gameScene->player()))
				obj->setFreezed(on);
	}
}

void Game::gameover()
{
	UIScene* gameoverScreen = new UIScene(RectF(0, 0, 16, 15));
	gameoverScreen->setBackgroundColor(Color(0, 0, 0));
	new RenderableObject(gameoverScreen, RectF(0, 0, 16, 15), SpriteFactory::instance()->get("gameover"));
	pushScene(gameoverScreen);
	Audio::instance()->playMusic("gameover", 0);
}