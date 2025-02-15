#include <list>
#include <iostream>
#include "Coin.h"
#include "Isaac.h"
#include "SpriteFactory.h"
#include "Game.h"
#include "Audio.h"

using namespace agp;


Coin::Coin(Scene* scene, const PointF& pos, int layer)
    : DynamicObject(scene, RectF(pos.x, pos.y, 1.4f, 1.2f), nullptr, 3)
{
    _scene = scene;
    _sprites["item_coin"] = SpriteFactory::instance()->get("item_coin");
    _sprites["item_coin_effect"] = SpriteFactory::instance()->get("item_coin_effect");
    _sprite = _sprites["item_coin"];

    _collidable = true;
    _compenetrable = true;

    // physics
    const double PI = 3.1415926535897932384650288;
    _h = _rect.pos.y;
    _x_dec_rel = 0;
    _x_dec_rel = 0;
    _vel.y = -5.0f - static_cast<float>(rand()) / RAND_MAX * 15.0f;
    _vel.x = static_cast<float>(rand()) / RAND_MAX * 3.0f - 1.5f;
    float alpha = -60;
    float d = static_cast<float>(rand()) / RAND_MAX * 1.5f;

    float u_x = static_cast<float>(cos(alpha * PI / 360));
    float u_y = -static_cast<float>(sin(alpha * PI / 360));
    float x = _rect.pos.x + d * u_x;
    _y = _rect.pos.y + d * u_y;
    //new RenderableObject(_scene, RectF(x, _y, 0.1, 0.1f), Color(255, 0, 0));
}

void Coin::update(float dt)
{
    RenderableObject::update(dt);
    //resolveCollisions(dt);

    if (_rect.pos.y > _y && _vel.y > 0)
    {
        return;
    }
    else
    {
        _vel.y += 100 * dt;
        _rect.pos.x += _vel.x * dt;
        _rect.pos.y += (50.0f * static_cast<float>(pow(dt, 2))) / 2.0f + _vel.y * dt;
    }
}

bool Coin::collision(CollidableObject* with, Direction fromDir)
{
    Isaac* isaac = dynamic_cast<Isaac*>(with);
    if (isaac && _collidable)
    {   
        Game::instance()->hud()->addCoins();
        Audio::instance()->playSound("key drop 2");
        destroy(isaac);
        return true;
    }

    return false;
}

void Coin::destroy(CollidableObject* isaac)
{
    _rect = RectF(isaac->rect().pos.x, isaac->rect().pos.y, 2, 2);
    _collidable = false;
    _sprite = _sprites["item_coin_effect"]; 
    schedule("coin_kill" + _id, 0.4f, [this]() {
        setVisible(false);
        kill();
        }, 0, false);
}