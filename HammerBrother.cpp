#include "HammerBrother.h"
#include "SpriteFactory.h"
#include "Hammer.h"
#include "GameScene.h"
#include "Mario.h"
#include <random>

using namespace agp;

HammerBrother::HammerBrother(Scene* scene, const PointF& pos)
	: Enemy(scene, RectF(pos.x + 1 / 16.0f, pos.y - 1, 1, 2), nullptr, 5)
{
	_collider.adjust(0.1f, 0.4f, -0.1f, -1 / 16.0f);

	_sprites["walk"] = SpriteFactory::instance()->get("hammer_brother_walk");
	_sprites["throw"] = SpriteFactory::instance()->get("hammer_brother_throw");
	_sprites["jump"] = SpriteFactory::instance()->get("hammer_brother_jump");
	_sprite = _sprites["throw"];
	_pivot = pos;
	_throwing = false;
	_chasing = false;

	// default physics
	_x_acc = 1000;
	_x_dec_rel = 0;
	_x_dec_skd = 1000;
	_x_vel_max = 1;
	_x_dir = Direction::LEFT;
	_halfRangeX = 0.7f;

	// scripting (hammer spawn loop)
	schedule("spawnHammer", 0.7f + (rand()%100)/1000.0f, [this]
		{
			if (rand() % 5 > 0 && !_dying)
			{
				new Hammer(_scene, _rect.tl(), this);
				_throwing = true;
				schedule("throwing_off", 0.5f, [this]() {_throwing = false; });
			}
		}, -1);

	// scripting (chasing Mario)
	schedule("chasing", 15.0f + rand() % 10, [this]
		{
			_chasing = true;
			_x_vel_max *= 2;
		});
}

void HammerBrother::update(float dt)
{
	Enemy::update(dt);

	Mario* mario = dynamic_cast<GameScene*>(_scene)->player();

	// state changes logic
	if(mario->rect().center().x > _rect.center().x)
		_facingDir = Direction::RIGHT;
	else
		_facingDir = Direction::LEFT;
	
	// horizontal movement
	if (_chasing)
		_x_dir = _facingDir;
	else if (_rect.center().x >= _pivot.x + _halfRangeX)
		_x_dir = Direction::LEFT;
	else if(_rect.center().x <= _pivot.x - _halfRangeX)
		_x_dir = Direction::RIGHT;

	// animations
	if(!_dying)
		_flip = _facingDir == Direction::RIGHT ? SDL_FLIP_HORIZONTAL : SDL_FLIP_NONE;
	else if(_throwing)
		_sprite = _sprites["throw"];
	else 
		_sprite = _sprites["walk"];
}