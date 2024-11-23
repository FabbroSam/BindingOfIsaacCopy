#include <list>
#include <iostream>
#include "Rock.h"
#include "SpriteFactory.h"
using namespace agp;


Rock::Rock(Scene* scene, const PointF& pos, int layer)
    : StaticObject(scene, RectF(pos.x, pos.y, 1.4f, 1.4f), nullptr, 3)
{
    _sprites["rock"] = SpriteFactory::instance()->get("rock");
    _sprite = _sprites["rock"];

    _collider.adjust(0.12f, 0.12f, -0.18f, -0.18f);
}

