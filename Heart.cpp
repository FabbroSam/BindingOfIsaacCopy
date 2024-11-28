#include <list>
#include <iostream>
#include "Heart.h"
#include "Isaac.h"
#include "SpriteFactory.h"
#include "Game.h"
#include "HUD.h"
#include "Tear.h"
#include "Fly.h"
#include "StaticObject.h"

using namespace agp;


Heart::Heart(Scene* scene, const PointF& pos, int layer)
    : DynamicObject(scene, RectF(pos.x, pos.y, 1.4f, 1.45f), nullptr, 3)
{
    _collider.adjust(0.4f, 0.4f, -0.4f, -0.4f);
    _scene = scene;
    _sprites["item_heart"] = SpriteFactory::instance()->get("item_heart");
    _sprites["item_coin_effect"] = SpriteFactory::instance()->get("item_coin_effect");
    _sprite = _sprites["item_heart"];

    _collidable = true;
    _compenetrable = false;
    _colliding = false;
    _x_vel_max = 3.0f;
    _y_vel_max = 3.0f;
    _x_dec_rel = 1.2f;
    _y_dec_rel = 1.2f;
    _vel = { 0.0f, 0.0f };
}

void Heart::update(float dt)
{
    DynamicObject::update(dt);
    
    if (_colliding) {
        _x_dir = Direction::NONE;
        _y_dir = Direction::NONE;
        _colliding = false;
    }
    
    resolveCollisions(dt);
    std::cout << "XPOS: " << _rect.pos.x << " -- YPOS: " << _rect.pos.y << std::endl;
}

bool Heart::collision(CollidableObject* with, Direction fromDir)
{
    HUD* hud = Game::instance()->hud();
    Isaac* isaac = dynamic_cast<Isaac*>(with);

    if (isaac && _collidable && hud->halfHearts() < 10)
    {
        destroy(isaac);
        return true;
    }
    else
    {
        _colliding = true;
        if (fromDir == Direction::LEFT)
        {
            _x_dir = Direction::RIGHT;
            velAdd({ 1.0f, 0.0f });
        }
        if (fromDir == Direction::RIGHT)
        {
            _x_dir = Direction::LEFT;
            velAdd({ -1.0f, 0.0f });
        }
        if (fromDir == Direction::DOWN)
        {
            _y_dir = Direction::UP;
            velAdd({ 0.0f, -1.0f });
        }
        if (fromDir == Direction::UP)
        {
            _y_dir = Direction::DOWN;
            velAdd({ 0.0f, 1.0f });
        }
    }

    return false;
}

bool Heart::collidableWith(CollidableObject* obj)
{
    if (obj->to<Fly*>() || obj->to<Tear*>())
        return false;
    return true;
}

void Heart::destroy(CollidableObject* isaac)
{
    _rect = RectF(isaac->rect().pos.x, isaac->rect().pos.y, 2, 2);
    _collidable = false;
    _sprite = _sprites["item_coin_effect"];
    schedule("heart_kill" + _id, 0.4f, [this]() {
        setVisible(false);
        kill();
        }, 0, false);
}