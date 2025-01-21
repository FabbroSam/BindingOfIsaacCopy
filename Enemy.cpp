#include "Enemy.h"
#include "Isaac.h"
#include "Audio.h"
#include "Scene.h"
#include "Tear.h"
#include "StaticObject.h"
#include "SpriteFactory.h"

using namespace agp;

Enemy::Enemy(Scene* scene, const RectF& rect, Sprite* sprite, float spawnDelay, int layer)
	: DynamicObject(scene, rect, sprite, layer)
{
	_hitable = true;
	_dying = false;
	_spawnDelay = spawnDelay;
	_visible = false;
	_movable = false;
	_shadow = new RenderableObject(_scene, _rect, SpriteFactory::instance()->get("shadow"), 3);
	_shadow->setVisible(false);

	//animation logic
	_fixSize = rect.size;
	_trigger = false;
	//schedule("setBounce", 3.0f, [this]() {	_bounceDirection = 1; });
	_bounceDirection = 1;
	_bounceCycle = 0;


	schedule("poofAppearing", 0.6f, [this]()
		{
			_poof = new RenderableObject(_scene, _rect, SpriteFactory::instance()->get("poof"), 6);
			_poof->setRect(_rect * Vec2Df(1.7f, 1.7f) + Vec2Df(-0.3f, -0.7f));

			schedule("enemyAppearing", 0.4f, [this]() //it appears after the poof
				{
					this->setVisible(true);
					_shadow->setVisible(true);
				}
			);
		} 
	);

	schedule("poofDisappearing", 1.1f, [this]() // poof disappearing delay = enemy appearing delay + poof appearing delay
		{
			_scene->killObject(_poof);
			_movable = true;
		}
	);
}

void Enemy::trigger()
{
	_trigger = true;
}

bool Enemy::collision(CollidableObject* with, Direction fromDir)
{

	Isaac* isaac = with->to<Isaac*>();
	if (isaac)
	{
		isaac->hurt();
		return true;
	}

	StaticObject* stobj = with->to<StaticObject*>();
	if (stobj)
		return true;

	return false;
}

void Enemy::hit(float damage, Vec2Df _dir)
{
	if (_hitable)
	{
		_life -= damage;
		if (_life <= 0)
			die();
	}
}