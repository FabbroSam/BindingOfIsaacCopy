#include <list>
#include <iostream>
#include "Poop.h"
#include "SpriteFactory.h"
#include "Coin.h"
#include <stdio.h>
#include <math.h>
using namespace agp;


Poop::Poop(Scene* scene, const PointF& pos, int layer)
    : StaticObject(scene, RectF(pos.x, pos.y, 1.4f, 1.4f), nullptr, 3)
{
    _sprites["poop"] = SpriteFactory::instance()->get("poop");
    _sprites["poop1"] = SpriteFactory::instance()->get("poop_1");
    _sprites["poop2"] = SpriteFactory::instance()->get("poop_2");
    _sprites["poop3"] = SpriteFactory::instance()->get("poop_3");
    _sprites["poop4"] = SpriteFactory::instance()->get("poop_4");
    _sprites["poop_gold"] = SpriteFactory::instance()->get("poop_gold");
    _sprites["poop_gold1"] = SpriteFactory::instance()->get("poop_gold_1");
    _sprites["poop_gold2"] = SpriteFactory::instance()->get("poop_gold_2");
    _sprites["poop_gold3"] = SpriteFactory::instance()->get("poop_gold_3");
    _sprites["poop_gold4"] = SpriteFactory::instance()->get("poop_gold_4");
    _sprites["poop_explosion"] = SpriteFactory::instance()->get("poop_explosion");
    _sprites["poop_gibs"] = SpriteFactory::instance()->get("poop_gibs");

    _collider.adjust(0.12f, 0.12f, -0.18f, -0.18f);
    _type = "";
    _index = 0;

    if (rand() % 5== 0)
    {
        _type = "poop_gold";
        _sprite = _sprites[_type];
    }
    else
    {
        _type = "poop";
        _sprite = _sprites[_type];
    }
    
}

void Poop::destroy()
{
    if (_index >= 4)
        return;

    _index += 1;
    std::string type = _type + std::to_string(_index);
    _sprite = _sprites[type];
    if (_index == 4 && _type == "poop_gold")
        new Coin(_scene, PointF(_rect.pos.x, _rect.pos.y), 3);
    else
        new PoopGibs(_scene, PointF(_rect.pos.x, _rect.pos.y), _type, 3);


    if (_index >= 4)
    {
        _collidable = false;
        _explosion = new RenderableObject(_scene, RectF(_rect.pos.x - 0.8f, _rect.pos.y - 0.7f, 3.2f,3.2f), _sprites["poop_explosion"],2);
    }
}

PoopGibs::PoopGibs(Scene* scene, const PointF& pos, std::string type, int layer)
    : DynamicObject(scene, RectF(pos.x, pos.y, 0.7f, 0.7f), nullptr, 0)
{
    _sprites["poop_gibs"] = SpriteFactory::instance()->get("poop_gibs");
    _sprites["coin"] = SpriteFactory::instance()->get("item_coin");


    _sprite = _sprites["poop_gibs"];
    _collidable = false;
    _compenetrable = true;

    const double PI = 3.1415926535897932384650288;

    // physics
    _h = _rect.pos.y;
    _x_dec_rel = 0;
    _x_dec_rel = 0;
    _vel.y = -5.0f - static_cast<float>(rand()) / RAND_MAX * 15.0f;
    _vel.x = static_cast<float>(rand()) / RAND_MAX * 3.0f - 1.5f;
    float alpha = -60;
    float d = static_cast<float>(rand()) / RAND_MAX * 1.5f;

    //std::cout << "_vel.x: " << _vel.x << " alpha: " << alpha << " d " << d << std::endl;
    float u_x =  static_cast<float>(cos(alpha * PI / 360));
    float u_y = -static_cast<float>(sin(alpha * PI / 360));
    float x = _rect.pos.x + d * u_x;
    _y = _rect.pos.y + d * u_y;
    //new RenderableObject(_scene, RectF(x, _y, 0.1,0.1f), Color(255, 0, 0));  
}

void PoopGibs::update(float dt)
{
    RenderableObject::update(dt);

    if (_rect.pos.y > _y && _vel.y > 0)
    {
        return;
    }

    _rect.pos.x += _vel.x * dt;
    _rect.pos.y += static_cast<float>((50 * pow(dt, 2)) / 2 + _vel.y * dt);
    _vel.y += 100 * dt;
}