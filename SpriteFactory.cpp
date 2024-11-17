// ----------------------------------------------------------------
// From "Algorithms and Game Programming" in C++ by Alessandro Bria
// Copyright (C) 2024 Alessandro Bria (a.bria@unicas.it). 
// All rights reserved.
// 
// Released under the BSD License
// See LICENSE in root directory for full details.
// ----------------------------------------------------------------

#include "SpriteFactory.h"
#include "SDL_image.h"
#include "graphicsUtils.h"
#include "sdlUtils.h"
#include "Window.h"
#include "AnimatedSprite.h"
#include "TiledSprite.h"
#include "FilledSprite.h"
#include "Game.h"
#include <iostream>

using namespace agp;

SpriteFactory* SpriteFactory::instance()
{
	static SpriteFactory uniqueInstance;
	return &uniqueInstance;
}

SpriteFactory::SpriteFactory()
{
	if (IMG_Init(IMG_INIT_PNG) == 0)
	{
		SDL_Log("Unable to initialize SDL_image: %s", SDL_GetError());
		return;
	}

	SDL_Renderer* renderer = Game::instance()->window()->renderer();
	_spriteSheets["mario"] = loadTexture(renderer, "../sprites/mario.png", { 147, 187, 236 });
	_spriteSheets["enemies"] = loadTexture(renderer, "../sprites/enemies.png", { 147, 187, 236 });
	_spriteSheets["hud"] = loadTexture(renderer, "../sprites/hud.png", { 147, 187, 236 });
	_spriteSheets["tiles"] = loadTexture(renderer, "../sprites/stage_tiles.png", { 147, 187, 236 });

	// BASEMENT
	_spriteSheets["basement"] = loadTexture(renderer, "../sprites/basement.png", { 0, 114, 188 });
	_spriteSheets["door"] = loadTexture(renderer, "../sprites/basement_door.png", { 0, 255, 255 });
	_spriteSheets["shop"] = loadTexture(renderer, "../sprites/basement_shop.png", { 0, 114, 188 });
	_spriteSheets["treasure"] = loadTexture(renderer, "../sprites/basement_treasure.png", { 0, 114, 188 });
	_spriteSheets["rocks"] = loadTexture(renderer, "../sprites/basement_rocks.png", { 0, 114, 188 });
	_spriteSheets["fireplace_blue"] = loadTexture(renderer, "../sprites/basement_fireplace_blue.png", { 147, 187, 236 });
	_spriteSheets["bluefire"] = loadTexture(renderer, "../sprites/basement_bluefire.png", { 147, 187, 236 });
	_spriteSheets["controls"] = loadTexture(renderer, "../sprites/basement_controls.png", { 147, 187, 236 });
	_spriteSheets["shading"] = loadTexture(renderer, "../sprites/basement_shading.png", { 147, 187, 236 });

	// MENU
	_spriteSheets["menu_title"] = loadTexture(renderer, "../sprites/titlemenu.png", { 147, 187, 236 });
	_spriteSheets["menu"] = loadTexture(renderer, "../sprites/menu.png", { 147, 187, 236 });
	_spriteSheets["menu_emptybg"] = loadTexture(renderer, "../sprites/menu_emptybg.png", { 147, 187, 236 });
	_spriteSheets["menu_shading"] = loadTexture(renderer, "../sprites/menu_shading.png", { 147, 187, 236 });
	_spriteSheets["menu_overlay"] = loadTexture(renderer, "../sprites/menu_overlay.png", { 147, 187, 236 });
	_spriteSheets["menu_options"] = loadTexture(renderer, "../sprites/menu_options.png", { 147, 187, 236 });
	_spriteSheets["menu_options_dark"] = loadTexture(renderer, "../sprites/menu_options_dark.png", { 147, 187, 236 });

	// UI
	_spriteSheets["hud_hearts"] = loadTexture(renderer, "../sprites/ui_hearts.png", { 147, 187, 236 });
	_spriteSheets["hud_items"] = loadTexture(renderer, "../sprites/ui_items.png", { 147, 187, 236 });
	_spriteSheets["hud_minimap"] = loadTexture(renderer, "../sprites/ui_minimap.png", { 147, 187, 236 });

	//UI MONSTER
	_spriteSheets["ui_boss"] = loadTexture(renderer, "../sprites/ui_boss.png", { 147, 187, 236 });
	_spriteSheets["ui_bossname"] = loadTexture(renderer, "../sprites/ui_bossname_dukeofflies.png", { 147, 187, 236 });
	_spriteSheets["ui_bossspot"] = loadTexture(renderer, "../sprites/ui_bossspot.png", { 147, 187, 236 });
	_spriteSheets["ui_isaac"] = loadTexture(renderer, "../sprites/ui_isaac.png", { 147, 187, 236 });
	_spriteSheets["ui_isaacname"] = loadTexture(renderer, "../sprites/ui_isaac_name.png", { 147, 187, 236 });
	_spriteSheets["ui_vs"] = loadTexture(renderer, "../sprites/ui_vs.png", { 147, 187, 236 });

	std::vector<RectI> vecRect;
	SDL_Texture* base = loadTextureSequence(renderer, "../image", vecRect, Point(0, 0), Point(52, 52));
	RectI src(0, 0, 180, 180);
	SDL_RenderCopy(renderer, base, &src.toSDL(), &src.toSDL());

}

