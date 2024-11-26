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
	// ISAAC
	_spriteSheets["isaac"] = loadTexture(renderer, "../sprites/isaac.png");
	// ITEM
	_spriteSheets["tears"] = loadTexture(renderer, "../sprites/tears.png");
	_spriteSheets["explosion"] = loadTexture(renderer, "../sprites/tear_animation.png");
	_spriteSheets["altar"] = loadTexture(renderer, "../sprites/altar.png");
	_spriteSheets["item_coin"] = loadTexture(renderer, "../sprites/item_coin.png", { 147, 187, 236 });
	_spriteSheets["item_coin_effect"] = loadTexture(renderer, "../sprites/item_coin.png", { 147, 187, 236 });

	_spriteSheets["enemies"] = loadTexture(renderer, "../sprites/enemies.png", { 147, 187, 236 });
	_spriteSheets["hud"] = loadTexture(renderer, "../sprites/hud.png", { 147, 187, 236 });
	_spriteSheets["tiles"] = loadTexture(renderer, "../sprites/stage_tiles.png", { 147, 187, 236 });

	// BASEMENT
	_spriteSheets["basement"] = loadTexture(renderer, "../sprites/basement.png", { 0, 114, 188 });
	_spriteSheets["door"] = loadTexture(renderer, "../sprites/basement_door.png", { 0, 255, 255 });
	_spriteSheets["shop"] = loadTexture(renderer, "../sprites/basement_shop.png", { 0, 114, 188 });
	_spriteSheets["treasure"] = loadTexture(renderer, "../sprites/basement_treasure.png", { 0, 114, 188 });
	_spriteSheets["rocks"] = loadTexture(renderer, "../sprites/basement_rocks.png", { 0, 114, 188 });
	_spriteSheets["poop"] = loadTexture(renderer, "../sprites/basement_poop.png", { 0, 114, 188 });
	_spriteSheets["poop_explosion"] = loadTexture(renderer, "../sprites/basement_poop_explosion.png", { 0, 114, 188 });
	_spriteSheets["poop_gibs"] = loadTexture(renderer, "../sprites/basement_poop_gibs.png", { 0, 114, 188 });
	_spriteSheets["fireplace_blue"] = loadTexture(renderer, "../sprites/basement_fireplace_blue.png", { 147, 187, 236 });
	_spriteSheets["bluefire"] = loadTexture(renderer, "../sprites/basement_bluefire.png", { 147, 187, 236 });
	_spriteSheets["controls"] = loadTexture(renderer, "../sprites/basement_controls.png", { 147, 187, 236 });
	_spriteSheets["shading"] = loadTexture(renderer, "../sprites/basement_shading.png", { 147, 187, 236 });
	_spriteSheets["props"] = loadTexture(renderer, "../sprites/basement_props.png", { 147, 187, 236 });
	_spriteSheets["overlay"] = loadTexture(renderer, "../sprites/basement_overlay.png", { 147, 187, 236 });

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
	_spriteSheets["hud_minimap"] = loadTexture(renderer, "../sprites/ui_minimap.png", { 147, 187, 236 });


	//UI MONSTER
	_spriteSheets["ui_boss"] = loadTexture(renderer, "../sprites/ui_boss.png", { 147, 187, 236 });
	_spriteSheets["ui_bossname"] = loadTexture(renderer, "../sprites/ui_bossname_dukeofflies.png", { 147, 187, 236 });
	_spriteSheets["ui_bossspot"] = loadTexture(renderer, "../sprites/ui_bossspot.png", { 147, 187, 236 });
	_spriteSheets["ui_isaac"] = loadTexture(renderer, "../sprites/ui_isaac.png", { 147, 187, 236 });
	_spriteSheets["ui_isaacname"] = loadTexture(renderer, "../sprites/ui_isaac_name.png", { 147, 187, 236 });
	_spriteSheets["ui_vs"] = loadTexture(renderer, "../sprites/ui_vs.png", { 147, 187, 236 });
	_spriteSheets["ui_font"] = loadTexture(renderer, "../sprites/ui_font.png", { 147, 187, 236 });



	// MOBS
	_spriteSheets["duke"] = loadTexture(renderer, "../sprites/duke.png", { 147, 187, 236 });
	_spriteSheets["fly"] = loadTexture(renderer, "../sprites/fly.png", { 147, 187, 236 });

	//BLOOD EXPLOTION
	_spriteSheets["bloodExplotion"] = loadTexture(renderer, "../sprites/bloodExplotion.png", { 147, 187, 236 });

	// SHADOW______da fixare trasparenza
	_spriteSheets["shadow"] = loadTexture(renderer, "../sprites/shadow.png", { 147, 187, 236});

	// POOF EFFECT
	_spriteSheets["poof"] = loadTexture(renderer, "../sprites/poof.png", { 147, 187, 236 });

	std::vector<RectI> vecRect;
	SDL_Texture* base = loadTextureSequence(renderer, "../image", vecRect, Point(0, 0), Point(52, 52));
	RectI src(0, 0, 180, 180);
	SDL_RenderCopy(renderer, base, &src.toSDL(), &src.toSDL());

}

