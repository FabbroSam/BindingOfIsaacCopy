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
#include "Audio.h"

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
    _x_vel_max = 4.0f;
    _y_vel_max = 4.0f;
    _x_dec_rel = 6.0f;
    _y_dec_rel = 6.0f;
    _x_acc = 6.0f;
    _y_acc = 6.0f;
    _vel = { 0.0f, 0.0f };

    //_coinsText = new RenderableObject(_scene, RectF(_rect + Vec2Df(0.4f,1.3f)), nullptr, 5);
   // _coinsText->setSprite(SpriteFactory::instance()->getNumber(1, 1));
}

void Heart::update(float dt)
{
    DynamicObject::update(dt);
    
    if (_colliding) {
        _x_dir = Direction::NONE;
        _y_dir = Direction::NONE;
        _colliding = false;
    }

}

bool Heart::collision(CollidableObject* with, Direction fromDir)
{
    HUD* hud = Game::instance()->hud();
    Isaac* isaac = dynamic_cast<Isaac*>(with);

    if (isaac && _collidable && hud->halfHearts() < 6)
    {
        hud->setHalfHearts(hud->halfHearts() + 2);
        Audio::instance()->playSound("penny pickup 1");
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
    //_coinsText->setVisible(false);
    _sprite = _sprites["item_coin_effect"];
    schedule("heart_kill" + _id, 0.4f, [this]() {
        setVisible(false);
        kill();
        }, 0, false);
}