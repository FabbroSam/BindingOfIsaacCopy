// ----------------------------------------------------------------
// From "Algorithms and Game Programming" in C++ by Alessandro Bria
// Copyright (C) 2024 Alessandro Bria (a.bria@unicas.it). 
// All rights reserved.
// 
// Released under the BSD License
// See LICENSE in root directory for full details.
// ----------------------------------------------------------------

#include "Menu.h"
#include "SpriteFactory.h"
#include "View.h"
#include "Game.h"
#include "Window.h"
#include "Audio.h"
#include "Game.h"
#include <iostream>

using namespace agp;

MenuItem::MenuItem(Menu* container, int index, float height, const std::string& text, std::function<void(SDL_Scancode)> task, bool notches, bool vsync)
	: RenderableObject(container,
		RectF(
			container->menuRect().left() + 0.8f + 0.1f * index,
			container->menuRect().top()  + 0.3f  + (0.5f + 0.95f) * index,
			container->menuRect().size.x, 
			height),
		SpriteFactory::instance()->get(text))
{
	_container = container;
	_index = index;
	_task = task;
	_selected = false;
	_volume = nullptr;
	_vsync1 = nullptr;
	_vsync2 = nullptr;

	_menuArrow = new RenderableObject(container,
		RectF(
			container->menuRect().left() + 0.0f + 0.1f * index,
			container->menuRect().top() + 0.3f + (0.5f + 0.95f) * index,
			1.0f, height),
		SpriteFactory::instance()->get("menu_arrow"));
	_menuArrow->setVisible(false);

	if (notches)
	{
		int volume = 0;
		if (text == "menu_options_sfx")
			volume = Audio::instance()->volumeSfx();
		else if (text == "menu_options_music")
			volume = Audio::instance()->volumeMusic();	
		std::string spriteName = "menu_options_" + std::to_string(volume);

		_volume = new RenderableObject(container,
			RectF(
				container->menuRect().left() + 4.0f + 0.1f * index,
				container->menuRect().top() + 0.3f + (0.5f + 0.95f) * index,
				container->menuRect().size.x,
				height),
			SpriteFactory::instance()->get(spriteName));
	}
	if (vsync)
	{
		std::string text2 = "";
		SDL_RendererInfo info;
		SDL_GetRendererInfo(Game::instance()->window()->renderer(), &info);
		bool vsyncOn = info.flags & SDL_RENDERER_PRESENTVSYNC;
		if (vsyncOn)
		{
			_text = "menu_options_vsync_on_1";
			text2 = "menu_options_vsync_off_2";
		}
		else
		{
			_text = "menu_options_vsync_on_2";
			text2 = "menu_options_vsync_off_1";
		}
		_vsync1 = new RenderableObject(container,
			RectF(
				container->menuRect().left() + 4.0f + 0.1f * index,
				container->menuRect().top() + 0.3f + (0.5f + 0.95f) * index,
				container->menuRect().size.x,
				height),
			SpriteFactory::instance()->get(_text));
		_vsync2 = new RenderableObject(container,
			RectF(
				container->menuRect().left() + (5.0f + 1) + 0.1f * index,
				container->menuRect().top() + 0.3f + (0.5f + 0.95f) * index,
				container->menuRect().size.x,
				height),
			SpriteFactory::instance()->get(text2));
	}
}
void MenuItem::refresh()
{

}

void MenuItem::update(float dt)
{
	RenderableObject::update(dt);

	if (_selected)
		_menuArrow->setVisible(true);
	else
		_menuArrow->setVisible(false);
	//_focused = _selected;
}

Menu::Menu(const PointF& position, float width, Menu* parent, bool closable)
	: UIScene(RectF(0, 0, 16, 12))
{

	_parent = parent;
	_itemSelected = 0;
	_menuRect = { position.x, position.y, width, 0.5f };
	_closable = closable;

	// menu layer
	_menuBackground = new RenderableObject(this, _menuRect, Color(0, 0, 0, 0));
	_menuBackground->setBorderColor({ 0, 0, 0, 0});

	// default: modal menu (blocks all lower scenes)
	_blocking = true;

	// setup view
	_view = new View(this, _rect);
	_view->setFixedAspectRatio(Game::instance()->aspectRatio());
	_view->setRect(RectF(0, 0, 16, 12));

}

Menu::Menu(Menu* parent)
	: Menu(parent->menuRect().tl(), parent->menuRect().size.x, parent)
{
}

MenuItem* Menu::addItem(const std::string& text, float height, std::function<void(SDL_Scancode)> task, bool notches, bool vsync)
{
	MenuItem* item = new MenuItem(this, int(_items.size()), height, text, task, notches, vsync);
	_items.push_back(item);
	if (_items.size() == 1)
		item->setSelected(true);

	_menuRect.size.y = 0.5f + _items.size() * 0.7f + 0.3f;
	_menuBackground->setRect(_menuRect);

	return item;
}