// anchors
static std::vector<RectI> basement_type{ RectI(0, 0, 234, 156),
										RectI(234, 0, 234, 156),
										RectI(0, 156, 234, 156),
										RectI(234, 156, 234, 156) };


Sprite* SpriteFactory::get(const std::string& id)
{
	std::vector< RectI> rects;

	// BASEMENT
	if (id == "wall")
		return new FilledSprite(_spriteSheets["basement"], RectI(0, 312, 52, 52), "wall");
	else if (id == "upWall")
		return new FilledSprite(_spriteSheets["basement"], RectI(0, 312, 52, 52), "upWall");
	else if (id == "door_normal")
		return new Sprite(_spriteSheets["door"], RectI(0, 0, 52, 52), "door_normal");
	else if (id == "door_normal_back")
		return new Sprite(_spriteSheets["door"], RectI(0, 52, 52, 52), "door_normal_back");
	else if (id == "door_normal_panel_left")
		return new Sprite(_spriteSheets["door"], RectI(0, 104, 26, 52), "door_normal_panel_left");
	else if (id == "door_normal_panel_right")
		return new Sprite(_spriteSheets["door"], RectI(26, 104, 26, 52), "door_normal_panel_right");
	else if (id == "door_normal_panel_left_rotation")
		return new Sprite(_spriteSheets["door"], RectI(0, 182, 52, 26), "door_normal_panel_left_rotation");
	else if (id == "door_normal_panel_right_rotation")
		return new Sprite(_spriteSheets["door"], RectI(0, 156, 52, 26), "door_normal_panel_right_rotation");
	else if (id == "door_treasure")
		return new Sprite(_spriteSheets["door"], RectI(52, 0, 52, 52), "door_treasure");
	else if (id == "door_boss")
		return new Sprite(_spriteSheets["door"], RectI(104, 0, 64, 52), "door_boss");
	else if (id == "door_boss_back")
		return new Sprite(_spriteSheets["door"], RectI(104, 52, 64, 52), "door_boss_back");
	else if (id == "door_boss_panel_left")
		return new Sprite(_spriteSheets["door"], RectI(104, 104, 32, 52), "door_boss_panel_left");
	else if (id == "door_boss_panel_right")
		return new Sprite(_spriteSheets["door"], RectI(136, 104, 32, 52), "door_boss_panel_right");
	else if (id == "door_boss_panel_left_rotation")
		return new Sprite(_spriteSheets["door"], RectI(104, 188, 52, 32), "door_boss_panel_left_rotation");
	else if (id == "door_boss_panel_right_rotation")
		return new Sprite(_spriteSheets["door"], RectI(104, 156, 52, 32), "door_boss_panel_right_rotation");
	else if (id == "door_boss_light")
		return new Sprite(_spriteSheets["door"], RectI(0, 208, 65, 38), "door_boss_light");
	else if (id == "controls")
		return new Sprite(_spriteSheets["controls"], RectI(0, 0, 325, 85), "controls");
	else if (id == "shading")
		return new Sprite(_spriteSheets["shading"], RectI(0, 0, 442, 286), "shading");
	else if (id == "fireplace_blue")
	{
		rects.push_back(moveBy(RectF(0, 0, 32, 32), 0, 0, 31, 32));
		rects.push_back(moveBy(RectF(0, 0, 32, 32), 1, 0, 31, 32));
		rects.push_back(moveBy(RectF(0, 0, 32, 32), 2, 0, 31, 32));
		return new AnimatedSprite(_spriteSheets["fireplace_blue"], rects, 13, "fireplace_blue");
	}
	else if (id == "bluefire")
	{
		rects.push_back(moveBy(RectF(0, 0, 51, 51), 0, 0, 47, 51));
		rects.push_back(moveBy(RectF(0, 0, 51, 51), 1, 0, 47, 51));
		rects.push_back(moveBy(RectF(0, 0, 51, 51), 2, 0, 47, 51));
		rects.push_back(moveBy(RectF(0, 0, 51, 51), 3, 0, 47, 51));
		rects.push_back(moveBy(RectF(0, 0, 51, 51), 4, 0, 47, 51));
		rects.push_back(moveBy(RectF(0, 0, 51, 51), 0, 1, 47, 51));
		return new AnimatedSprite(_spriteSheets["bluefire"], rects, 13, "bluefire");
	}
	else if (id == "rock")
	{
		int x = rand() % 3;
		//int y = rand() % 10 < 6 ? 0 : rand() % 3;
		int srcPosx = 32 * x;
		//int srcPosy = 32 * y;
		return new Sprite(_spriteSheets["rocks"], RectF(float(srcPosx), 0, 32, 32), "rock");
	}
	else if (id == "poop")
		return new Sprite(_spriteSheets["poop"], RectF(0, 0, 32, 32), "poop");
	else if (id == "poop_1")
		return new Sprite(_spriteSheets["poop"], RectF(1*32, 0, 32, 32), "poop");
	else if (id == "poop_2")
		return new Sprite(_spriteSheets["poop"], RectF(2*32, 0, 32, 32), "poop");
	else if (id == "poop_3")
		return new Sprite(_spriteSheets["poop"], RectF(3 * 32, 0, 32, 32), "poop");
	else if (id == "poop_4")
		return new Sprite(_spriteSheets["poop"], RectF(4 * 32, 0, 32, 32), "poop");
	else if (id == "poop_gold")
		return new Sprite(_spriteSheets["poop"], RectF(0, 3 * 32, 32, 32), "poop");
	else if (id == "poop_gold_1")
		return new Sprite(_spriteSheets["poop"], RectF(1 * 32, 3*32, 32, 32), "poop");
	else if (id == "poop_gold_2")
		return new Sprite(_spriteSheets["poop"], RectF(2 * 32, 3 * 32, 32, 32), "poop");
	else if (id == "poop_gold_3")
		return new Sprite(_spriteSheets["poop"], RectF(3 * 32, 3 * 32, 32, 32), "poop");
	else if (id == "poop_gold_4")
		return new Sprite(_spriteSheets["poop"], RectF(4 * 32, 3 * 32, 32, 32), "poop");
	else if (id == "poop_explosion")
	{
		rects.push_back(moveBy(RectI(0, 0, 64, 64), 0, 0, 64, 64,0,0));
		rects.push_back(moveBy(RectI(0, 0, 64, 64), 1, 0, 64, 64,0,0));
		rects.push_back(moveBy(RectI(0, 0, 64, 64), 2, 0, 64, 64,0,0));
		rects.push_back(moveBy(RectI(0, 0, 64, 64), 3, 0, 64, 64,0,0));
		rects.push_back(moveBy(RectI(0, 0, 64, 64), 0, 1, 64, 64,0,0));
		//rects.push_back(moveBy(RectI(0, 0, 64, 64), 1, 1, 64, 64,0,0));
		//rects.push_back(moveBy(RectI(0, 0, 64, 64), 2, 1, 64, 64, 0, 0));
		//rects.push_back(moveBy(RectI(0, 0, 64, 64), 3, 1, 64, 64, 0, 0));
		//rects.push_back(moveBy(RectI(0, 0, 64, 64), 0, 2, 64, 64, 0, 0));
		//rects.push_back(moveBy(RectI(0, 0, 64, 64), 1, 2, 64, 64, 0, 0));
		return new AnimatedSprite(_spriteSheets["poop_explosion"], rects, 18, "poop_explosion", false);
	}
	else if (id == "poop_gibs")
	{
		int cases = rand() % 5;
		if(cases == 0)
			return new Sprite(_spriteSheets["poop_gibs"], RectF(0 * 16, 0 * 16, 16, 16), "poop_gibs");
		else if (cases == 1)
			return new Sprite(_spriteSheets["poop_gibs"], RectF(1 * 16, 0 * 16, 16, 16), "poop_gibs");
		else if (cases == 2)
			return new Sprite(_spriteSheets["poop_gibs"], RectF(2 * 16, 0 * 16, 16, 16), "poop_gibs");
		else if (cases == 3)
			return new Sprite(_spriteSheets["poop_gibs"], RectF(0 * 16, 1 * 16, 16, 16), "poop_gibs");
		else if (cases == 4)
			return new Sprite(_spriteSheets["poop_gibs"], RectF(1 * 16, 1 * 16, 16, 16), "poop_gibs");
	}


	else if (id == "basement_UpL" || id == "basement_UpR" || id == "basement_DownL" || id == "basement_DownR")
	{
		float size = 52;
		for (int j = 0; j < 3; j++)
		{
			for (int i = 0; i < 4; i++)
			{
				int _type = rand() % 10 < 6 ? 0 : rand() % 3;
				float srcPosx = 234 * float(_type % 2);
				float srcPosy = 156 * float(_type / 2);
				if (id == "basement_UpL")
				{
					rects.push_back(RectF(srcPosx + i * size, srcPosy + j * size, size, size));
				}
				else if (id == "basement_UpR")
				{
					rects.push_back(RectF(srcPosx + (3 - i) * size, srcPosy + j * size, size, size));
				}
				else if (id == "basement_DownL")
				{
					rects.push_back(RectF(srcPosx + i * size, srcPosy + (2 - j) * size - 1 + 0.1f, size, size));
				}
				else if (id == "basement_DownR")
				{
					rects.push_back(RectF(srcPosx + (3 - i) * size, srcPosy + (2 - j) * size, size, size));
				}
			}
		}
		return new TiledSprite(_spriteSheets["basement"], rects, "basement_wall", { 2,2 });
	}
	else if (id == "basement_boss_UpL" || id == "basement_boss_UpR" || id == "basement_boss_DownL" || id == "basement_boss_DownR")
	{
		float size = 52;
		for (int j = 0; j < 3; j++)
		{
			for (int i = 0; i < 4; i++)
			{
				float srcPosx = 234;
				float srcPosy = 156;
				if (id == "basement_boss_UpL")
				{
					rects.push_back(RectF(srcPosx + i * size, srcPosy + j * size, size, size));
				}
				else if (id == "basement_boss_UpR")
				{
					rects.push_back(RectF(srcPosx + (3 - i) * size, srcPosy + j * size, size, size));
				}
				else if (id == "basement_boss_DownL")
				{
					rects.push_back(RectF(srcPosx + i * size, srcPosy + (2 - j) * size, size, size));
				}
				else if (id == "basement_boss_DownR")
				{
					rects.push_back(RectF(srcPosx + (3 - i) * size, srcPosy + (2 - j) * size, size, size));
				}
			}
		}
		return new TiledSprite(_spriteSheets["basement"], rects, "basement_wall_boss", { 2,2 });
	}
	else if (id == "basement_shop_UpL" || id == "basement_shop_UpR" || id == "basement_shop_DownL" || id == "basement_shop_DownR")
	{
		float size = 52;
		for (int j = 0; j < 3; j++)
		{
			for (int i = 0; i < 4; i++)
			{
				int _type = rand() % 10 < 6 ? 0 : rand() % 4;
				float srcPosx = 234 * float(_type % 2);
				float srcPosy = 156 * float(_type / 2);
				if (id == "basement_shop_UpL")
				{
					rects.push_back(RectF(srcPosx + i * size, srcPosy + j * size, size, size));
				}
				else if (id == "basement_shop_UpR")
				{
					rects.push_back(RectF(srcPosx + (3 - i) * size, srcPosy + j * size, size, size));
				}
				else if (id == "basement_shop_DownL")
				{
					rects.push_back(RectF(srcPosx + i * size, srcPosy + (2 - j) * size - 1 + 0.1f, size, size));
				}
				else if (id == "basement_shop_DownR")
				{
					rects.push_back(RectF(srcPosx + (3 - i) * size, srcPosy + (2 - j) * size, size, size));
				}
			}
		}
		return new TiledSprite(_spriteSheets["shop"], rects, "basement_wall_shop", { 2,2 });
	}
	else if (id == "basement_treasure_UpL" || id == "basement_treasure_UpR" || id == "basement_treasure_DownL" || id == "basement_treasure_DownR")
	{
		float size = 52;
		for (int j = 0; j < 3; j++)
		{
			for (int i = 0; i < 4; i++)
			{
				int _type = rand() % 10 < 6 ? 0 : rand() % 4;
				float srcPosx = 234 * float(_type % 2);
				float srcPosy = 156 * float(_type / 2);
				if (id == "basement_treasure_UpL")
				{
					rects.push_back(RectF(srcPosx + i * size, srcPosy + j * size, size, size));
				}
				else if (id == "basement_treasure_UpR")
				{
					rects.push_back(RectF(srcPosx + (3 - i) * size, srcPosy + j * size, size, size));
				}
				else if (id == "basement_treasure_DownL")
				{
					rects.push_back(RectF(srcPosx + i * size, srcPosy + (2 - j) * size - 1 + 0.1f, size, size));
				}
				else if (id == "basement_treasure_DownR")
				{
					rects.push_back(RectF(srcPosx + (3 - i) * size, srcPosy + (2 - j) * size, size, size));
				}
			}
		}
		return new TiledSprite(_spriteSheets["treasure"], rects, "basement_wall_treasure", { 2,2 });
	}
	else if (id == "basement_props")
	{
		int index = rand() % 20;
		for (int i = 0; i < 2; i++)
		{
			for (int j = 0; j < 10; j++)
			{
				if ((i * 10 + j) == index)
				{
					return new Sprite(_spriteSheets["props"], RectI(j * 27, i * 27, 27, 27), "basement_props");
				}
			}
		}
	}
	else if (id == "basement_overlay")
	{
		int index = rand() % 13;
		int overlay[13] = { 3,5,8,10,11,12,13,14,15,18,19,20,21 };
		for (int i = 0; i < 4; i++)
		{
			for (int j = 0; j < 6; j++)
			{
				if ((i * 6 + j) == overlay[index])
				{
					std::cout << index << std::endl;
					return new Sprite(_spriteSheets["overlay"], RectI(j * 331, i * 215, 331, 215), "basement_overlay", SDL_FLIP_NONE, 200);
				}
			}
		}
	}
	// UI_MONSTER
	else if (id == "ui_boss")
		return new Sprite(_spriteSheets["ui_boss"], RectI(0, 0, 192, 192), "ui_boss");
	else if (id == "ui_bossname")
		return new Sprite(_spriteSheets["ui_bossname"], RectI(0, 0, 174, 55), "ui_bossname");
	else if (id == "ui_bossspot")
		return new Sprite(_spriteSheets["ui_bossspot"], RectI(0, 0, 260, 70), "ui_bossspot");
	else if (id == "ui_isaac")
		return new Sprite(_spriteSheets["ui_isaac"], RectI(0, 0, 112, 78), "ui_isaac");
	else if (id == "ui_isaacname")
		return new Sprite(_spriteSheets["ui_isaacname"], RectI(0, 0, 192, 64), "ui_isaacname");
	else if (id == "ui_vs")
		return new Sprite(_spriteSheets["ui_vs"], RectI(0, 0, 128, 64), "ui_vs");

	// SPRITES HUD
	else if (id == "hud_heart_red")
		return new Sprite(_spriteSheets["hud_hearts"], RectI(0, 0, 16, 16), "hud_heart_red");
	else if (id == "hud_heart_half_red")
		return new Sprite(_spriteSheets["hud_hearts"], RectI(16, 0, 16, 16), "hud_heart_half_red");
	else if (id == "hud_heart_empty")
		return new Sprite(_spriteSheets["hud_hearts"], RectI(32, 0, 16, 16), "hud_heart_empty");
	else if (id == "hud_coin")
		return new Sprite(_spriteSheets["hud_items"], RectI(0, 0, 16, 16), "hud_coin");
	else if (id == "hud_bomb")
		return new Sprite(_spriteSheets["hud_items"], RectI(0, 16, 16, 16), "hud_bomb");
	else if (id == "hud_minimap_back")
		return new Sprite(_spriteSheets["hud_minimap"], RectI(0, 0, 55, 49), "hud_minimap_back");
	else if (id == "hud_minimap_room_select")
		return new Sprite(_spriteSheets["hud_minimap"], RectI(114, 82, 9, 8), "hud_minimap_room_select");
	else if (id == "hud_minimap_room")
		return new Sprite(_spriteSheets["hud_minimap"], RectI(66, 18, 9, 8), "hud_minimap_room");
	else if (id == "hud_minimap_boss")
		return new Sprite(_spriteSheets["hud_minimap"], RectI(5, 51, 9, 8), "hud_minimap_boss");
	else if (id == "hud_minimap_treasure")
		return new Sprite(_spriteSheets["hud_minimap"], RectI(18, 52, 8, 7), "hud_minimap_treasure");
	else if (id == "hud_minimap_shop")
		return new Sprite(_spriteSheets["hud_minimap"], RectI(30, 51, 9, 8), "hud_minimap_shop");

	// SPRITES MENU
	else if (id == "menu_title")
		return new Sprite(_spriteSheets["menu_title"], RectI(0, 0, 480, 272), "menu_title");
	else if (id == "menu_title_angel")
		return new Sprite(_spriteSheets["menu_title"], RectI(0, 272, 479, 105), "menu_title_angel");
	else if (id == "menu_title_start")
	{
		rects.push_back(RectF(9, 382, 160, 158));
		rects.push_back(RectF(169, 382, 160, 158));
		return new AnimatedSprite(_spriteSheets["menu_title"], rects, 10, "menu_title_start");
	}
	else if (id == "menu_background")
		return new Sprite(_spriteSheets["menu"], RectI(0, 0, 480, 269), "menu_background");
	else if (id == "menu_emptybg")
		return new Sprite(_spriteSheets["menu_emptybg"], RectI(0, 0, 480, 270), "menu_emptybg");
	else if (id == "menu_shading")
		return new Sprite(_spriteSheets["menu_shading"], RectI(0, 0, 256, 150), "menu_shading");
	else if (id == "menu_overlay")
		return new Sprite(_spriteSheets["menu_overlay"], RectI(0, 0, 480, 270), "menu_overlay");
	else if (id == "menu_new_run")
		return new Sprite(_spriteSheets["menu"], RectI(32, 295, 121, 46), "menu_new_run");
	else if (id == "menu_continue")
		return new Sprite(_spriteSheets["menu"], RectI(0, 0, 480, 269), "menu_continue");
	else if (id == "menu_options")
		return new Sprite(_spriteSheets["menu"], RectI(32, 479, 121, 46), "menu_options");
	else if (id == "menu_exit")
		return new Sprite(_spriteSheets["menu"], RectI(32, 434, 121, 46), "menu_exit");
	else if (id == "menu_arrow")
		return new Sprite(_spriteSheets["menu"], RectI(1, 296, 31, 44), "menu_arrow");
	else if (id == "menu_options_menu")
		return new Sprite(_spriteSheets["menu_options"], RectI(0, 0, 234, 234), "menu_options_menu");
	else if (id == "menu_options_menu_dark")
		return new Sprite(_spriteSheets["menu_options_dark"], RectI(0, 0, 234, 234), "menu_options_menu_dark");
	else if (id == "menu_options_sfx")
		return new Sprite(_spriteSheets["menu_options"], RectI(0, 246, 121, 46), "menu_options_sfx");
	else if (id == "menu_options_music")
		return new Sprite(_spriteSheets["menu_options"], RectI(0, 522, 121, 46), "menu_options_music");
	else if (id == "menu_options_reset")
		return new Sprite(_spriteSheets["menu_options"], RectI(0, 568, 121, 46), "menu_options_reset");
	else if (id == "menu_options_resume")
		return new Sprite(_spriteSheets["menu_options"], RectI(0, 614, 121, 46), "menu_options_resume");
	else if (id == "menu_options_exit")
		return new Sprite(_spriteSheets["menu_options"], RectI(0, 660, 121, 46), "menu_options_exit");
	else if (id == "menu_options_10")
		return new Sprite(_spriteSheets["menu_options"], RectI(121, 246, 121, 46), "menu_options_10");
	else if (id == "menu_options_9")
		return new Sprite(_spriteSheets["menu_options"], RectI(121, 292, 121, 46), "menu_options_9");
	else if (id == "menu_options_8")
		return new Sprite(_spriteSheets["menu_options"], RectI(121, 338, 121, 46), "menu_options_8");
	else if (id == "menu_options_7")
		return new Sprite(_spriteSheets["menu_options"], RectI(121, 384, 121, 46), "menu_options_7");
	else if (id == "menu_options_6")
		return new Sprite(_spriteSheets["menu_options"], RectI(121, 430, 121, 46), "menu_options_6");
	else if (id == "menu_options_5")
		return new Sprite(_spriteSheets["menu_options"], RectI(121, 476, 121, 46), "menu_options_5");
	else if (id == "menu_options_4")
		return new Sprite(_spriteSheets["menu_options"], RectI(121, 522, 121, 46), "menu_options_4");
	else if (id == "menu_options_3")
		return new Sprite(_spriteSheets["menu_options"], RectI(121, 568, 121, 46), "menu_options_3");
	else if (id == "menu_options_2")
		return new Sprite(_spriteSheets["menu_options"], RectI(121, 614, 121, 46), "menu_options_2");
	else if (id == "menu_options_1")
		return new Sprite(_spriteSheets["menu_options"], RectI(121, 660, 121, 46), "menu_options_1");
	else if (id == "menu_options_0")
		return new Sprite(_spriteSheets["menu_options"], RectI(121, 706, 121, 46), "menu_options_0");
	else if (id == "menu_options_vsync")
		return new Sprite(_spriteSheets["menu_options"], RectI(0, 292, 121, 46), "menu_options_vsync");
	else if (id == "menu_options_vsync_on_1")
		return new Sprite(_spriteSheets["menu_options"], RectI(0, 338, 121, 46), "menu_options_vsync_on_1");
	else if (id == "menu_options_vsync_on_2")
		return new Sprite(_spriteSheets["menu_options"], RectI(0, 384, 121, 46), "menu_options_vsync_on_2");
	else if (id == "menu_options_vsync_off_1")
		return new Sprite(_spriteSheets["menu_options"], RectI(0, 430, 121, 46), "menu_options_vsync_off_1");
	else if (id == "menu_options_vsync_off_2")
		return new Sprite(_spriteSheets["menu_options"], RectI(0, 476, 121, 46), "menu_options_vsync_off_2");

	// ISAAC SPRITES
	else if (id == "isaac_headFront")
		return new Sprite(_spriteSheets["isaac"], RectI(0 * 32, 0, 32, 32), "isaac_headFront");
	else if (id == "isaac_headBack")
		return new Sprite(_spriteSheets["isaac"], RectI(4 * 32, 0, 32, 32), "isaac_headBack");
	else if (id == "isaac_headRight")
		return new Sprite(_spriteSheets["isaac"], RectI(2 * 32, 0, 32, 32), "isaac_headRight");
	else if (id == "isaac_headFrontShoot")
		return new Sprite(_spriteSheets["isaac"], RectI(1 * 32, 0, 32, 32), "isaac_headFrontShoot");
	else if (id == "isaac_headBackShoot")
		return new Sprite(_spriteSheets["isaac"], RectI(5 * 32, 0, 32, 32), "isaac_headBackShoot");
	else if (id == "isaac_headRightShoot")
		return new Sprite(_spriteSheets["isaac"], RectI(3 * 32, 0, 32, 32), "isaac_headRightShoot");
	else if (id == "isaac_bodyFront")
		return new Sprite(_spriteSheets["isaac"], RectI(0, 32, 32, 32), "isaac_bodyFront");
	else if (id == "isaac_walkDown")
	{
		for (int i = 0; i < 7; i++)
		{
			rects.push_back(RectI(i * 32, 32, 32, 32));
		}
		rects.push_back(RectI(6 * 32, 0, 32, 32));
		rects.push_back(RectI(7 * 32, 0, 32, 32));
		return new AnimatedSprite(_spriteSheets["isaac"], rects, 10, "isaac_walkDown");
	}
	else if (id == "isaac_walkRight")
	{
		for (int i = 0; i < 7; i++)
		{
			rects.push_back(RectI(i * 32, 2 * 32, 32, 32));
		}
		rects.push_back(RectI(0, 3 * 32, 32, 32));
		rects.push_back(RectI(1 * 32, 3 * 32, 32, 32));
		return new AnimatedSprite(_spriteSheets["isaac"], rects, 10, "isaac_walkRight");
	}
	// ITEM SPRITES
	else if (id == "tear_default")
		return new Sprite(_spriteSheets["tears"], RectI(192, 0, 32, 32), "tear_default");
	else if (id == "tear_explosion") {
		for (int i = 0; i < 4; i++) {
			for (int j = 0; j < 4; j++) {
				rects.push_back(RectI(j * 64, i * 64, 64, 64));
			}
		}
		return new AnimatedSprite(_spriteSheets["explosion"], rects, 20, "tear_explosion");
	}
	else if (id == "tear_wet")
		return new Sprite(_spriteSheets["shadow"], RectF(0, 0, 120, 49), "tear_wet", SDL_FLIP_NONE, rand()%5 + 5);
	else if (id == "item_coin")
	{
		rects.push_back(moveBy(RectF(0, 1, 32, 16), 0, 0, 32, 16,0,0));
		rects.push_back(moveBy(RectF(0, 1, 32, 16), 1, 0, 32, 16,0,0));
		rects.push_back(moveBy(RectF(0, 1, 32, 16), 0, 1, 32, 16,0,0));
		rects.push_back(moveBy(RectF(0, 1, 32, 16), 1, 1, 32, 16,0,0));
		rects.push_back(moveBy(RectF(0, 1, 32, 16), 0, 2, 32, 16,0,0));
		rects.push_back(moveBy(RectF(0, 1, 32, 16), 1, 2, 32, 16,0,0));
		return new AnimatedSprite(_spriteSheets["item_coin"], rects, 10, "coin");
	}
	else if (id == "item_coin_effect")
	{
		rects.push_back(moveBy(RectF(64, 0, 64, 64), 0, 0, 64, 64,0,0));
		rects.push_back(moveBy(RectF(64, 0, 64, 64), 1, 0, 64, 64,0,0));
		rects.push_back(moveBy(RectF(64, 0, 64, 64), 2, 0, 64, 64,0,0));
		rects.push_back(moveBy(RectF(64, 0, 64, 64), 0, 1, 64, 64,0,0));
		rects.push_back(moveBy(RectF(64, 0, 64, 64), 1, 1, 64, 64,0,0));
		return new AnimatedSprite(_spriteSheets["item_coin"], rects, 15, "item_coin_effect", false);
	}

	else if (id == "altar")
		return new Sprite(_spriteSheets["altar"], RectI(0, 0, 32, 32), "altar");

		//DUKE OF FLIES (MOB)
	else if (id == "duke_1")
		return new Sprite(_spriteSheets["duke"], RectF(154, 0, 77, 66), "duke");		
	else if (id == "duke_2")
		return new Sprite(_spriteSheets["duke"], RectF(0, 0, 77, 66), "duke");	
	else if (id == "duke_3")
		return new Sprite(_spriteSheets["duke"], RectF(231, 0, 77, 66), "duke");
	else if (id == "duke_4")
		return new Sprite(_spriteSheets["duke"], RectF(77, 0, 77, 66), "duke");


	else if (id == "fly")
	{
		rects.push_back(moveBy(RectF(0, 0, 31, 31), 0, 0, 31, 31));
		rects.push_back(moveBy(RectF(0, 0, 31, 31), 1, 0, 31, 31));
		return new AnimatedSprite(_spriteSheets["fly"], rects, 30, "fly");
	}

	else if (id == "dyingFly")
	{
		rects.push_back(moveBy(RectF(0, 68, 58, 55), 0, 0, 58, 55));
		rects.push_back(moveBy(RectF(0, 68, 58, 55), 1, 0, 58, 55));
		rects.push_back(moveBy(RectF(0, 68, 58, 55), 2, 0, 58, 55));
		rects.push_back(moveBy(RectF(0, 68, 58, 55), 3, 0, 58, 55));
		rects.push_back(moveBy(RectF(0, 128, 62,61), 0, 0 ,62, 61));
		rects.push_back(moveBy(RectF(0, 128, 62,61), 1, 0, 62, 61));
		rects.push_back(moveBy(RectF(0, 128, 62,61), 2, 0, 62, 61));
		rects.push_back(moveBy(RectF(0, 128, 62,61), 3, 0, 62, 61));
		rects.push_back(moveBy(RectF(0, 193, 62, 61), 0, 0, 62, 61));
		rects.push_back(moveBy(RectF(0, 193, 62, 61), 1, 0, 62, 61));
		return new AnimatedSprite(_spriteSheets["fly"], rects, 28, "fly");
	}

	else if (id == "bloodExplotion")
	{
		rects.push_back(moveBy(RectF(0, 0, 145, 100), 0, 0, 144, 103));
		rects.push_back(moveBy(RectF(0, 0, 145, 100), 1, 0, 144, 103));
		rects.push_back(moveBy(RectF(0, 0, 145, 100), 2, 0, 144, 103));
		rects.push_back(moveBy(RectF(0, 0, 145, 100), 0, 1, 144, 103));
		rects.push_back(moveBy(RectF(0, 0, 145, 100), 1, 1, 144, 103));
		rects.push_back(moveBy(RectF(0, 0, 145, 100), 2, 1, 144, 103));
		rects.push_back(moveBy(RectF(0, 0, 145, 120), 0, 2, 144, 103));
		rects.push_back(moveBy(RectF(0, 0, 145, 120), 1, 2, 144, 103));
		rects.push_back(moveBy(RectF(0, 0, 145, 120), 2, 2, 144, 103));
		rects.push_back(moveBy(RectF(0, 0, 145, 120), 0, 3, 144, 103));

		return new AnimatedSprite(_spriteSheets["bloodExplotion"], rects, 19, "bloodExplotion");
		}
	
	else if (id == "blood")
	{
		return new Sprite(_spriteSheets["bloodExplotion"], RectF(0, 220, 147, 100), "blood");
	}


	else if (id == "shadow")
	{
		return new Sprite(_spriteSheets["shadow"], RectF(0, 0, 120, 49), "shadow", SDL_FLIP_NONE, 75);
	}

	else if (id == "poof")
	{
		rects.push_back(moveBy(RectF(0, 0, 64, 66), 0, 0, 64, 66));
		rects.push_back(moveBy(RectF(0, 0, 64, 66), 1, 0, 64, 66));
		rects.push_back(moveBy(RectF(0, 0, 64, 66), 2, 0, 64, 66));
		rects.push_back(moveBy(RectF(0, 0, 64, 66), 3, 0, 64, 66));
		rects.push_back(moveBy(RectF(0, 0, 64, 66), 0, 1, 64, 66));
		rects.push_back(moveBy(RectF(0, 0, 64, 66), 1, 1, 64, 66));
		rects.push_back(moveBy(RectF(0, 0, 64, 66), 2, 1, 64, 66));
		rects.push_back(moveBy(RectF(0, 0, 64, 66), 3, 1, 64, 66));
		rects.push_back(moveBy(RectF(0, 0, 64, 100), 0, 2, 64, 100));
		rects.push_back(moveBy(RectF(0, 0, 64, 100), 1, 2, 64, 100));
		rects.push_back(moveBy(RectF(0, 0, 64, 100), 2, 2, 64, 100));
		rects.push_back(moveBy(RectF(0, 0, 64, 100), 3, 2, 64, 100));
		rects.push_back(moveBy(RectF(0, 0, 64, 100), 0, 3, 64, 100));
		rects.push_back(moveBy(RectF(0, 0, 64, 100), 1, 3, 64, 100));
		rects.push_back(moveBy(RectF(0, 0, 64, 100), 2, 3, 64, 100));
		return new  AnimatedSprite(_spriteSheets["poof"], rects, 20, "poof");
	}
	else
	{
		std::cerr << "Cannot find sprite \"" << id << "\"\n";
		return nullptr;
	}
}

Sprite* SpriteFactory::getNumber(int n, int fill)
{
	std::vector< RectI> tiles;

	std::string text = std::to_string(n);

	if (fill)
		while (text.size() != fill)
			text = '0' + text;

	for (auto& c : text)
	{
		if (isdigit(c))
			tiles.push_back(moveBy(RectI(0, 145, 9, 11), c - '0', 0, 9, 11));
	}

	return new TiledSprite(_spriteSheets["ui_font"], tiles, "number", { 0.4f, 0.4f });
}