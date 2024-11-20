#include "Tear.h"
#include "SpriteFactory.h"
#include "Audio.h"
#include "AnimatedSprite.h"
#include "Game.h"
#include "Scene.h"
#include <list>
#include <iostream>
#include "Isaac.h"

using namespace agp;

Tear::Tear(Scene* scene, const PointF& pos, Direction dir, int layer)
    : DynamicObject(scene, RectF(pos.x, pos.y, 1, 1), SpriteFactory::instance()->get("tears_default"), layer)
{
    _vel = { 0.0f, 0.0f };
    _x_dec_rel = 0;
    _y_dec_rel = 0;
    
	switch (dir) {
	case Direction::LEFT:
        _vel = { -_absVel, 0.0f };
		break;
	case Direction::RIGHT:
        _vel = { _absVel, 0.0f };
		break;
	case Direction::UP:
        _vel = { 0.0f, -_absVel };
		break;
	case Direction::DOWN:
        _vel = { 0.0f, _absVel };
		break;
	default:
        _vel = { 0.0f, 0.0f };
		break;
	}
}

void Tear::update(float dt)
{
    DynamicObject::update(dt);
}

bool Tear::collision(CollidableObject* with, Direction fromDir)
{
    std::cout << with->name() << std::endl;
    Isaac* isaac = dynamic_cast<Isaac*>(with);

    if (!isaac) {
        _vel = { 0.0f,0.0f };
        _compenetrable = true;
        _scene->killObject(this);
    }

    return true;
}