void Menu::event(SDL_Event& evt)
{
	UIScene::event(evt);

	if (evt.type == SDL_KEYDOWN)
	{
		if (evt.key.keysym.scancode == SDL_SCANCODE_DOWN)
		{
			_items[_itemSelected]->setSelected(false);
			_itemSelected = (_itemSelected + 1) % int(_items.size());
			_items[_itemSelected]->setSelected(true);
		}
		else if (evt.key.keysym.scancode == SDL_SCANCODE_UP)
		{
			_items[_itemSelected]->setSelected(false);
			_itemSelected = (_itemSelected - 1) < 0 ? int(_items.size()) - 1 : _itemSelected - 1;
			_items[_itemSelected]->setSelected(true);
		}
		else if (evt.key.keysym.scancode == SDL_SCANCODE_RETURN)
		{
			_items[_itemSelected]->run();
		}
		else if (_closable && (evt.key.keysym.scancode == SDL_SCANCODE_ESCAPE))
		{
			Game::instance()->popSceneLater();
		}
		else if (evt.key.keysym.scancode == SDL_SCANCODE_RIGHT)
		{
			if (_items[_itemSelected]->volume())
			{
				_items[_itemSelected]->run(SDL_SCANCODE_RIGHT);
			}
		}
		else if (evt.key.keysym.scancode == SDL_SCANCODE_LEFT)
		{
			if (_items[_itemSelected]->volume())
			{
				_items[_itemSelected]->run(SDL_SCANCODE_LEFT);
			}
		}
		else if (evt.type == SDL_KEYDOWN && evt.key.keysym.scancode == SDL_SCANCODE_V && !evt.key.repeat)
			Game::instance()->toggleBorders();
	}
}

Menu* Menu::mainMenu()
{
	Menu* menu = new Menu({ 4.6f, 2.2f }, 5.5f, 0, false);

	new RenderableObject(menu, RectF(-2, 2 - 3, 16+4, 8 + 6), SpriteFactory::instance()->get("menu_background"), -1);
	new RenderableObject(menu, RectF(0, 0, 16, 12), SpriteFactory::instance()->get("menu_overlay"), -1);
	new RenderableObject(menu, RectF(-6, 5, 21, 15), SpriteFactory::instance()->get("menu_shading"), -1);

	menu->addItem("menu_new_run", 1.0f + 1, [](SDL_Scancode code)
	{
		Game::instance()->popSceneLater();
		Audio::instance()->playMusic("diptera sonata intro", false, 1);
		Audio::instance()->playMusic("diptera sonata(basement)",true);
	});
	menu->addItem("menu_options", 1.0f + 1, [menu](SDL_Scancode code)
	{
		Menu* nestedMenu = new Menu(menu);
		new RenderableObject(nestedMenu, RectF(-2, 2 - 3, 16 + 4, 8 + 6), SpriteFactory::instance()->get("menu_emptybg"), -1);
		new RenderableObject(nestedMenu, RectF(3.4f, -0.1f, 10.2f, 11.6f), SpriteFactory::instance()->get("menu_options_menu"), -1);
		new RenderableObject(nestedMenu, RectF(0, 0, 16, 12), SpriteFactory::instance()->get("menu_overlay"), -1);
		new RenderableObject(nestedMenu, RectF(-6, 5, 21, 15), SpriteFactory::instance()->get("menu_shading"), -1);

		nestedMenu->addItem("menu_options_sfx", 1.2f + 1, [nestedMenu](SDL_Scancode code)
			{
				if (nestedMenu->itemAt(0)->volume())
				{
					if (code == SDL_SCANCODE_RIGHT)
					{
						Audio::instance()->upVolumeSfx();
						Audio::instance()->playSound("plop");
					}
					else if (code == SDL_SCANCODE_LEFT)
					{
						Audio::instance()->downVolumeSfx();
						Audio::instance()->playSound("plop");
					}
					int volume = Audio::instance()->volumeSfx();
					std::string spriteName = "menu_options_" + std::to_string(volume);
					nestedMenu->itemAt(0)->volume()->setSprite(SpriteFactory::instance()->get(spriteName));
				}
			}, true);
		nestedMenu->addItem("menu_options_music", 1.2f + 1, [nestedMenu](SDL_Scancode code)
			{
				if (nestedMenu->itemAt(1)->volume())
				{
					if (code == SDL_SCANCODE_RIGHT)
						Audio::instance()->upVolumeMusic();
					else if (code == SDL_SCANCODE_LEFT)
						Audio::instance()->downVolumeMusic();

					int volume = Audio::instance()->volumeMusic();
					std::string spriteName = "menu_options_" + std::to_string(volume);
					nestedMenu->itemAt(1)->volume()->setSprite(SpriteFactory::instance()->get(spriteName));
				}
			}, true);

		nestedMenu->addItem("menu_options_vsync", 1.2f + 1, [nestedMenu](SDL_Scancode code)
			{
				SDL_RendererInfo info;
				SDL_GetRendererInfo(Game::instance()->window()->renderer(), &info);
				bool vsyncOn = info.flags & SDL_RENDERER_PRESENTVSYNC;
				SDL_RenderSetVSync(Game::instance()->window()->renderer(), !vsyncOn);
				if (vsyncOn)
				{
					nestedMenu->itemAt(2)->vsync1()->setSprite(SpriteFactory::instance()->get("menu_options_vsync_on_2"));
					nestedMenu->itemAt(2)->vsync2()->setSprite(SpriteFactory::instance()->get("menu_options_vsync_off_1"));
				}
				else
				{
					nestedMenu->itemAt(2)->vsync1()->setSprite(SpriteFactory::instance()->get("menu_options_vsync_on_1"));
					nestedMenu->itemAt(2)->vsync2()->setSprite(SpriteFactory::instance()->get("menu_options_vsync_off_2"));
				}
			}, false, true);
		Game::instance()->pushScene(nestedMenu);
	});
	menu->addItem("menu_exit", 1.2f + 1, [](SDL_Scancode code) {Game::instance()->quit(); });

	return menu;
}

