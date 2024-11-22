#include "UIMonster.h"
#include "SpriteFactory.h"
#include "View.h"
#include "Game.h"
#include "Scheduler.h"
#include <iostream>
#include <cmath> 

using namespace agp;



UIMonster::UIMonster()
	: Scene(RectF(0, 0, 16, 12))
{

	// setup view 
	_view = new View(this, _rect);
	_view->setFixedAspectRatio(Game::instance()->aspectRatio());
	_view->setRect(RectF(0, 0, 16, 12));

	_sprites["boss"] = SpriteFactory::instance()->get("ui_boss");
	_sprites["bossname"] = SpriteFactory::instance()->get("ui_bossname");
	_sprites["bossspot"] = SpriteFactory::instance()->get("ui_bossspot");
	_sprites["isaac"] = SpriteFactory::instance()->get("ui_isaac");
	_sprites["isaacname"] = SpriteFactory::instance()->get("ui_isaacname");
	_sprites["vs"] = SpriteFactory::instance()->get("ui_vs");

	_FPS = 0;
	_dx = 0;

	_isaacspot = new MovableObject(this, RectF(1 - 8, 8, 6, 1.7f), _sprites["bossspot"], 9);
	_isaac = new MovableObject(this, RectF(1.9f - 8, 6.2f, 4, 3), _sprites["isaac"], 9);
	_bossspot = new MovableObject(this, RectF(7 + 8, 8, 9, 2.5f), _sprites["bossspot"], 9);
	_boss = new MovableObject(this, RectF(7.5f + 8, 2.5f, 8, 7.5f), _sprites["boss"], 9);
	_isaacname = new MovableObject(this, RectF(1 - 6, 1.3f, 6, 2), _sprites["isaacname"], 9);
	_vs = new MovableObject(this, RectF(6.3f, 1.3f, 3.4f, 2), _sprites["vs"], 9);
	_bossname = new MovableObject(this, RectF(9.5f + 6, 1.3f, 6, 2), _sprites["bossname"], 9);

	_isaacname->setVisible(false);
	_isaac->setVisible(false);
	_bossspot->setVisible(false);
	_boss->setVisible(false);
	_isaacname->setVisible(false);
	_vs->setVisible(false);
	_bossname->setVisible(false);

	_show = false;

}

UIMonster* UIMonster::UIMonsterBoss()
{
	UIMonster* uiMonster = new UIMonster();
	return uiMonster;
}

void UIMonster::update(float dt)
{
	Scene::update(dt);

	if (!_show)
		return;

	if (_dx > 14.6f)
	{
		setActiveUIMonster();
		return;
	}
	else if (_dx > 13)
	{
		_FPS = 0.7f;
		_isaacspot->moveBy({ _FPS * dt,0 });
		_isaac->moveBy({ _FPS * dt,0 });

		_bossspot->moveBy({ -_FPS * dt,0 });
		_boss->moveBy({ -_FPS * dt,0 });
		_vs->setAngularVelocity(0);
		_dx += _FPS * dt;
	}
	else
	{ 
		_FPS = 20;
		_dx += _FPS * dt;

		_isaacspot->moveBy({ _FPS * dt,0 });
		_isaac->moveBy({ _FPS * dt,0 });

		_bossspot->moveBy({ -_FPS * dt,0 });
		_boss->moveBy({ -_FPS * dt,0 });

		_isaacname->moveBy({ _FPS * dt,0 });
		_bossname->moveBy({ -_FPS * dt,0 });
		_vs->setAngularVelocity(-1095);

		_dx += _FPS * dt;
	}
}

void UIMonster::setActiveUIMonster()
{
	if (_show)
	{
		_show = false;
		_active = false;
		setBackgroundColor({ 0,0,0,0 });
		_isaacspot->setVisible(false);
		_isaac->setVisible(false);
		_bossspot->setVisible(false);
		_boss->setVisible(false);
		_isaacname->setVisible(false);
		_vs->setVisible(false);
		_bossname->setVisible(false);
	}
	else
	{
		_show = true;
		_active = true;
		setBackgroundColor({ 0,0,0 });
		_isaacspot->setVisible(true);
		_isaac->setVisible(true);
		_bossspot->setVisible(true);
		_boss->setVisible(true);
		_isaacname->setVisible(true);
		_vs->setVisible(true);
		_bossname->setVisible(true);
	}
}