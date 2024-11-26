#include "Enemy.h"
#include "Isaac.h"
#include "Audio.h"
#include "Scene.h"
#include "SpriteFactory.h"

using namespace agp;

Enemy::Enemy(Scene* scene, const RectF& rect, Sprite* sprite, float spawnDelay, int layer)
	: DynamicObject(scene, rect, sprite, layer)
{
	_smashable = true;
	_dying = false;
	_visible = false;
	_facingDir = Direction::LEFT;
	_spawnDelay = spawnDelay;
	
	_shadow = new RenderableObject(_scene, _rect, SpriteFactory::instance()->get("shadow"), 4);
	_shadow->setVisible(false);
	

	schedule("poofAppearing", 0.6f, [this]()
		{
			_poof = new RenderableObject(_scene, _rect, SpriteFactory::instance()->get("poof"), 6);
			_poof->setRect(_rect * Vec2Df(1.7f, 1.7f)+Vec2Df(-0.3f,-0.7f));
			
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
		}
	);
	
}

bool Enemy::collision(CollidableObject* with, Direction fromDir)
{
	Isaac* isaac = dynamic_cast<Isaac*>(with);

	if (isaac)
	{
		if (_smashable && isaac->invincible())
			smash();
		else
			isaac->hurt();

		return true;
	}

	return false;
}

void Enemy::smash()
{
	_dying = true;
	_vel.y = -8;
	_collidable = false;
	_flip = SDL_FLIP_VERTICAL;
	Audio::instance()->playSound("kick");

	schedule("die-smash", 2, [this]() {_scene->killObject(this); });
}