Menu* Menu::pauseMenu()
{
	Menu* menu = new Menu({ 4.6f, 1.2f }, 5.5f, 0, false);

	menu->setBackgroundColor({ 0,0,0, 130});
	new RenderableObject(menu, RectF(3.4f, -0.1f, 10.2f, 11.6f), SpriteFactory::instance()->get("menu_options_menu_dark"), -1);

	menu->addItem("menu_options_resume", 1.0f + 1, [](SDL_Scancode code)
		{
			Game::instance()->popSceneLater();
		});
	menu->addItem("menu_options_reset", 1.0f + 1, [](SDL_Scancode code)
		{
			Game::instance()->reset();
		});

	menu->addItem("menu_options_sfx", 1.2f + 1, [menu](SDL_Scancode code)
		{
			if (menu->itemAt(2)->volume())
			{
				if (code == SDL_SCANCODE_RIGHT)
				{
					Audio::instance()->upVolumeSfx();
					Audio::instance()->playSound("plop");
				}
				else if (code == SDL_SCANCODE_LEFT)
				{
					Audio::instance()->downVolumeSfx();
					Audio::instance()->playSound("plop");
				}
				int volume = Audio::instance()->volumeSfx();
				std::string spriteName = "menu_options_" + std::to_string(volume);
				menu->itemAt(2)->volume()->setSprite(SpriteFactory::instance()->get(spriteName));
			}
		}, true);
	menu->addItem("menu_options_music", 1.2f + 1, [menu](SDL_Scancode code)
		{
			if (menu->itemAt(3)->volume())
			{
				if (code == SDL_SCANCODE_RIGHT)
					Audio::instance()->upVolumeMusic();
				else if (code == SDL_SCANCODE_LEFT)
					Audio::instance()->downVolumeMusic();

				int volume = Audio::instance()->volumeMusic();
				std::string spriteName = "menu_options_" + std::to_string(volume);
				menu->itemAt(3)->volume()->setSprite(SpriteFactory::instance()->get(spriteName));
			}
		}, true);


	menu->addItem("menu_options_vsync", 1.2f + 1, [menu](SDL_Scancode code)
		{
			SDL_RendererInfo info;
			SDL_GetRendererInfo(Game::instance()->window()->renderer(), &info);
			bool vsyncOn = info.flags & SDL_RENDERER_PRESENTVSYNC;
			SDL_RenderSetVSync(Game::instance()->window()->renderer(), !vsyncOn);
			if (vsyncOn)
			{
				menu->itemAt(4)->vsync1()->setSprite(SpriteFactory::instance()->get("menu_options_vsync_on_2"));
				menu->itemAt(4)->vsync2()->setSprite(SpriteFactory::instance()->get("menu_options_vsync_off_1"));
			}
			else
			{
				menu->itemAt(4)->vsync1()->setSprite(SpriteFactory::instance()->get("menu_options_vsync_on_1"));
				menu->itemAt(4)->vsync2()->setSprite(SpriteFactory::instance()->get("menu_options_vsync_off_2"));
			}
		}, false, true);

	menu->addItem("menu_options_exit", 1.4f + 1, [](SDL_Scancode code) {Game::instance()->quit(); });

	return menu;
}


Menu* Menu::startMenu()
{
	Menu* menu = new Menu({ 5.2f, 3.2f }, 3.5f, 0, true);

	new RenderableObject(menu, RectF(-2, 2 - 3, 16 + 4, 8 + 6), SpriteFactory::instance()->get("menu_title"), 1);
	new RenderableObject(menu, RectF(4.4f, 4.1f, 7, 7), SpriteFactory::instance()->get("menu_title_start"), 1);
	new RenderableObject(menu, RectF(-0.5, 0, 17, 5), SpriteFactory::instance()->get("menu_title_angel"),1);
	new RenderableObject(menu, RectF(0, 0, 16, 12), SpriteFactory::instance()->get("menu_overlay"), 1);
	new RenderableObject(menu, RectF(-6, 5, 21, 15), SpriteFactory::instance()->get("menu_shading"), 1);

	Audio::instance()->playMusic("title screen intro");

	menu->addItem(" ", 1.0f, [](SDL_Scancode code)
		{
			Audio::instance()->playSound("book page turn");
			Game::instance()->popSceneLater();
		});
	Game::instance()->pushScene(Menu::mainMenu());
	return menu;
}