// anchors
static std::vector<RectI> basement_type{RectI(0, 0, 234, 156),
										RectI(234, 0, 234, 156),
										RectI(0, 156, 234, 156),
										RectI(234, 156, 234, 156)};

// ANDRANNO CANCELLATI
static RectI mario_small(1, 9, 16, 16);
static RectI hud_letter(519, 254, 8, 8);
static RectI hud_number(519, 263, 8, 8);
static RectI hud_letter_disabled(519, 366, 8, 8);
static RectI hud_number_disabled(519, 375, 8, 8);
static RectI hud_coin(519, 289, 8, 8);
static RectI flower(1, 12, 16, 16);
static RectI goomba(1, 28, 16, 16);
static RectI koopa(52, 12, 16, 32);


Sprite* SpriteFactory::get(const std::string& id)
{
	std::vector< RectI> rects;

	// BASEMENT
	if (id == "empty")
		return new FilledSprite(_spriteSheets["basement"], RectI(0, 312, 52, 52));
	else if (id == "door_normal")
		return new Sprite(_spriteSheets["door"], RectI(0,0,52,52));
	else if (id == "door_normal_back")
		return new Sprite(_spriteSheets["door"], RectI(0, 52, 52, 52));
	else if (id == "door_normal_panel_left")
		return new Sprite(_spriteSheets["door"], RectI(0, 104, 26, 52));
	else if (id == "door_normal_panel_right")
		return new Sprite(_spriteSheets["door"], RectI(26, 104, 26, 52));
	else if (id == "door_normal_panel_left_rotation")
		return new Sprite(_spriteSheets["door"], RectI(0, 182, 52, 26));
	else if (id == "door_normal_panel_right_rotation")
		return new Sprite(_spriteSheets["door"], RectI(0, 156, 52, 26));
	else if (id == "door_treasure")
		return new Sprite(_spriteSheets["door"], RectI(52, 0, 52, 52));
	else if (id == "door_boss")
		return new Sprite(_spriteSheets["door"], RectI(104, 0, 64, 52));
	else if (id == "door_boss_back")
		return new Sprite(_spriteSheets["door"], RectI(104, 52, 64, 52));
	else if (id == "door_boss_panel_left")
		return new Sprite(_spriteSheets["door"], RectI(104, 104, 32, 52));
	else if (id == "door_boss_panel_right")
		return new Sprite(_spriteSheets["door"], RectI(136, 104, 32, 52));
	else if (id == "door_boss_panel_left_rotation")
		return new Sprite(_spriteSheets["door"], RectI(104, 188, 52, 32));
	else if (id == "door_boss_panel_right_rotation")
		return new Sprite(_spriteSheets["door"], RectI(104, 156, 52, 32));
	else if (id == "door_boss_light")
		return new Sprite(_spriteSheets["door"], RectI(0, 208, 65, 38));
	else if (id == "controls")
		return new Sprite(_spriteSheets["controls"], RectI(0, 0, 325, 85));
	else if (id == "shading")
		return new Sprite(_spriteSheets["shading"], RectI(0, 0, 442, 286));
	else if (id == "fireplace_blue")
	{
		rects.push_back(moveBy(RectF(0, 0, 32, 32), 0, 0, 31, 32));
		rects.push_back(moveBy(RectF(0, 0, 32, 32), 1, 0, 31, 32));
		rects.push_back(moveBy(RectF(0, 0, 32, 32), 2, 0, 31, 32));
		return new AnimatedSprite(_spriteSheets["fireplace_blue"], rects, 13);
	}
	else if (id == "bluefire")
	{
		rects.push_back(moveBy(RectF(0, 0, 51, 51), 0, 0, 47, 51));
		rects.push_back(moveBy(RectF(0, 0, 51, 51), 1, 0, 47, 51));
		rects.push_back(moveBy(RectF(0, 0, 51, 51), 2, 0, 47, 51));
		rects.push_back(moveBy(RectF(0, 0, 51, 51), 3, 0, 47, 51));
		rects.push_back(moveBy(RectF(0, 0, 51, 51), 4, 0, 47, 51));
		rects.push_back(moveBy(RectF(0, 0, 51, 51), 0, 1, 47, 51));
		return new AnimatedSprite(_spriteSheets["bluefire"], rects, 13);
	}
	else if (id == "rock") 
	{
		int x = rand() % 3;
		//int y = rand() % 10 < 6 ? 0 : rand() % 3;
		int srcPosx = 32 * x;
		//int srcPosy = 32 * y;
		return new Sprite(_spriteSheets["rocks"], RectI(srcPosx, 0, 32, 32));
	}		
	else if (id == "basement_UpL" || id == "basement_UpR" || id == "basement_DownL" || id == "basement_DownR")
	{
		int size = 52;
		for (int j = 0; j < 3; j++)
		{
			for (int i = 0; i < 4; i++)
			{
				int _type = rand() % 10 < 6 ? 0 : rand() % 3;
				int srcPosx = 234 * (_type % 2);
				int srcPosy = 156 * (_type / 2);
				if (id == "basement_UpL")
				{
					rects.push_back(RectI(srcPosx + i * size, srcPosy + j * size, size, size));
				}
				else if (id == "basement_UpR")
				{
					rects.push_back(RectI(srcPosx + ( 3 - i) * size, srcPosy + j * size, size, size));
				}
				else if (id == "basement_DownL")
				{
					rects.push_back(RectI(srcPosx + i * size, srcPosy + (2 - j) * size - 1 +0.1, size, size));
				}
				else if (id == "basement_DownR")
				{
					rects.push_back(RectI(srcPosx + (3 - i) * size, srcPosy + (2 - j) * size, size, size));
				}
			}
		}
		return new TiledSprite(_spriteSheets["basement"], rects, { 2,2 });	
	}
	else if (id == "basement_boss_UpL" || id == "basement_boss_UpR" || id == "basement_boss_DownL" || id == "basement_boss_DownR")
	{
		int size = 52;
		for (int j = 0; j < 3; j++)
		{
			for (int i = 0; i < 4; i++)
			{
				int srcPosx = 234;
				int srcPosy = 156;
				if (id == "basement_boss_UpL")
				{
					rects.push_back(RectI(srcPosx + i * size, srcPosy + j * size, size, size));
				}
				else if (id == "basement_boss_UpR")
				{
					rects.push_back(RectI(srcPosx + (3 - i) * size, srcPosy + j * size, size, size));
				}
				else if (id == "basement_boss_DownL")
				{
					rects.push_back(RectI(srcPosx + i * size, srcPosy + (2 - j) * size, size, size));
				}
				else if (id == "basement_boss_DownR")
				{
					rects.push_back(RectI(srcPosx + (3 - i) * size, srcPosy + (2 - j) * size, size, size));
				}
			}
		}
		return new TiledSprite(_spriteSheets["basement"], rects, { 2,2 });
	}
	else if (id == "basement_shop_UpL" || id == "basement_shop_UpR" || id == "basement_shop_DownL" || id == "basement_shop_DownR")
	{
		int size = 52;
		for (int j = 0; j < 3; j++)
		{
			for (int i = 0; i < 4; i++)
			{
				int _type = rand() % 10 < 6 ? 0 : rand() % 4;
				int srcPosx = 234 * (_type % 2);
				int srcPosy = 156 * (_type / 2);
				if (id == "basement_shop_UpL")
				{
					rects.push_back(RectI(srcPosx + i * size, srcPosy + j * size, size, size));
				}
				else if (id == "basement_shop_UpR")
				{
					rects.push_back(RectI(srcPosx + ( 3 - i) * size, srcPosy + j * size, size, size));
				}
				else if (id == "basement_shop_DownL")
				{
					rects.push_back(RectI(srcPosx + i * size, srcPosy + (2 - j) * size - 1 +0.1, size, size));
				}
				else if (id == "basement_shop_DownR")
				{
					rects.push_back(RectI(srcPosx + (3 - i) * size, srcPosy + (2 - j) * size, size, size));
				}
			}
		}
		return new TiledSprite(_spriteSheets["shop"], rects, { 2,2 });	
	}
	else if (id == "basement_treasure_UpL" || id == "basement_treasure_UpR" || id == "basement_treasure_DownL" || id == "basement_treasure_DownR")
	{
		int size = 52;
		for (int j = 0; j < 3; j++)
		{
			for (int i = 0; i < 4; i++)
			{
				int _type = rand() % 10 < 6 ? 0 : rand() % 4;
				int srcPosx = 234 * (_type % 2);
				int srcPosy = 156 * (_type / 2);
				if (id == "basement_treasure_UpL")
				{
					rects.push_back(RectI(srcPosx + i * size, srcPosy + j * size, size, size));
				}
				else if (id == "basement_treasure_UpR")
				{
					rects.push_back(RectI(srcPosx + (3 - i) * size, srcPosy + j * size, size, size));
				}
				else if (id == "basement_treasure_DownL")
				{
					rects.push_back(RectI(srcPosx + i * size, srcPosy + (2 - j) * size - 1 + 0.1, size, size));
				}
				else if (id == "basement_treasure_DownR")
				{
					rects.push_back(RectI(srcPosx + (3 - i) * size, srcPosy + (2 - j) * size, size, size));
				}
			}
		}
		return new TiledSprite(_spriteSheets["treasure"], rects, { 2,2 });
		}
	
	// UI_MONSTER
	else if (id == "ui_boss")
		return new Sprite(_spriteSheets["ui_boss"], RectI(0, 0, 192, 192));
	else if (id == "ui_bossname")
		return new Sprite(_spriteSheets["ui_bossname"], RectI(0, 0, 174, 55));
	else if (id == "ui_bossspot")
		return new Sprite(_spriteSheets["ui_bossspot"], RectI(0, 0, 260, 70));
	else if (id == "ui_isaac")
		return new Sprite(_spriteSheets["ui_isaac"], RectI(0, 0, 112, 78));
	else if (id == "ui_isaacname")
		return new Sprite(_spriteSheets["ui_isaacname"], RectI(0, 0, 192, 64));
	else if (id == "ui_vs")
		return new Sprite(_spriteSheets["ui_vs"], RectI(0, 0, 128, 64));

	// SPRITES HUD
	else if (id =="hud_heart_red")
		return new Sprite(_spriteSheets["hud_hearts"], RectI(0, 0, 16, 16));
	else if (id == "hud_heart_half_red")
		return new Sprite(_spriteSheets["hud_hearts"], RectI(16, 0, 16, 16));
	else if (id == "hud_heart_empty")
		return new Sprite(_spriteSheets["hud_hearts"], RectI(32, 0, 16, 16));
	else if (id == "hud_coin")
		return new Sprite(_spriteSheets["hud_items"], RectI(0, 0, 16, 16));
	else if (id == "hud_bomb")
		return new Sprite(_spriteSheets["hud_items"], RectI(0, 16, 16, 16));
	else if (id == "hud_minimap_back")
		return new Sprite(_spriteSheets["hud_minimap"], RectI(0, 0, 55, 49));
	else if (id == "hud_minimap_room_select")
		return new Sprite(_spriteSheets["hud_minimap"], RectI(114, 82, 9, 8));
	else if (id == "hud_minimap_room")
		return new Sprite(_spriteSheets["hud_minimap"], RectI(66, 18, 9, 8));
	else if (id == "hud_minimap_boss")
		return new Sprite(_spriteSheets["hud_minimap"], RectI(5, 51, 9, 8));
	else if (id == "hud_minimap_treasure")
		return new Sprite(_spriteSheets["hud_minimap"], RectI(18, 52, 8, 7));
	else if (id == "hud_minimap_shop")
		return new Sprite(_spriteSheets["hud_minimap"], RectI(30, 51, 9, 8));

	//SPRITES MENU
	else if (id == "menu_title")
		return new Sprite(_spriteSheets["menu_title"], RectI(0, 0, 480, 272));
	else if (id == "menu_title_angel")
		return new Sprite(_spriteSheets["menu_title"], RectI(0, 272, 479, 105));
	else if (id == "menu_title_start")
	{
		rects.push_back(RectF(9,382,160,158));
		rects.push_back(RectF(169, 382, 160, 158));
		return new AnimatedSprite(_spriteSheets["menu_title"], rects, 10);
	}
	else if (id == "menu_background")
		return new Sprite(_spriteSheets["menu"], RectI(0, 0, 480, 269));
	else if (id == "menu_emptybg")
		return new Sprite(_spriteSheets["menu_emptybg"], RectI(0, 0, 480, 270));
	else if (id == "menu_shading")
		return new Sprite(_spriteSheets["menu_shading"], RectI(0, 0, 256, 150));
	else if (id == "menu_overlay")
		return new Sprite(_spriteSheets["menu_overlay"], RectI(0, 0, 480, 270));
	else if (id == "menu_new_run")
		return new Sprite(_spriteSheets["menu"], RectI(32, 295, 121, 46));
	else if (id == "menu_continue")
		return new Sprite(_spriteSheets["menu"], RectI(0, 0, 480, 269));
	else if (id == "menu_options")
		return new Sprite(_spriteSheets["menu"], RectI(32, 479, 121, 46));
	else if (id == "menu_exit")
		return new Sprite(_spriteSheets["menu"], RectI(32, 434, 121, 46));
	else if (id == "menu_arrow")
		return new Sprite(_spriteSheets["menu"], RectI(1, 296, 31, 44));
	else if (id == "menu_options_menu")
		return new Sprite(_spriteSheets["menu_options"], RectI(0, 0, 234, 234));
	else if (id == "menu_options_menu_dark")
		return new Sprite(_spriteSheets["menu_options_dark"], RectI(0, 0, 234, 234));
	else if (id == "menu_options_sfx")
		return new Sprite(_spriteSheets["menu_options"], RectI(0, 246, 121, 46));
	else if (id == "menu_options_music")
		return new Sprite(_spriteSheets["menu_options"], RectI(0, 522, 121, 46));
	else if (id == "menu_options_reset")
		return new Sprite(_spriteSheets["menu_options"], RectI(0, 568, 121, 46));
	else if (id == "menu_options_resume")
		return new Sprite(_spriteSheets["menu_options"], RectI(0, 614, 121, 46));
	else if (id == "menu_options_exit")
		return new Sprite(_spriteSheets["menu_options"], RectI(0, 660, 121, 46));
	else if (id == "menu_options_10")
		return new Sprite(_spriteSheets["menu_options"], RectI(121, 246, 121, 46));
	else if (id == "menu_options_9")
		return new Sprite(_spriteSheets["menu_options"], RectI(121, 292, 121, 46));
	else if (id == "menu_options_8")
		return new Sprite(_spriteSheets["menu_options"], RectI(121, 338, 121, 46));
	else if (id == "menu_options_7")
		return new Sprite(_spriteSheets["menu_options"], RectI(121, 384, 121, 46));
	else if (id == "menu_options_6")
		return new Sprite(_spriteSheets["menu_options"], RectI(121, 430, 121, 46));
	else if (id == "menu_options_5")
		return new Sprite(_spriteSheets["menu_options"], RectI(121, 476, 121, 46));
	else if (id == "menu_options_4")
		return new Sprite(_spriteSheets["menu_options"], RectI(121, 522, 121, 46));
	else if (id == "menu_options_3")
		return new Sprite(_spriteSheets["menu_options"], RectI(121, 568, 121, 46));
	else if (id == "menu_options_2")
		return new Sprite(_spriteSheets["menu_options"], RectI(121, 614, 121, 46));
	else if (id == "menu_options_1")
		return new Sprite(_spriteSheets["menu_options"], RectI(121, 660, 121, 46));
	else if (id == "menu_options_0")
		return new Sprite(_spriteSheets["menu_options"], RectI(121, 706, 121, 46));
	else if (id == "menu_options_vsync")
		return new Sprite(_spriteSheets["menu_options"], RectI(0, 292, 121, 46));
	else if (id == "menu_options_vsync_on_1")
		return new Sprite(_spriteSheets["menu_options"], RectI(0, 338, 121, 46));
	else if (id == "menu_options_vsync_on_2")
		return new Sprite(_spriteSheets["menu_options"], RectI(0, 384, 121, 46));
	else if (id == "menu_options_vsync_off_1")
		return new Sprite(_spriteSheets["menu_options"], RectI(0, 430, 121, 46));
	else if (id == "menu_options_vsync_off_2")
		return new Sprite(_spriteSheets["menu_options"], RectI(0, 476, 121, 46));

	// ANDRANNO CANCELLATE
	else if (id == "wall")
		return new Sprite(_spriteSheets["tiles"], moveBy(flower, 2, 5));
	else if (id == "block")
		return new Sprite(_spriteSheets["tiles"], moveBy(flower, 5, 3));
	else if (id == "welcome")
		return new Sprite(_spriteSheets["hud"], RectI(1, 2 + 16 * 2, 16 * 16, 13 * 16));
	else if (id == "gameover")
		return new Sprite(_spriteSheets["hud"], RectI(260, 253, 16 * 16, 15 * 16));
	else if (id == "mario_stand")
		return new Sprite(_spriteSheets["mario"], mario_small);
	else if (id == "mario_jump")
		return new Sprite(_spriteSheets["mario"], moveBy(mario_small, 6, 0));
	else if (id == "mario_skid")
		return new Sprite(_spriteSheets["mario"], moveBy(mario_small, 5, 0));
	else if (id == "mario_die")
		return new Sprite(_spriteSheets["mario"], moveBy(mario_small, 1, 0));
	else if (id == "hammer")
		return new Sprite(_spriteSheets["enemies"], moveBy(goomba, 25, 0));
	else if (id == "hammer_brother_jump")
		return new Sprite(_spriteSheets["enemies"], moveBy(koopa, 19, 0));
	else if (id == "platform")
		return new FilledSprite(_spriteSheets["tiles"], RectI(120, 148, 8, 8), { 0.5f, 0.5f });

	else if (id == "mario_walk")
	{
		rects.push_back(moveBy(mario_small, 2, 0));
		rects.push_back(moveBy(mario_small, 3, 0));
		rects.push_back(moveBy(mario_small, 4, 0));
		return new AnimatedSprite(_spriteSheets["mario"], rects, 10);
		}
	else if (id == "mario_run")
	{
		rects.push_back(moveBy(mario_small, 2, 0));
		rects.push_back(moveBy(mario_small, 3, 0));
		rects.push_back(moveBy(mario_small, 4, 0));
		return new AnimatedSprite(_spriteSheets["mario"], rects, 20);
		}
	else if (id == "box")
	{
		rects.push_back(moveBy(flower, 0, 3));
		rects.push_back(moveBy(flower, 1, 3));
		rects.push_back(moveBy(flower, 2, 3));
		rects.push_back(moveBy(flower, 1, 3));
		rects.push_back(moveBy(flower, 0, 3));
		return new AnimatedSprite(_spriteSheets["tiles"], rects, 5);
		}

	else
	{
		std::cerr << "Cannot find sprite \"" << id << "\"\n";
		return nullptr;
	}
}

