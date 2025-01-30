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
#include <string>
#include <vector>
#include <functional>
#include <iostream>

namespace agp
{
	class MenuItem;
	class Menu;
}

// MenuItem
// - implements a simple textSprite-based menu item
class agp::MenuItem : public RenderableObject
{
	protected:

		Menu* _container;
		int _index;
		int _selected;
		std::string _text;
		std::function<void(SDL_Scancode)> _task;	// performed when Enter pressed on this item
		RenderableObject* _menuArrow;
		RenderableObject* _volume;
		RenderableObject* _vsync1;
		RenderableObject* _vsync2;
	public:

		MenuItem(Menu* container, int index, float height, const std::string& text, std::function<void(SDL_Scancode)> task, bool notches = false, bool vsync = false);
		virtual ~MenuItem() {};	

		bool selected() { return _selected; }
		void setSelected(bool on) { _selected = on;}
		const std::string& text() { return _text; }
		void setText(const std::string& text) { _text = text;}

		RenderableObject* volume() { return _volume; }
		RenderableObject* vsync1() { return _vsync1; }
		RenderableObject* vsync2() { return _vsync2; }
		void refresh();

		void run(SDL_Scancode code = SDL_SCANCODE_UNKNOWN) { _task(code); }

		// extends update logic (+selection)
		virtual void update(float dt) override;
};

// Menu
// - implements a simple multiple-choice Menu
class agp::Menu : public UIScene
{
protected:

	Menu* _parent;
	RectF _menuRect;
	int _itemSelected;
	bool _closable;
	bool _vsyncOn;
	std::vector< MenuItem* > _items;
	RenderableObject* _menuBackground;

public:

		Menu(const PointF& position, float width, Menu* parent = 0, bool closable = true);
		Menu(Menu* parent);
		virtual ~Menu() {};

		// getters/setters (to be completed)
		const RectF& menuRect() { return _menuRect; }
		MenuItem* addItem(const std::string& text, float height, std::function<void(SDL_Scancode)> task, bool notches = false, bool vsync = false);
		MenuItem* itemAt(int index) { return _items[index]; }
		

		// extends event handler (+menu selection)
		virtual void event(SDL_Event& evt) override;

		// provide main menus
		static Menu* mainMenu();
		static Menu* pauseMenu();
		static Menu* startMenu();

};