Sprite* SpriteFactory::getText(std::string text, const Vec2Df& size, int fillN, char fillChar, bool enabled)
{
	std::vector< RectI> tiles;

	if (fillN)
		while (text.size() != fillN)
			text = fillChar + text;

	RectI& number_anchor = enabled ? hud_number : hud_number_disabled;
	RectI& letter_anchor = enabled ? hud_letter : hud_letter_disabled;

	for (auto& c : text)
	{
		if(isdigit(c))
			tiles.push_back(moveBy(number_anchor, c - '0', 0, 8, 8));
		else if (isalpha(c))
			tiles.push_back(moveBy(letter_anchor, toupper(c) - 'A', 0, 8, 8));
		else if (c == '-')
			tiles.push_back(moveBy(number_anchor, 10, 0, 8, 8));
		else if (c == '*')
			tiles.push_back(moveBy(number_anchor, 11, 0, 8, 8));
		else if (c == '!')
			tiles.push_back(moveBy(number_anchor, 12, 0, 8, 8));
		else if (c == '©')
			tiles.push_back(moveBy(number_anchor, 13, 0, 8, 8));
		else if (c == '.')
			tiles.push_back(moveBy(number_anchor, 14, 0, 8, 8));
		else
			tiles.push_back(moveBy(hud_letter, 0, -5, 8, 8));	// empty space
	}

	return new TiledSprite(_spriteSheets["hud"], tiles, size);
}